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
typedef struct{
struct list_head list;
int data;
} tNodo;

/* Lista enlazada */
struct list_head modlist;



//operaciones internas
//


static int add (int valor)
{
  tNodo* unNodo=(tNodo*)(vmalloc(sizeof (tNodo)));
  if (unNodo==NULL)
	return -ENOMEM;	
  unNodo->data = valor;
  list_add_tail(&(unNodo->list), &modlist);
  return 0;
}

static void limpiar(struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	trace_printk(KERN_INFO "%s\n","limpiando");
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);
	list_del(cur_node);
		vfree(item);	
		
	}

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
 
 trace_printk("\nempiezo a copiar\n");
 
	int numero=NULL;
	
 if(sscanf(str,"add %i",&numero)==1){
 	
 		trace_printk("es add");
 		return numero;
 		
 	}
 else if(sscanf(str,"r %i",&numero)==1){
 
 		trace_printk("es remove");
 			return numero;
 }	
 else{
 	trace_printk("es nada");
 }
 
 	return -1;
 }

 
  /*while (!(*str >= '0' && *str <= '9') && (*str != '-') && (*str != '+')) str++;  
  if (sscanf(str, "%d", &number) == 1) {
    return number;
  }*/
  // No int found

	
		
  


static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
	int r;
	char* unBuffer;
	  if ((*off) > 0) /* The application can write in this entry just once !! */
		return 0;
	  
	  
	  /* Transfer data from user to kernel space */
	  unBuffer=(char *)vmalloc( BUFFER_LENGTH );  
	  if (copy_from_user( &unBuffer[0], buf, len ))  
		return -EFAULT;

	
	  r=GetNumber(unBuffer);
		add(r);
	  *off+=len;            /* Update the file pointer */
	  vfree(unBuffer);
	  trace_printk("He insertado: %d\n",r);
	  
	  return len;
};



static int remove (int valor,struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	trace_printk(KERN_INFO "Entra metodo de remove\n");
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);

	if((item->data) == valor){
		trace_printk(KERN_INFO "el valor que va a eliminar es %i\n",valor);
		list_del(cur_node);
		vfree(item);

		}
	}
	return 0;

}

void print_list(struct list_head *list) {
        tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	trace_printk(KERN_INFO "%s\n","imprimiendo");
	list_for_each(cur_node, list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);
	trace_printk(KERN_INFO "%i\n",item->data);
	}
	
}



void generaVector(char* unBuffer,struct list_head* list){
	//struct list_head* list=&Modlist;
	  tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	trace_printk(KERN_INFO "%s\n","imprimiendo");


	int i;
	i=0;
	
	list_for_each(cur_node, list) 
	{
	// item points to the structure wherein the links are embedded 

	item = list_entry(cur_node,tNodo, list);
	trace_printk(KERN_INFO "%i\n",item->data);
	int temp=item->data;
	//AQUI HAY QUE HACER UNA CONVERSION ASIGNANDO EL VALOR A LA VARIABLE C
	sprintf(unBuffer[i],"%d",temp);
	
	}
}


static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    int nr_bytes;
    //!!!!!!!!tipo de unBuffer igual podemos declararlo directamente con tipo int que facilita luego al rellenarlo con
    //los valores de tipo int. Es mas, Supongo que el metodo copy_to_user recibe cualquier tipo 
	char* unBuffer;
	  if ((*off) > 0) /* Tell the application that there is nothing left to read */
	      return 0;
	 unBuffer=(char *)vmalloc( BUFFER_LENGTH+sizeof(char));//aqui somo uno mas es para poder poner final de array un '\0'
 	
 	//generaVector(unBuffer);
 	
 	nr_bytes=strlen(unBuffer);

	if (len<nr_bytes)
    return -ENOSPC;

  unBuffer[nr_bytes]='\0';
    /* Transfer data from the kernel to userspace */  
  if (copy_to_user(buf, unBuffer,nr_bytes))
    return -EINVAL;

  
    
  (*off)+=len;  /* Update the file pointer */

  return nr_bytes; 


	 
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
  trace_printk(KERN_INFO "modlist: Module loaded, es aquiiiii?\n");
add(1);
add(2);
add(3);
trace_printk(KERN_INFO "entra metodo remove\n");
remove(2,&modlist);
trace_printk(KERN_INFO "sele metodo remove\n");
print_list(&modlist);

GetNumber("r3");

  return ret;
 

 return 0;
};

module_init( init_modlist_module );

//Destructora


void exit_modlist_module( void )
{
  remove_proc_entry("modlist", NULL);
  limpiar(&modlist);
  trace_printk(KERN_INFO "modlist: Module unloaded.\n");
  print_list(&modlist);

};
module_exit( exit_modlist_module );
