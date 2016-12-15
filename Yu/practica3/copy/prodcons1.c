#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <asm-generic/errno.h>
#include <linux/semaphore.h>
#include "cbuffer.h"


#define MAX_ITEMS_CBUF  5
#define MAX_CHARS_KBUF  64

MODULE_LICENSE("GPL");

static struct proc_dir_entry *proc_entry;
cbuffer_t* cbuffer; /* Buffer circular */ 
int prod_count = 0; /* Número de procesos que abrieron la entrada /proc para escritura (productores) */ 
int cons_count = 0; /* Número de procesos que abrieron la entrada /proc para lectura (consumidores) */ 
struct semaphore mtx; /* para garantizar Exclusión Mutua */ 
struct semaphore sem_prod; /* cola de espera para productor(es) */ 
struct semaphore sem_cons; /* cola de espera para consumidor(es) */ 
int nr_prod_waiting=0; /* Número de procesos productores esperando */ 
int nr_cons_waiting=0; /* Número de procesos consumidores esperando */

//struct inode *inode, struct file *file
/* Se invoca al hacer open() de entrada /proc */ 
static int fifoproc_open(struct inode *inode, struct file *file)
{
    /*acceso a la sesion critica*/
    if (down_interruptible(&mtx))
        {
        return -EINTR;
        }
    
    
    printk("entro open \n");
    if (file->f_mode & FMODE_READ)  
    { /* Un consumidor abrió el FIFO */
      printk("soy consumidor\n");
         cons_count++;

        /* Bloquearse mientras no haya productor preparado */
        while (prod_count<=0)
        {
          /* Incremento de consumidores esperando */
          nr_cons_waiting++;
          printk("consumidor: espero en la cola mientras que no hay productor, soy numero %d\n",nr_cons_waiting);

          /* Liberar el 'mutex' antes de bloqueo*/
          up(&mtx);

          /* Bloqueo en cola de espera */   
          if (down_interruptible(&sem_cons)){
            down(&mtx);
             printk("consumidor: salgo de la cola, soy numero %d\n",nr_cons_waiting);
            nr_cons_waiting--;
            up(&mtx);   
            return -EINTR;
          }   
        }
       /* Despertar a los productores bloqueados (si hay alguno) */
          if (nr_prod_waiting>0)
          {
             printk("consumidor : voy a despertar productor numeor %d\n",nr_prod_waiting);
          up(&sem_prod);  
          nr_prod_waiting--;
          }
    } 
    else
    { /* Un productor abrió el FIFO */
      if (file->f_mode & FMODE_WRITE) 
      { /* Un consumidor abrió el FIFO */
        printk("soy productor\n");
          prod_count++;
         
          /* Bloquearse mientras no haya consumidor preparado */
          while (cons_count<=0)
          {
          /* Incremento de productores esperando */
          nr_prod_waiting++;
          printk("productor: espero mientras que no hay consumidor, soy numero %d\n",nr_prod_waiting);

          /* Liberar el 'mutex' antes de bloqueo*/
          up(&mtx);

          /* Bloqueo en cola de espera */   
          if (down_interruptible(&sem_prod)){
            down(&mtx);
            printk("productor: salgo de la cola de espera porque se supone que ya hay consumidor");
            nr_prod_waiting--;
            up(&mtx);   
            return -EINTR;
          }
        }
           /* Despertar a los consumidores bloqueados (si hay alguno) */
      while (nr_cons_waiting>0)
        {
          printk("productor : voy a despertar consumidor numeor %d\n",nr_cons_waiting);
        up(&sem_cons);  
        nr_cons_waiting--;
        }
      } 
  }
  /* Salir de la sección crítica */
     up(&mtx);
    return 0;

}



