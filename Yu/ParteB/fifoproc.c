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


#define MAX_CBUFFER_LEN  64
#define MAX_KBUF  64

MODULE_LICENSE("GPL");

static struct proc_dir_entry *proc_entry;
cbuffer_t* cbuffer0; /* Buffer circular */ 
cbuffer_t* cbuffer1; /* Buffer circular */ 

struct semaphore mtx0;  
struct semaphore mtx1;  
struct semaphore mtx_lectura1; 
struct semaphore mtx_escritura1;  
struct semaphore mtx_lectura0;  
struct semaphore mtx_escritura0; 

int fifo0_lectura = 0; 
int fifo0_escritura = 0; 
int fifo1_lectura = 0; 
int fifo1_escritura = 0;

//struct semaphore sem_cons; /* cola de espera para consumidor(es) */ 
//int nr_prod_waiting=0; /* Número de procesos productores esperando */ 
//int nr_cons_waiting=0; /* Número de procesos consumidores esperando */

//struct inode *inode, struct file *file
/* Se invoca al hacer open() de entrada /proc */ 

static int fifoproc_open(struct inode *inode, struct file *file)
{

  printk("\nentro open\n");
    printk("\nentro read");
    int t=(int)PDE_DATA(file->f_inode);
    printk("valor de t es %d",t);    
   

     //es para cuando ya estan en conexsion los usuarios, el tercero no puede entrar
    if(fifo1_lectura==2){
      return -1;
    }


  if(t==0){

      if (file->f_mode & FMODE_READ)  
      { 
         fifo0_lectura++;
         
         //mientras que no haya gente para escribir
          while (fifo0_escritura<=0)
          {
            //up(&mtx0);
             if(down_interruptible(&mtx_escritura0))
            {
                return -EINTR;
            }     
          }
          up(&mtx_lectura0);
      }
      else
      { 
        if (file->f_mode & FMODE_WRITE) 
        { 
            fifo0_escritura++;

            while (fifo0_lectura<=0)
            {
               //up(&mtx0);
              if (down_interruptible(&mtx_lectura0))
              {
                return -EINTR;
              } 
            }
        }
      }
  }
    
  else{
        if (file->f_mode & FMODE_READ)  
        { 
             fifo1_lectura++;
             
             //mientras que no haya gente para escribir
              while (fifo1_escritura<=0)
              {
                up(&mtx1);
                 
                if(down_interruptible(&mtx1))
                {
                    return -EINTR;
                }     
              }
          }
        else
        { 
          if (file->f_mode & FMODE_WRITE) 
          { 
              fifo1_escritura++;

              while (fifo1_lectura<=0 )
              {
                 up(&mtx1);
                if (down_interruptible(&mtx1)){
                  return -EINTR;} 
              }
          }
        }
    }
    printk("\nsalgo de open\n");
    return 0;
}
  







