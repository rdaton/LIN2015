#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <linux/ftrace.h>
#include <linux/list.h>
#include <linux/spinlock.h>

 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modlist module - FDI-UCM; Thread-Safe");
MODULE_AUTHOR("Roumen Daton; Yu Liu");

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
static int longitud=0;
DEFINE_RWLOCK(rwl);



//operaciones internas
//

static int hayEspacio(void)
{
	int num_elem_max=100;
  return (longitud < num_elem_max) ;
}

static int add (int valor)
{
  int error=1;
  tNodo* unNodo=(tNodo*)(vmalloc(sizeof (tNodo)));
  if (unNodo==NULL){
  	return -ENOMEM;	
  }	
  unNodo->data = valor;
  
  //sección critica lista de enteros
  write_lock(&rwl);
  
  if (hayEspacio())
  {
    list_add_tail(&(unNodo->list), &modlist);
    error=0;
    longitud++;
  };
  write_unlock(&rwl);
  //fin sección critica lista de enteros
  
  if (error) 
  {
    vfree(unNodo);
    return -ENOSPC;	
  }
  
  return 0;
}


static void limpiar(struct list_head* list){
	tNodo* item=NULL;
	tNodo* listaNodos[longitud];
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	//trace_printk(KERN_INFO "%s\n","limpiando");
	
	//sección critica lista de enteros
	//unsigned long flags=0;
	int i=0;
	int longitud_aux=0;
	
	//write_lock_irqsave(&rwl,flags); no habrá interrupciones que
	//accedan a la lista, así que uso
	write_lock(&rwl);
	longitud_aux=longitud;
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	  item = list_entry(cur_node,tNodo, list);
	  list_del(cur_node);
	  listaNodos[i]=item;
	  i++;	
	}
	longitud=0;
	write_unlock(&rwl);
	//fin sección critica lista de enteros
	
// 	//hago el vfree fuera del spinlock
	for (i=0;i<longitud_aux;i++)
	  vfree(listaNodos[i]);
	

}


static int remove (int valor,struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	tNodo* listaNodos[longitud];
	int longitud_aux=0;
	int i=0;
	//trace_printk(KERN_INFO "Entra metodo de remove\n");
	
	//sección critica lista de enteros
	//unsigned long flags=0;
	
	//write_lock_irqsave(&rwl,flags); no habrá interrupciones que
	//accedan a la lista, así que uso
	write_lock(&rwl);
	longitud_aux=longitud;
	
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	  item = list_entry(cur_node,tNodo, list);
	  if((item->data) == valor)
	  {
		  //trace_printk(KERN_INFO "el valor que va a eliminar es %i\n",valor);
		  list_del(cur_node);
		  listaNodos[i]=item;
		  longitud--;
		  i++;
	  }
	 
	}
	//write_unlock_irqrestore(&rwl,flags);
	write_unlock(&rwl);
	//fin sección critica lista de enteros	
	// 	//hago el vfree fuera del spinlock
	for (i=0;i<longitud_aux;i++)
	  vfree(listaNodos[i]);
	return 0;

}


/*
void print_list(struct list_head *list) {
        tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	//trace_printk(KERN_INFO "%s\n","imprimiendo");
	
	//sección critica lista de enteros
	read_lock(&rwl);
	list_for_each(cur_node, list) 
	{
	// item points to the structure wherein the links are embedded 
	  item = list_entry(cur_node,tNodo, list);
	//trace_printk(KERN_INFO "%i\n",item->data);
	}
	read_unlock(&rwl);
	//fin sección critica lista de enteros
	
}
*/




static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
	int r;
	
	char* unBuffer;
	  if ((*off) > 0) /* The application can write in this entry just once !! */
		return 0;
	  
	  
	  /* Transfer data from user to kernel space */
	  unBuffer=(char *)vmalloc( BUFFER_LENGTH );  
	  if (copy_from_user( &unBuffer[0], buf, len )){
	  	vfree(unBuffer);
	  	return -EFAULT;
	  }  
		

	unBuffer[len]='\0';
		//trace_printk(unBuffer);
	  if(sscanf(unBuffer,"add %i",&r)==1){
	  		add(r);
	  		//trace_printk("He insertado: %d\n",r);

	  }
	  else if(sscanf(unBuffer,"remove %i\n",&r)==1){
	  		remove(r,&modlist);
	  		//trace_printk("intentando a borrar: %d\n",r);
	  		//print_list(&modlist);
	  }
	   else if(strcmp(unBuffer,"cleanup\n")==0){
	  		limpiar(&modlist);
	  		//trace_printk("intentando a limpiar todo");
	  		//print_list(&modlist);
	  }

	  else{
	  	//trace_printk(unBuffer);
	  	//trace_printk("error de introccion de comando");
	  	vfree(unBuffer);
	  	return -EFAULT;
	  };
		

	  *off+=len;            /* Update the file pointer */
	  vfree(unBuffer);
	 // 
	  
	  return len;
};









int generaVector(char* unBuffer,struct list_head* list){
	//struct list_head* list=&Modlist;
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	//trace_printk(KERN_INFO "%s\n","imprimiendo");
	int long_max_buffer=9;
	char miniBuffer[long_max_buffer];	
	char* dest=unBuffer;
	int nr_chars,len;
	//sección critica
	read_lock(&rwl);
	
	list_for_each(cur_node, list) 
	{
		// item points to the structure wherein the links are embedded 

		item = list_entry(cur_node,tNodo, list);
		//trace_printk(KERN_INFO "%i\n",item->data);
	
		//AQUI HAY QUE HACER UNA CONVERSION ASIGNANDO EL VALOR A LA VARIABLE C
		len=sprintf(miniBuffer,"%i\n",item->data))
		if (len+nr_chars> BUFFER_LENGTH )
			break;
			
		dest+=sprintf(dest,"%i\n",item->data);
		nr_chars+=len;
		
	}
	read_unlock(&rwl);
	//fin sección critica
	return dest-unBuffer;
}


static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    int num_elem;
    //!!!!!!!!tipo de unBuffer igual podemos declararlo directamente con tipo int que facilita luego al rellenarlo con
    //los valores de tipo int. Es mas, Supongo que el metodo copy_to_user recibe cualquier tipo 
	char* unBuffer;
	  if ((*off) > 0) /* Tell the application that there is nothing left to read */
	      return 0;

	 unBuffer=(char *)vmalloc( BUFFER_LENGTH);//aqui somo uno mas es para poder poner final de array un '\0'

 	num_elem=generaVector(unBuffer,&modlist);
 
 	//nr_bytes=strlen(unBuffer);

	if (len<num_elem){
		vfree(unBuffer);
		return -ENOSPC;
	}
    

  //unBuffer[nr_bytes]='\0';
    /* Transfer data from the kernel to userspace */  
  if (copy_to_user(buf, unBuffer,num_elem)){
  		vfree(unBuffer);
  	 return -EINVAL;
  }
   

  //print_list(&modlist);
    
  (*off)+=len;  /* Update the file pointer */

	vfree(unBuffer);
  return num_elem; 


	 
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
  //trace_printk(KERN_INFO "modlist: Can't create /proc entry\n");
  } else   
  return ret;
 

 
};

module_init( init_modlist_module );

//Destructora


void exit_modlist_module( void )
{
  remove_proc_entry("modlist", NULL);
  limpiar(&modlist);
  //trace_printk(KERN_INFO "modlist: Module unloaded.\n");
  //print_list(&modlist);

};
module_exit( exit_modlist_module );