/* Se invoca al hacer close() de entrada /proc */ 
static int fifoproc_release(struct inode *inodo, struct file *file)
{
   down(&mtx);
  printk("entro release\n");
  if (file->f_mode & FMODE_WRITE){
    printk("productor release el numero %d \n",prod_count);
    prod_count--;
      up(&sem_prod);
  }
  else if (file->f_mode & FMODE_READ){
 printk("consumidor release el numero %d\n",cons_count);
    cons_count--;
    up(&sem_cons);
  }

  if (cons_count==0 && prod_count==0) 
    {         
       printk("libero la tuberia \n");
        clear_cbuffer_t (cbuffer);
  } 
  up(&mtx);
  return 0;
}


/* Se invoca al hacer read() de entrada /proc */ 
static ssize_t fifoproc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
   printk("consumidor empieza a consumir \n");
      int nr_bytes=0;
      int* item=NULL;
      char kbuff[MAX_CHARS_KBUF+1];
      
      if ((*off) > 0) 
          return 0;

      /* Entrar a la sección crítica */
      if (down_interruptible(&mtx))
      {
      return -EINTR;
      }

      /*Si se intenta hacer una lectura del FIFO cuando el 
      buffer circular esté vacío y no haya productores, el 
      módulo devolverá el valor 0 (EOF)*/
      
      if(size_cbuffer_t(cbuffer)==0 && prod_count==0){
        printk("consumidor: no hay nada en cbuffer y tampoco hay productor \n");
        up(&mtx);
          return 0;
      }

     /* Bloquearse mientras buffer esté vacío */
      while (size_cbuffer_t(cbuffer)==0  && prod_count > 0)
      {
      /* Incremento de consumidores esperando */
        nr_cons_waiting++;
        printk("espero en la cola de consumidor mientras que no haya nada que leer, numero de consumidor es %d\n", nr_cons_waiting);

        /* Liberar el 'mutex' antes de bloqueo*/
        up(&mtx);
           /*despierto los productores*/ 
        while(nr_prod_waiting > 0){
             up(&prod_count);
             nr_prod_waiting--;
        }
        
        /* Bloqueo en cola de espera */   
        if (down_interruptible(&sem_cons)){
          down(&mtx);
          printk("salgo de la cola de espera de consumidor, soy numero %d\n",nr_cons_waiting);
          nr_cons_waiting--;
          up(&mtx);   
          return -EINTR;
        } 
      }
    
      printk("consumirdor: voy a eliminar elemento\n");
      /* Obtener el primer elemento del buffer y eliminarlo */
      item=head_cbuffer_t(cbuffer);
    /*borrar el contenido en el cbuffer mientras obteniendo una copia en item*/
      remove_items_cbuffer_t (cbuffer, item, nr_bytes);
      printk("consumidor: elemento ya esta eliminado \n");
        /*pasar la copia a kbuff*/
     nr_bytes=sprintf(kbuff,"%i\n",*item);
    
     if (len < nr_bytes){
        printk("consumidor: no hay espacio, retorno -ENOSPC \n");
        up(&mtx);
        return -ENOSPC;
      }

      printk("voy a copiar kbuf a user \n");
      if (copy_to_user(buf,kbuff,nr_bytes)){
        up(&mtx);
         return -EINVAL;
      }
        printk("he mandado kbuf a user \n");
      /* Liberar memoria del elemento extraido */
      //vfree(item);
      /* Despertar a los productores bloqueados (si hay alguno) */
      if (nr_prod_waiting>0)
      {
        printk("consumidor: voy a despertar productor numero %d\n",prod_count);
        up(&sem_prod);  
        nr_prod_waiting--;
      }
      
      (*off)+=nr_bytes;  /* Update the file pointer */
      printk("consumidor:  termina de consumir \nvalor de retorno de write es %d \n",nr_bytes);
    /* Salir de la sección crítica */ 
      up(&mtx);
      return nr_bytes;
  }