/* Se invoca al hacer read() de entrada /proc */ 
static ssize_t fifoproc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
   
    printk("\nentro read");
    int t=(int)PDE_DATA(filp->f_inode);
    printk("valor de t es %d",t);

      char kbuff[MAX_KBUF];

      if (len> MAX_CBUFFER_LEN || len> MAX_KBUF) 
      {
        return -ENOSPC;
      } 
    

      if(t==1){

          /* Entrar a la sección crítica */
          if (down_interruptible(&mtx1))
          {
            return -EINTR;
          }
            /* Bloquearse mientras buffer1 esté vacío */
          while (size_cbuffer_t(cbuffer1)==0 && fifo0_escritura >0 )
          {
            /* Liberar el 'mutex' antes de bloqueo*/
            up(&mtx1);
            if (down_interruptible(&mtx_lectura1))
            {
              return -EINTR;
            }  
          }
            /* Obtener el primer elemento del buffer1 y eliminarlo */
          remove_items_cbuffer_t (cbuffer1, kbuff, len);

          //despertar al que esta esperando a escribir
          up(&mtx_escritura1); 
          up(&mtx1);
      }

      else{

          /* Entrar a la sección crítica */
          if (down_interruptible(&mtx0))
          {
          return -EINTR;
          }
                /* Bloquearse mientras buffer0 esté vacío */
          while (size_cbuffer_t(cbuffer0)==0 && fifo0_escritura >0 )
          {
            /* Liberar el 'mutex' antes de bloqueo*/
            up(&mtx0);
            if (down_interruptible(&mtx_lectura0))
            {
              return -EINTR;
            }  
          }
              /* Obtener el primer elemento del buffer0 y eliminarlo */
            remove_items_cbuffer_t (cbuffer0, kbuff, len); 

            //despertar al que esta esperando a escribir
            up(&mtx_escritura0);
            up(&mtx0);
           
      }

      if (copy_to_user(buf,kbuff,len)){
         return -EINVAL;
      }
      
      /* Detectar fin de comunicación por error (lector de cbuffer1 cierra FIFO antes) */ 
          if (fifo1_escritura==0) 
          {
            //up(&mtx_lectura1);
            up(&mtx1);
            return -EPIPE;
          } 

          if (fifo0_escritura==0) 
          {
            //up(&mtx_lectura1);
            up(&mtx1);
            return -EPIPE;
          } 


      printk("\nsalgo read");
      return len;
}


/* Se invoca al hacer write() de entrada /proc */ 
static ssize_t fifoproc_write(struct file *flip, const char *buf, size_t len, loff_t *off)
{


  printk("\nentro write");
  int t=(int)PDE_DATA(flip->f_inode);
  printk("valor de t es %d",t);
  char kbuf[MAX_KBUF];
   
    
    
    if (len > MAX_CBUFFER_LEN || len > MAX_KBUF) {
      return -ENOSPC;
    }
   
    if (copy_from_user( kbuf, buf, len )) {
      return -EFAULT;
    }
  

    if(t=1){
            /* Acceso a la sección crítica */
          if (down_interruptible(&mtx1))
          {
            
            return -EINTR;
          }
                    /* Bloquearse mientras no haya huecos en el buffer */
          while ( nr_gaps_cbuffer_t(cbuffer1)<len && fifo1_lectura >0 )
          {
            up(&mtx1);
            if (down_interruptible(&mtx_escritura1)){return -EINTR;} 
          }

          /* Detectar fin de comunicación por error (lector de cbuffer1 cierra FIFO antes) */ 
        /*  if (fifo1_lectura==0) 
          {
            //up(&mtx_lectura1);
            up(&mtx1);
            return -EPIPE;
          } 
          */
             /* Insertar en el buffer */
          insert_items_cbuffer_t(cbuffer1,kbuf,len);

          /* Despertar al que esta esperando a leer */
            up(&mtx_lectura1);  
            up(&mtx1);
    }
    else{
            /* Acceso a la sección crítica */
          if (down_interruptible(&mtx0))
          {
            return -EINTR;
          }
                    /* Bloquearse mientras no haya huecos en el buffer */
          while ( nr_gaps_cbuffer_t(cbuffer0)<len && fifo0_lectura >0 )
          {
            up(&mtx0);
            if (down_interruptible(&mtx_escritura0)){return -EINTR;} 
          }

          /* Detectar fin de comunicación por error (lector de cbuffer1 cierra FIFO antes) */ 
          /*if (fifo0_lectura==0) 
          {
            //up(&mtx_lectura1);
            up(&mtx0);
            return -EPIPE;
          } 
          */
             /* Insertar en el buffer */
          insert_items_cbuffer_t(cbuffer0,kbuf,len);

          /* Despertar al que esta esperando a leer */
            up(&mtx_lectura0);  
            up(&mtx0);
    }
    

      printk("\nsalgo write");
    return len;
}



