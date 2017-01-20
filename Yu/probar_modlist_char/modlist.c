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
MODULE_AUTHOR(" Yu Liu; Roumen Daton");

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
char* data;
} tNodo;

/* Lista enlazada */
struct list_head modlist;



//operaciones internas
//
static int add(char*  valor) 
{
  tNodo* unNodo=(tNodo*)(vmalloc(sizeof (tNodo)));
  if (unNodo==NULL){
	if (valor!=NULL){
		vfree(valor);
	}
		vfree(unNodo);
 	 	return -ENOMEM;	
  }

  unNodo->data = valor;
  list_add_tail(&(unNodo->list), &modlist);
  return 0;
  /*	
	 char* tem_char=(char *)vmalloc( sizeof(char));
     char ini='9';
    // tem_char=&ini;
      
    if (sscanf(&ini,"%s",tem_char)==1){
    	printk("\nentro add se ha copiado bien\n");
    	printk("\nentro add valor de tem_char es %c\n",*tem_char);
    	printk("\nentro add valor de valor es %c\n",*valor);

    }
    else{
    	    	printk("\nNo se ha copiado bien\n");

    }
	 unNodo->data = tem_char;
  
*/
}

static int push (char* valor) 
{
  tNodo* unNodo=(tNodo*)(vmalloc(sizeof (tNodo)));
  if (unNodo==NULL){
	if (valor!=NULL)
		vfree(valor);

  	vfree(unNodo);
  	return -ENOMEM;	
  }
	
  unNodo->data = valor;
  list_add(&(unNodo->list), &modlist);
  return 0;
}


static void pop(struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	//trace_printk(KERN_INFO "%s\n","limpiando con pop");

	int i=0;
	list_for_each_safe(cur_node,lista_aux,list) 
	{
		
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);
	list_del(cur_node);
		
		if (item->data!=NULL)
		  vfree (item->data);
		
		vfree(item);	
		i++;

		if(i==1){
			break;
		}
		
	}

}

static void limpiar(struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	//trace_printk(KERN_INFO "%s\n","limpiando");
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);
	list_del(cur_node);
		
		if (item->data!=NULL)
		  vfree (item->data);
		
		vfree(item);	
		
	}

}

static int remove (char* valor,  struct list_head* list){
	tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	struct list_head* lista_aux=NULL;
	//trace_printk(KERN_INFO "Entra metodo de remove\n");
	list_for_each_safe(cur_node,lista_aux,list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);

	if(strcmp(item->data,valor)==0){
		//trace_printk(KERN_INFO "el valor que va a eliminar es %i\n",valor);
		list_del(cur_node);

		if (item->data!=NULL)
			vfree(item->data);

		vfree(item);
		}
	}
	return 0;

}

void print_list(struct list_head *list) {
        tNodo* item=NULL;
	struct list_head* cur_node=NULL;
	//trace_printk(KERN_INFO "%s\n","imprimiendo");
	list_for_each(cur_node, list) 
	{
	/* item points to the structure wherein the links are embedded */
	item = list_entry(cur_node,tNodo, list);
	//trace_printk(KERN_INFO "%i\n",item->data);
	}
	
}