/* Se invoca al hacer write() de entrada /proc */ 
static ssize_t fifoproc_write(struct file *flip, const char *buf, size_t len, loff_t *off)
{

  printk("productor empieza a producir \n");
  char kbuf[MAX_CHARS_KBUF+1];
    //int val=0;
 //   int* item=NULL;

    if ((*off) > 0) /* The application can write in this entry just once !! */
      return 0;
    
    if (len > MAX_CHARS_KBUF) {
      printk("no hay espacio suficiente, len tiene tamanio %d\n",len);
      return -ENOSPC;
    }
   
    if (copy_from_user( kbuf, buf, len )) {
      return -EFAULT;
    }
    printk("productor: he mandado valor de kbuf a buf, valor de buf es %s, valor de kbuf es %s, valor de len es %d\n",*buf,*kbuf,len);
    
    kbuf[len] ='\0'; 
     *off+=len;            /* Update the file pointer */
  //  item=vmalloc(sizeof(int));
  // (*item)=kbuf;
    printk("prductor : voy a entrar sesion critica\n");
    
    
    /* Acceso a la sección crítica */
    if (down_interruptible(&mtx))
    {
    return -EINTR;
    }

    /* Bloquearse mientras no haya huecos en el buffer */
    while (is_full_cbuffer_t(cbuffer) && cons_count >0 )
    {
      /* Incremento de productores esperando */
      nr_prod_waiting++;
      printk("productor: mientras que no haya hueco,espero, soy numero %d\n",prod_count);
      /* Liberar el 'mutex' antes de bloqueo*/
      up(&mtx);
       /*despierto los consumidores*/ 
        while(nr_cons_waiting > 0){
             up(&cons_count);
             nr_cons_waiting--;
        }
     

      /* Bloqueo en cola de espera */   
      if (down_interruptible(&sem_prod)){
        down(&mtx);
        printk("productor: salgo de la cola de espera\n");
        nr_prod_waiting--;
        up(&mtx);   
        return -EINTR;
      } 
    }


      /* Si se intenta escribir en el FIFO cuando no hay consumidores
       (extremo de lectura cerrado), el módulo devolverá un error*/
      if(cons_count == 0){
        printk("productor: no hay consumidor\n");
        up(&mtx);
        return -EINTR;
      }

      printk("empiezo a escribir \n");
       /* Insertar en el buffer */
    insert_items_cbuffer_t (cbuffer,kbuf,len);
        
    printk("productor : termino de escribir y en el cbuffer tiene size %d\n",size_cbuffer_t(cbuffer)); 
    
    /* Despertar a los consumidores bloqueados (si hay alguno) */
    if (nr_cons_waiting>0)
    {
      printk("productor: despierto un consumidor numero %d \n",cons_count);
    up(&sem_cons);  
    nr_cons_waiting--;
    }

    /* Salir de la sección crítica */
    up(&mtx);
    printk("productor: termina de producir, valor de retorno de write es %d \n",len);
    return len;
}


static const struct file_operations proc_entry_fops = {
    .read = fifoproc_read,
    .write = fifoproc_write, 
    .open = fifoproc_open,
    .release =  fifoproc_release,
};

/* Funciones de inicialización y descarga del módulo */ 
int init_cons_module(void){
  /* Inicialización del buffer */  
  cbuffer = create_cbuffer_t(MAX_ITEMS_CBUF);

  if (!cbuffer) {
    return -ENOMEM;
  }

  /* Inicialización a 0 de los semáforos usados como colas de espera */
  sema_init(&sem_prod,0);
  sema_init(&sem_cons,0);

  /* Inicializacion a 1 del semáforo que permite acceso en exclusión mutua a la SC */
  sema_init(&mtx,1);

  nr_prod_waiting=nr_cons_waiting=0;

  proc_entry = proc_create_data("modfifo",0666, NULL, &proc_entry_fops, NULL);
    
  if (proc_entry == NULL) {
      destroy_cbuffer_t(cbuffer);
      printk(KERN_INFO "fifomod: No puedo crear la entrada en proc\n");
      return  -ENOMEM;
  }
      
  printk(KERN_INFO "fifomod: Cargado el Modulo.\n");
  
  return 0;
}
void cleanup_cons_module(void){
  remove_proc_entry("modfifo", NULL);
  destroy_cbuffer_t(cbuffer);
  printk(KERN_INFO "modfifo: Modulo descargado.\n");
}

module_init( init_cons_module );
module_exit( cleanup_cons_module );