/* Se invoca al hacer close() de entrada /proc */ 
static int fifoproc_release(struct inode *inodo, struct file *file)
{
        printk("\nentro release");
        int t=(int)PDE_DATA(file->f_inode);
        printk("valor de t es %d",t);

        if(t == 1){
              down(&mtx1);

              if (file->f_mode & FMODE_WRITE){
                fifo1_escritura--;
                up(&mtx_escritura1);//aunque en este caso esta operacion no es necesario, porque solamente hay un unico escritor1
                 
              }
              else if (file->f_mode & FMODE_READ){
                fifo1_lectura--;
                up(&mtx_lectura1);
                
              }

              if (fifo1_escritura==0 && fifo1_lectura==0) 
              {         
                   clear_cbuffer_t (cbuffer1);
              } 
              up(&mtx1);
        }
        else{
             down(&mtx0);

                if (file->f_mode & FMODE_WRITE){
                  fifo0_escritura--;
                  up(&mtx_escritura0);//aunque en este caso esta operacion no es necesario, porque solamente hay un unico escritor1
                   
                }
                else if (file->f_mode & FMODE_READ){
                  fifo0_lectura--;
                  up(&mtx_lectura0);
                  
                }

                if (fifo0_escritura==0 && fifo0_lectura==0) 
                {         
                     clear_cbuffer_t (cbuffer0);
                } 
                up(&mtx0);
        }


  printk("\nsalgo release");
  return 0;
}

static const struct file_operations proc_entry_fops = {
    .read = fifoproc_read,
    .write = fifoproc_write, 
   // .open = fifoproc_open,
    .release =  fifoproc_release,
};

/* Funciones de inicialización y descarga del módulo */ 
int init_cons_module(void){
  /* Inicialización del buffer */  
  cbuffer0 = create_cbuffer_t(MAX_CBUFFER_LEN);

  if (!cbuffer0) {
    return -ENOMEM;
  }

  cbuffer1 = create_cbuffer_t(MAX_CBUFFER_LEN);

  if (!cbuffer1) {
    return -ENOMEM;
  }

  /* Inicialización a 0 de los semáforos usados como colas de espera */
  sema_init(&mtx_lectura1,0);
  sema_init(&mtx_escritura0,0);
  sema_init(&mtx_lectura1,1);
  sema_init(&mtx_escritura0,1);

  /* Inicializacion a 1 del semáforo que permite acceso en exclusión mutua a la SC */
  sema_init(&mtx0,1);
  sema_init(&mtx1,1);
  int t0=0;
  int t1=1;
  //nr_prod_waiting=nr_cons_waiting=0;

  proc_entry = proc_create_data("fifo0",0666, NULL, &proc_entry_fops, (void *)t0);
    
  if (proc_entry == NULL) {
      destroy_cbuffer_t(cbuffer0);
      destroy_cbuffer_t(cbuffer1);
      printk(KERN_INFO "fifo0: No puedo crear la entrada en proc\n");
      return  -ENOMEM;
  }
      
  printk(KERN_INFO "fifo0: Cargado el Modulo.\n");

  proc_entry = proc_create_data("fifo1",0666, NULL, &proc_entry_fops, (void *)t1);
    
  if (proc_entry == NULL) {
        destroy_cbuffer_t(cbuffer0);
        destroy_cbuffer_t(cbuffer1);
        printk(KERN_INFO "fifo1: No puedo crear la entrada en proc\n");
      return  -ENOMEM;
  }
      
  printk(KERN_INFO "fifo1: Cargado el Modulo.\n");

  
  return 0;
}
void cleanup_cons_module(void){
  remove_proc_entry("fifo0", NULL);
  remove_proc_entry("fifo1", NULL);
  destroy_cbuffer_t(cbuffer0);
  destroy_cbuffer_t(cbuffer1);
  printk(KERN_INFO "modfifo: Modulo descargado.\n");
}

module_init( init_cons_module );
module_exit( cleanup_cons_module );
