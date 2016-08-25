#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <linux/ftrace.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modlist module - FDI-UCM");
MODULE_AUTHOR("Roumen Daton");



//entrada en proc
static struct proc_dir_entry *proc_entry;

//operaciones de entrada salida
static const struct file_operations proc_entry_fops = {
    .read = modlist_read,
    .write = modlist_write,    
};


//variables locales

/* Tipo de nodos */

typedef struct {
struct list_head links;
int data;
}list_item_t;

#define TAM_ELEM sizeof(

static LIST_HEAD(modlist); /* Lista enlazada */

//operaciones internas
//http://stackoverflow.com/questions/33933344/adding-items-in-kernel-linked-list
static void inserta (int valor)
{
	struct list_item_t* nuevo_elemento = (list_item_t*) vmalloc(sizeof(*nuevo_elemento));
	nuevo_elemento->data=valor;
	list_add_tail(nuevo_elemento,modList);
	
}

static list_item_t dameElemento(int valor)
{
	
}


//API del módulo
static const struct file_operations proc_entry_fops = {
    .read = modlist_read,
    .write = modlist_write,
};





static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
  //int available_space = BUFFER_LENGTH-1;
  
  if ((*off) > 0) /* The application can write in this entry just once !! */
    return 0;
  
  /*if (len > available_space) {
    printk(KERN_INFO "clipboard: not enough space!!\n");
    return -ENOSPC;
  }
  */
  /* Transfer data from user to kernel space */
  /*
  if (copy_from_user( &clipboard[0], buf, len ))  
    return -EFAULT;
*/
 // clipboard[len] = '\0'; /* Add the `\0' */  
  
  //*off+=len;            /* Update the file pointer */
  
  //trace_printk("Current value of clipboard: %s\n",clipboard);
  
  return 0;
}

static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
  
  int nr_bytes;
  
  //if ((*off) > 0) /* Tell the application that there is nothing left to read */
  //    return 0;
    
  //nr_bytes=strlen(clipboard);
    
  //if (len<nr_bytes)
  //  return -ENOSPC;
  
    /* Transfer data from the kernel to userspace */  
  //if (copy_to_user(buf, clipboard,nr_bytes))
  //  return -EINVAL;
    
  //(*off)+=len;  /* Update the file pointer */

  //return nr_bytes; 
  return 0
}






//Constructora
module_init( init_modlist_module );



int init_modlist_module( void )
{
  int ret = 0;
  proc_entry = proc_create( "modlist", 0666, NULL, &proc_entry_fops);
  if (proc_entry == NULL) {
  ret = -ENOMEM;
  vfree(modlist);
  printk(KERN_INFO "modlist: Can't create /proc entry\n");
  } else 
  
  printk(KERN_INFO "modlist: Module loaded\n");
  
  }
  
  
 
  return ret;

}


//Destructora
module_exit( exit_modlist_module );

void exit_modlist_module( void )
{
  remove_proc_entry("modlist", NULL);
  vfree(modlist);
  printk(KERN_INFO "modlist: Module unloaded.\n");
}
