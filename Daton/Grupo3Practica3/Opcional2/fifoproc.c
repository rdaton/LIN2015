//http://www.linuxdevcenter.com/pub/a/linux/2007/07/05/devhelloworld-a-simple-introduction-to-device-drivers-under-linux.html?page=2
//http://codereview.stackexchange.com/questions/31938/simple-linux-char-driver
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <asm-generic/errno.h>
#include <linux/semaphore.h>
#include <linux/kfifo.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include<linux/cdev.h>


MODULE_LICENSE("GPL");

#define FIFO_SIZE 64 //potencia de 2
#define MAX_KBUF 64
typedef STRUCT_KFIFO_REC_1(FIFO_SIZE) tipoKfifo;	
static tipoKfifo unKFifo; /* Buffer circular */
int Major;
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
    
   
     if (down_interruptible(&mtx))
        {
        return -EINTR;
        }

    if (file->f_mode & FMODE_READ)  
    { /* Un consumidor abrió el FIFO */
      /* Acceso a la crítica */
     
       cons_count++;
       
        /* Bloquearse mientras no haya productor preparado */
        while (prod_count<=0)
        {
          /* Incremento de consumidores esperando */
          nr_cons_waiting++;
          up(&mtx);
         
          /* Bloqueo en cola de espera */   
          if (down_interruptible(&sem_cons)){  
           
             down(&mtx);
            nr_cons_waiting--;
            up(&mtx);
            return -EINTR;
          }
            if(down_interruptible(&mtx))
            {
              return -EINTR;
            }     
        }
        /* Despertar a los productores bloqueados (si hay alguno) */
        while (nr_prod_waiting>0)
        {
          up(&sem_prod);  
          nr_prod_waiting--;
        }
    } 
    else
    { /* Un productor abrió el FIFO */
      if (file->f_mode & FMODE_WRITE) 
      { /* Un consumidor abrió el FIFO */
          prod_count++;
          /* Bloquearse mientras no haya consumidor preparado */
          while (cons_count<=0)
          {
            /* Incremento de productores esperando */
            nr_prod_waiting++;
             up(&mtx);
            /* Bloqueo en cola de espera */   
            if (down_interruptible(&sem_prod)){
              down(&mtx);
              nr_prod_waiting--;
              up(&mtx); 
              return -EINTR;
            }
            if (down_interruptible(&mtx)){return -EINTR;} 
          }
          /* Despertar a los consumidores bloqueados (si hay alguno) */
          if (nr_cons_waiting>0)
          {
          up(&sem_cons);  
          nr_cons_waiting--;
          }
      }

  /* Salir de la sección crítica */
    }
  up(&mtx);
  return 0;

}



/* Se invoca al hacer close() de entrada /proc */ 
static int fifoproc_release(struct inode *inodo, struct file *file)
{
  down(&mtx);
  if (file->f_mode & FMODE_WRITE){
    prod_count--;
    up(&sem_prod);
     
  }
  else if (file->f_mode & FMODE_READ){
    cons_count--;
    up(&sem_cons);
    
  }

  if (cons_count==0 && prod_count==0) 
  {         
       kfifo_reset(&unKFifo);
  } 

  up(&mtx);
  return 0;
}


/* Se invoca al hacer read() de entrada /proc */ 
static ssize_t fifoproc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
 
  if (len> FIFO_SIZE || len> MAX_KBUF)  {
      return -ENOSPC;
    }
      char kbuff[MAX_KBUF];
      /* Entrar a la sección crítica */
      if (down_interruptible(&mtx))
      {
      return -EINTR;
      }

     /* Bloquearse mientras buffer esté vacío */
      while (kfifo_is_empty(&unKFifo)  && prod_count > 0)
      {
      /* Incremento de consumidores esperando */
        nr_cons_waiting++;
        /* Liberar el 'mutex' antes de bloqueo*/
        up(&mtx);
        /* Bloqueo en cola de espera */   
        if (down_interruptible(&sem_cons)){ 
          down(&mtx);
          nr_cons_waiting--;
          up(&mtx);   
          return -EINTR;
        }
        if (down_interruptible(&mtx))
        {
          return -EINTR;
        }  
      }

     
      if(kfifo_is_empty(&unKFifo) && prod_count==0){
          up(&mtx);
          return 0;
      } 
        /* Obtener los elementos del buffer y eliminarlo */
      kfifo_out (&unKFifo, kbuff, len);
      /* Despertar a los productores bloqueados (si hay alguno) */
      if (nr_prod_waiting>0)
      {
      up(&sem_prod);  
      nr_prod_waiting--;
      }       

       /* Salir de la sección crítica */ 
      up(&mtx);


      if (copy_to_user(buf,kbuff,len)){
         return -EINVAL;
      }
     
      return len;
  }


