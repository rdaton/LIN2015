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


#define MAX_ITEMS_CBUF	5
#define MAX_CHARS_KBUF	10

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
		
		if (file->f_mode & FMODE_READ)	
		{ /* Un consumidor abrió el FIFO */
			/* Acceso a la crítica */
			  if (down_interruptible(&mtx))
			  {
				return -EINTR;
			  }

			  /* Bloquearse mientras no haya productor preparado */
			  while (prod_count<=0)
			  {
				/* Incremento de consumidores esperando */
				cons_count++;

				/* Liberar el 'mutex' antes de bloqueo*/
				up(&mtx);

				/* Bloqueo en cola de espera */		
				if (down_interruptible(&sem_cons)){
					down(&mtx);
					cons_count--;
					up(&mtx);		
					return -EINTR;
				}	

				/* Readquisición del 'mutex' antes de entrar a la SC */				
				if (down_interruptible(&mtx)){
					return -EINTR;
				}	
			  }

			  //abrir extremo de escritura
			  
			  /* Despertar a los lectores bloqueados (si hay alguno) */
			  if (cons_count>0)
			  {
				up(&sem_cons);	
				cons_count--;
			  }

			  /* Salir de la sección crítica */
			  up(&mtx);

			  return 0;

		} 
		else
		{ /* Un productor abrió el FIFO */
			if (file->f_mode & FMODE_WRITE) 
			{ /* Un consumidor abrió el FIFO */
				/* Acceso a la sección crítica */
				  if (down_interruptible(&mtx))
				  {
					return -EINTR;
				  }

				  /* Bloquearse mientras no haya consumidor preparado */
				  while (cons_count<=0)
				  {
					/* Incremento de productores esperando */
					prod_count++;

					/* Liberar el 'mutex' antes de bloqueo*/
					up(&mtx);

					/* Bloqueo en cola de espera */		
					if (down_interruptible(&sem_prod)){
						down(&mtx);
						prod_count--;
						up(&mtx);		
						return -EINTR;
					}	

					/* Readquisición del 'mutex' antes de entrar a la SC */				
					if (down_interruptible(&mtx)){
						return -EINTR;
					}	
				  }
				  
				  /* Despertar a los lectores bloqueados (si hay alguno) */
				  if (prod_count>0)
				  {
					up(&sem_prod);	
					prod_count--;
				  }

				  /* Salir de la sección crítica */
				  up(&mtx);
			} 
			return 0;
	}
	return -1;

}



/* Se invoca al hacer close() de entrada /proc */ 
static int fifoproc_release(struct inode *inodo, struct file *file)
{
	
	if (file->f_mode & FMODE_WRITE){
		down(&mtx);
		prod_count--;
	    up(&mtx);

	}
	else if (file->f_mode & FMODE_READ){

		down(&mtx);
		cons_count--;
	  	up(&mtx);
	  
	}

	if (cons_count==0 && prod_count==0) 
  	{	  			
  		down(&mtx);
  		clear_cbuffer_t (cbuffer); 
	  	up(&mtx); 
	} 

	return 0;
}


/* Se invoca al hacer read() de entrada /proc */ 
static ssize_t fifoproc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
		  int nr_bytes=0;
		  int* item=NULL;
		  char kbuff[MAX_CHARS_KBUF];
		  
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
		  if(size_cbuffer_t(cbuffer)==0 && prod_count){
		  		return 0;
		  }

		 /* Bloquearse mientras buffer esté vacío */
		  while (size_cbuffer_t(cbuffer)==0)
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
			
			/* Readquisición del 'mutex' antes de entrar a la SC */		
			if (down_interruptible(&mtx)){
				return -EINTR;
			}	
		  }

		  /* Obtener el primer elemento del buffer y eliminarlo */
		  item=head_cbuffer_t(cbuffer);
		  remove_cbuffer_t(cbuffer);  

		  /* Detectar fin de comunicación por error (productores cierra FIFO antes) */ 
	  		if (prod_count==0) {up(&mtx); return -EPIPE;} 

		  
		  /* Despertar a los productores bloqueados (si hay alguno) */
		  if (nr_prod_waiting>0)
		  {
			up(&sem_prod);	
			nr_prod_waiting--;
		  }

		  /* Salir de la sección crítica */	
		  up(&mtx);
		   
		  nr_bytes=sprintf(kbuff,"%i\n",*item); 
		  
		  /* Liberar memoria del elemento extraido */
		  vfree(item);
		   
		  if (len < nr_bytes)
		    return -ENOSPC;
		  
		  if (copy_to_user(buf,kbuff,nr_bytes))
		    return -EINVAL;
		   
		  (*off)+=nr_bytes;  /* Update the file pointer */

		  return nr_bytes;
  }


/* Se invoca al hacer write() de entrada /proc */ 
static ssize_t fifoproc_write(struct file *flip, const char *buf, size_t len, loff_t *off)
{
	char kbuf[MAX_CHARS_KBUF+1];
	  int val=0;
	  int* item=NULL;

	  if ((*off) > 0) /* The application can write in this entry just once !! */
	    return 0;
	  
	  if (len > MAX_CHARS_KBUF) {
	    return -ENOSPC;
	  }
	  if (copy_from_user( kbuf, buf, len )) {
	    return -EFAULT;
	  }

	  kbuf[len] ='\0'; 
	   *off+=len;            /* Update the file pointer */
	   
	  if (sscanf(kbuf,"%i",&val)!=1)
	  {
		return -EINVAL;
	  }
		
	  item=vmalloc(sizeof(int));

	  (*item)=val;

	  /* Acceso a la sección crítica */
	  if (down_interruptible(&mtx))
	  {
		return -EINTR;
	  }

	  /* Bloquearse mientras no haya huecos en el buffer */
	  while (is_full_cbuffer_t(cbuffer))
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

			/* Readquisición del 'mutex' antes de entrar a la SC */				
			if (down_interruptible(&mtx)){
				return -EINTR;
			}	
  	  }

  	  /* Si se intenta escribir en el FIFO cuando no hay consumidores
  	   (extremo de lectura cerrado), el módulo devolverá un error*/
  	  if(cons_count == 0){
  	  	return -EINTR;
  	  }

		   /* Insertar en el buffer */
	  insert_cbuffer_t(cbuffer,item); 

	  /* Detectar fin de comunicación por error (consumidor cierra FIFO antes) */ 
	  if (cons_count==0) {up(&mtx); return -EPIPE;} 
	  
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


static const struct file_operations proc_entry_fops = {
    .read = fifoproc_read,
    .write = fifoproc_write, 
    .open = fifoproc_open,
    .release =  fifoproc_release,
};

/* Funciones de inicialización y descarga del módulo */ 
int init_module(void){
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

  proc_entry = proc_create_data("fifomod",0666, NULL, &proc_entry_fops, NULL);
    
  if (proc_entry == NULL) {
      destroy_cbuffer_t(cbuffer);
      printk(KERN_INFO "fifomod: No puedo crear la entrada en proc\n");
      return  -ENOMEM;
  }
      
  printk(KERN_INFO "fifomod: Cargado el Modulo.\n");
  
  return 0;
}
void cleanup_module(void){
	remove_proc_entry("fifomod", NULL);
  destroy_cbuffer_t(cbuffer);
  printk(KERN_INFO "fifomod: Modulo descargado.\n");
}

module_init( init_module );
module_exit( cleanup_module );