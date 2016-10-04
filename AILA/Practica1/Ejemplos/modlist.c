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

#define BUFFER_LENGTH  PAGE_SIZE


//http://stackoverflow.com/questions/19647356/linux-kernel-linked-list
//http://isis.poly.edu/kulesh/stuff/src/klist/
//https://www.cs.uic.edu/~hnagaraj/articles/linked-list/
//
//entrada en procf
static struct proc_dir_entry *proc_entry ;

//variables locales

/* Tipo de nodo */
typedef struct tNodo{
struct list_head list;
int data;
} tNodo;

/* Lista enlazada */
struct list_head modlist;



//operaciones internas
//
static void add (int valor)
{
  tNodo unNodo;
  unNodo.data = 1;
  list_add_tail(&unNodo.list, &modlist);
}

/*
static void rem (int valor)
{
	tNodo* actual=modlist;
	bool enc=false;
	int i=
	while (!enc && !empty(modlist))
	{
		actual=modlist.
	}
}*/


int GetNumber(const char *str) {
  int number=0;
  while (!(*str >= '0' && *str <= '9') && (*str != '-') && (*str != '+')) str++;  
  if (sscanf(str, "%d", &number) == 1) {
    return number;
  }
  // No int found
  return -1; 
}


static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
	int r;
	char* unBuffer;
	  if ((*off) > 0) /* The application can write in this entry just once !! */
		return 0;
	  
	  
	  /* Transfer data from user to kernel space */
	  unBuffer=(char *)vmalloc( BUFFER_LENGTH );  
	  if (copy_from_user( &unBuffer, buf, len ))  
		return -EFAULT;
	  r=GetNumber(unBuffer);
	//  add(r);
	  off+=len;            /* Update the file pointer */
	  vfree(unBuffer);
	  trace_printk("He insertado: %d\n",r);
	  
	  return 0;
};


void print_list(struct list_head* list) {
	tNodo* item=NULL;
	tNodo* cur_node=NULL;
	
	//list_for_each(cur_node, list) 
	//{
	/* item points to the structure wherein the links are embedded */
	//item = list_entry(cur_node, struct list_item, links);
	//trace_printk(KERN_INFO "%i\n",item->data);
	//}
	
}

static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
  
  
  
	  if ((*off) > 0) /* Tell the application that there is nothing left to read */
	      return 0;
	  print_list(&modlist);
	  return 0;
};







//operaciones de entrada salida
static const struct file_operations proc_entry_fops = {
    .read = modlist_read,
    .write = modlist_write,    
};

//Constructora
int init_modlist_module( void )
{
  
  int ret = 0;
  INIT_LIST_HEAD(&modlist); /* Initialize the list */
  proc_entry = proc_create( "modlist", 0666, NULL, &proc_entry_fops);
  if (proc_entry == NULL) {
  ret = -ENOMEM;
  trace_printk(KERN_INFO "modlist: Can't create /proc entry\n");
  } else   
  trace_printk(KERN_INFO "modlist: Module loaded\n");
  return ret;

 return 0;
};

module_init( init_modlist_module );

//Destructora


void exit_modlist_module( void )
{
  remove_proc_entry("modlist", NULL);
  trace_printk(KERN_INFO "modlist: Module unloaded.\n");

};
module_exit( exit_modlist_module );