/* Se invoca al hacer write() de entrada /proc */ 
static ssize_t fifoproc_write(struct file *flip, const char *buf, size_t len, loff_t *off)
{
    char kbuf[MAX_KBUF];
    
    if (len> FIFO_SIZE || len> MAX_KBUF)  {
      return -ENOSPC;
    }
   
    if (copy_from_user( kbuf, buf, len )) {
      return -EFAULT;
    }
    
    /* Acceso a la sección crítica */
    if (down_interruptible(&mtx))
    {
    return -EINTR;
    }
     printk("w tam %d",kfifo_len(&unKFifo));
     nr_prod_waiting++;
    /* Bloquearse mientras no haya huecos en el buffer */
    while ( kfifo_avail(&unKFifo)<len && cons_count >0 )
    {
      /* Incremento de productores esperando */
      nr_prod_waiting++;
      /* Liberar el 'mutex' antes de bloqueo*/
      up(&mtx);
      /* Bloqueo en cola de espera */   
      if (down_interruptible(&sem_prod)){
        down(&mtx);
        nr_prod_waiting--;
        up(&mtx); 
        return -EINTR;
      }
      if (down_interruptible(&mtx)){return -EINTR;} 
    }

    /* Detectar fin de comunicación por error (consumidor cierra FIFO antes) */ 
    if (cons_count==0) 
    {
      up(&mtx);
      return -EPIPE;
    } 
     
       /* Insertar en el buffer */
   kfifo_in(&unKFifo,kbuf,len);
    /* Despertar a los consumidores bloqueados (si hay alguno) */
    if (nr_cons_waiting>0)
    {
      up(&sem_cons);  
      nr_cons_waiting--;
    }

    /* Salir de la sección crítica */
      up(&mtx);
      
    return len;
}



static const struct file_operations fops = {
    .read = fifoproc_read,
    .write = fifoproc_write, 
    .open = fifoproc_open,
    .release =  fifoproc_release,
};



static int major;
static dev_t dev;
static struct cdev my_cdev;





/* Funciones de inicialización y descarga del módulo */ 
int init_cons_module(void){
   INIT_KFIFO(unKFifo);
  /* Inicialización a 0 de los semáforos usados como colas de espera */
  sema_init(&sem_prod,0);
  sema_init(&sem_cons,0);

  /* Inicializacion a 1 del semáforo que permite acceso en exclusión mutua a la SC */
  sema_init(&mtx,1);

  nr_prod_waiting=nr_cons_waiting=0;

  Major = register_chrdev(0, "patata", &fops);

   if (Major < 0) {
     printk ("Registering the character device failed with %d\n", Major);
     return Major;
   }

   printk("<1>I was assigned major number %d.  To talk to\n", Major);
   printk("<1>the driver, create a dev file with\n");
   printk("'mknod /dev/hello c %d 0'.\n", Major);
   printk("<1>Try various minor numbers.  Try to cat and echo to\n");
   printk("the device file.\n");
   printk("<1>Remove the device file and module when done.\n");

   return 0;
 
  printk(KERN_INFO "fifomod: Cargado el Modulo.\n");
  
  return 0;
  
 
}
void cleanup_cons_module(void){
   unregister_chrdev(Major, "patata");
  
}

module_init( init_cons_module );
module_exit( cleanup_cons_module );