static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {

	char* r=(char *)vmalloc( BUFFER_LENGTH ); 

	
	char* unBuffer;
	  if ((*off) > 0) /* The application can write in this entry just once !! */
		return 0;
	  
	  
	  /* Transfer data from user to kernel space */
	  unBuffer=(char *)vmalloc( BUFFER_LENGTH );  
	  if (copy_from_user( &unBuffer[0], buf, len )){
	  	vfree(unBuffer);

		vfree(r); 

	  	return -EFAULT;
	  }  
		

		unBuffer[len]='\0';
		//trace_printk(unBuffer);
	char* tem_char1=(char *)vmalloc( sizeof(char));
	char* tem_char2=(char *)vmalloc( sizeof(char));
	char* tem_char3=(char *)vmalloc( sizeof(char));
	char* tem_char4=(char *)vmalloc( sizeof(char));


    //char ini[3]={'a','\n','b'};
    char ini1={'a'};
    char ini2={'b'};
    char ini3={'c'};
    char ini4={'0'};

    if (sscanf(&ini1,"%c\n",tem_char1)==1){
    	printk("\nse ha copiado bien\n");
    	printk("\nvalor de tem_char1 es %c\n",*tem_char1);}
    else{printk("\nNo se ha copiado bien\n");}

     if (sscanf(&ini2,"%c\n",tem_char2)==1){
    	printk("\nse ha copiado bien\n");
    	printk("\nvalor de tem_char2 es %c\n",*tem_char2);}
    else{printk("\nNo se ha copiado bien\n");}
     if (sscanf(&ini3,"%c\n",tem_char3)==1){
    	printk("\nse ha copiado bien\n");
    	printk("\nvalor de tem_char3 es %c\n",*tem_char3);}
    else{printk("\nNo se ha copiado bien\n");}

    if (sscanf(&ini4,"%s\n",tem_char4)==1){
    	printk("\nse ha copiado bien\n");
    	printk("\nvalor de tem_char4 es %c\n",*tem_char4);}
    else{printk("\nNo se ha copiado bien\n");}

   //  tem_char=&ini;

		//trace_printk("add opt"); 
	  if(sscanf(unBuffer,"add %c",r)==1){
	  	printk("\nvalor de r es %c\n",*r);
	  //	char* rr=r+(sizeof(char));
	 // 	printk("\nvalor de r es %s\n",*rr);
	  		//add(r);
	  	add(tem_char1);
	  	add(tem_char2);
	  	add(tem_char3);
	  	add(tem_char4);
	  		//trace_printk("He insertado: %d\n",r);
	  }
	  else 
	
	  	//trace_printk("rem opt");
		if(sscanf(unBuffer,"remove %s\n",r)==1){
	 
	  		remove(r,&modlist);
	  		//trace_printk("intentando a borrar: %d\n",r);
	  		print_list(&modlist);
	  }
	  
	   else 
	
	   	//trace_printk("push opt");
		if(sscanf(unBuffer,"push %s\n",r)==1){
	
	  		push(r);
	  		//trace_printk("intentando a push: %d\n",r);
	  		print_list(&modlist);
	  }
	   else if(strcmp(unBuffer,"cleanup\n")==0){
	  		limpiar(&modlist);
	  		//trace_printk("intentando a limpiar todo");
	  		print_list(&modlist);
	  }
	   else if(strcmp(unBuffer,"pop\n")==0){
	  		pop(&modlist);
	  		//trace_printk("intentando a hacer pop");
	  		print_list(&modlist);
	  }
	  /*
	  else if(strcmp(unBuffer,"sort\n")==0){
	  		sort(&modlist);
	  		//trace_printk("intentando a ordenar");
	  		print_list(&modlist);
	  }
*/
	  else{
	  	//trace_printk(unBuffer);
	  	//trace_printk("error de introccion de comando");
	  	vfree(unBuffer);
		
		vfree(r); 
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


	
	char* dest=unBuffer;
	list_for_each(cur_node, list) 
	{
	// item points to the structure wherein the links are embedded 

	item = list_entry(cur_node,tNodo, list);
	//trace_printk(KERN_INFO "%i\n",item->data);
	
	dest+=sprintf(dest,"%s\n",item->data);
	
	
	}
	return dest-unBuffer;
}


static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    int nr_bytes;
   
	char* unBuffer;
	  if ((*off) > 0) /* Tell the application that there is nothing left to read */
	      return 0;

	 unBuffer=(char *)vmalloc( BUFFER_LENGTH);//aqui somo uno mas es para poder poner final de array un '\0'
 	
 	nr_bytes=generaVector(unBuffer,&modlist);
 	
 	//nr_bytes=strlen(unBuffer);

	if (len<nr_bytes){
		vfree(unBuffer);
		return -ENOSPC;
	}
    limpiar(&modlist);

   int num_aleatorio =111; 
   char tmp[]="Hola mundo";
   //char tmp2[]="adios mundo";

   //char tmp3[50];
   //char ini[sizeof(num_aleatorio)];
   //char* valor=ini;
 	char valor[400];
  	int tam_valor=sprintf(valor,"%i",num_aleatorio);

  	printk("\nvalor de valor es %s, tamanio es %i\n",valor,tam_valor);
  	strcat(valor,tmp);
  	printk("--------------%s\n",valor);
 	//strcat(&tmp,&tmp2);

 	printk("\nvalor de temp es %s\n",tmp);
 	//sprintf(tmp3,"%i__%s",num_aleatorio,tmp2);

 	
 	//printk("\nvalor de temp2 es %s\n",tmp2);
 	//printk("\nvalor de temp3 es %s\n",tmp3);
  //unBuffer[nr_bytes]='\0';
  int tam_buf=sprintf(unBuffer,"%s\n",valor);
  
  unBuffer[50]='\0';


int numero=12;
  char* ttt;
  ttt=(char*)(vmalloc(sizeof(numero)));
  ttt=(char*)(&numero);
  //unbuffer=ttt;

  printk("\nvalor de unbuffer es %s, tamanio de buf es %i\n",unBuffer,tam_buf);
    /* Transfer data from the kernel to userspace */  
  if (copy_to_user(buf, ttt,sizeof(numero)))//el numero es valor en bytes
  		{
  		vfree(unBuffer);
  	 return -EINVAL;
  }
   
 
  printk("\n%s----------------\n",buf);
  printk("%s",unBuffer);
  print_list(&modlist);
    
  (*off)+=50;  /* Update the file pointer */

	vfree(unBuffer);
  return tam_buf; 
	 
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
  proc_entry = proc_create( "modlist_mio", 0666, NULL, &proc_entry_fops);
  if (proc_entry == NULL) {
  ret = -ENOMEM;
  //trace_printk(KERN_INFO "modlist: Can't create /proc entry\n");
  }// else   
  printk("modlist: Module loaded\n");

/*  int num=12;
  char* c=(char*)(&num);
  printk("valor de c es %c\n",c);
  int* tmp=(int*)(c);
  printk("\nvalor de tmp es %i\n",tmp);
*/

  return ret;
 

 
};

module_init( init_modlist_module );

//Destructora


void exit_modlist_module( void )
{
  remove_proc_entry("modlist_mio", NULL);
  limpiar(&modlist);
  //trace_printk(KERN_INFO "modlist: Module unloaded.\n");
  print_list(&modlist);

};
module_exit( exit_modlist_module );
