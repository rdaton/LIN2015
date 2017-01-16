#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/random.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include "cbuffer.h"
#include <linux/list.h>
#include <asm-generic/uaccess.h>


#define MAX_ITEMS_CBUF  10
#define BUFFER_LENGTH  PAGE_SIZE

DEFINE_RWLOCK(sp);
DEFINE_RWLOCK(rwl);

MODULE_LICENSE("GPL");

struct timer_list my_timer; /* Structure that describes the kernel timer */
cbuffer_t* cbuffer; /* Buffer circular */ 
struct work_struct my_work;/* Work descriptor */
static struct proc_dir_entry *proc_entry ;
static int timer_period_ms=1;
static int emergency_threshold=80;//por centaje 80%
static int max_random= 100;
static int longitud=0;

/* Tipo de nodo */
typedef struct{
    struct list_head list;
    char* data;
} tNodo;

/* Lista enlazada */
struct list_head modlist;


static void limpiar(struct list_head* list){
    tNodo* item=NULL;
    tNodo* listaNodos[longitud];
    struct list_head* cur_node=NULL;
    struct list_head* lista_aux=NULL;


  int i=0;
  int longitud_aux=0;
  longitud_aux=longitud;
      write_lock(&rwl);
    //trace_printk(KERN_INFO "%s\n","limpiando");
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

    //hago el vfree fuera del spinlock
  for (i=0;i<longitud_aux;i++)
    vfree(listaNodos[i]);
}

void print_list(struct list_head *list) {
  //printk("\nentra print\n");
        tNodo* item=NULL;
  struct list_head* cur_node=NULL;
  //trace_printk(KERN_INFO "%s\n","imprimiendo");
  
  list_for_each(cur_node, list) 
  {
    
  // item points to the structure wherein the links are embedded 
    item = list_entry(cur_node,tNodo, list);
    //&b=item->data;
      printk("valor es %c\n",*(item->data));
  }
  //read_unlock(&rwl);
  //fin sección critica lista de enteros
  
}

static int add(char* valor) 
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

   //sección critica lista de enteros
  write_lock(&rwl);
  list_add_tail(&(unNodo->list), &modlist);
  longitud++;
  printk("\nse ha metido el nuevo dato\n");
  write_unlock(&rwl);
  //fin sección critica lista de enteros

  print_list(&modlist);
  return 0;
}



int generaVector(char* unBuffer,struct list_head* list){
  //struct list_head* list=&Modlist;
  printk("\nentro generate vector\n");
    tNodo* item=NULL;
  struct list_head* cur_node=NULL;
  //trace_printk(KERN_INFO "%s\n","imprimiendo");
  
  char* dest=unBuffer;

  //sección critica
  read_lock(&rwl);
  list_for_each(cur_node, list) 
  {
  // item points to the structure wherein the links are embedded 

  item = list_entry(cur_node,tNodo, list);
  //trace_printk(KERN_INFO "%i\n",item->data);
  
  dest+=sprintf(dest,"%s\n",item->data);
  
  }
  printk("\nentro leer e imprimo la lista\n");
  print_list(&modlist);
  printk("\ntermino de  leer e imprimo la lista\n");
  printk("\nsalgo de  generate vector\n");
  
  read_unlock(&rwl);
  //fin sección critica
  return dest-unBuffer;
}


static ssize_t read_config(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    


    int num_elem;

    char* unBuffer;
    if ((*off) > 0) /* Tell the application that there is nothing left to read */
        return 0;

       

   unBuffer=(char *)vmalloc( BUFFER_LENGTH);//aqui somo uno mas es para poder poner final de array un '\0'

  num_elem=generaVector(unBuffer,&modlist);
 
  //nr_bytes=strlen(unBuffer);

  if (len<num_elem){
//    printk("\nentra read 1\n");
    vfree(unBuffer);
    return -ENOSPC;
  }
    

    /* Transfer data from the kernel to userspace */  
  if (copy_to_user(buf, unBuffer,num_elem)){
      vfree(unBuffer);
     return -EINVAL;
  }
   
  (*off)+=len;  /* Update the file pointer */

  vfree(unBuffer);
  return num_elem; 
   
};


static ssize_t read_timer(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    
      printk("\nentro leer e imprimo la lista\n");
      print_list(&modlist);
       printk("\ntermino de  leer e imprimo la lista\n");


    int num_elem;

  char* unBuffer;
    if ((*off) > 0) /* Tell the application that there is nothing left to read */
        return 0;

       

   unBuffer=(char *)vmalloc( BUFFER_LENGTH);//aqui somo uno mas es para poder poner final de array un '\0'

  num_elem=generaVector(unBuffer,&modlist);
 
  //nr_bytes=strlen(unBuffer);

  if (len<num_elem){
//    printk("\nentra read 1\n");
    vfree(unBuffer);
    return -ENOSPC;
  }
    

    /* Transfer data from the kernel to userspace */  
  if (copy_to_user(buf, unBuffer,num_elem)){
      vfree(unBuffer);
     return -EINVAL;
  }
   

  printk("\nentro leer e imprimo la lista otra vez\n");
  print_list(&modlist);
  limpiar(&modlist);
  printk("\ndespues de limpiar voy a imprimir\n");
  print_list(&modlist);
  (*off)+=len;  /* Update the file pointer */

  printk("\nentra -------------\n");
  vfree(unBuffer);
  printk("\nsalgo...............\n");
  return num_elem; 
   
};

static ssize_t write_config(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
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
    
    //timer_period_ms 500


  unBuffer[len]='\0';

   if(sscanf(unBuffer,"timer_period_ms %i",&r)==1){
        timer_period_ms = r;
        printk("\n----------------------He insertado: %d\n",r);
      }
    else{
      //trace_printk(unBuffer);
      //trace_printk("error de introccion de comando");
      vfree(unBuffer);
      return -EFAULT;
    };
    

    *off+=len;            /* Update the file pointer */
    vfree(unBuffer);
   
    
    return len;
};

/* Work's handler function */
static void copy_items_into_list ( struct work_struct *work )
{
    int num_temp=size_cbuffer_t (cbuffer);
    char temp[num_temp];
    
    
    unsigned long flags = 0;
    
//entra sesion critica
      read_lock_irqsave(&sp,flags);
      remove_items_cbuffer_t (cbuffer, temp, num_temp); 
     read_unlock_irqrestore(&sp,flags);
//sale de sesion critica
    int i;
    for(i=0;i<num_temp;i++){
        char* kbuff=(char *)vmalloc(sizeof(char));
      if (sscanf(&temp[i],"%c\n",kbuff)==1){
          printk("\nse ha copiado bien\n");
          printk("\nvalor de tem_char es %c\n",*kbuff);
          add(kbuff);
      }
    }

   

     
  
    print_list(&modlist);
    printk("\nTermino de copiar elementos a la lista despues de volcar elementos a cbuffer\n");
   
}


/* Function invoked when timer expires (fires) */
static void fire_timer(unsigned long data)
{   
    float capacidad=MAX_ITEMS_CBUF;
    int numero_cpu =  smp_processor_id();
    unsigned int num_aleatorio=5;
    unsigned long flags = 0;
      

    /*num_aleatorio = (unsigned int)(get_random_int());
 
    while (num_aleatorio > max_random-1){
        num_aleatorio = get_random_int();
    }*/

       // char* a=(char*)(&num_aleatorio);

    
    char ini[sizeof(num_aleatorio)];
    char* dest=ini;
    int len=0;

     len=sprintf(dest,"%i\n",num_aleatorio);
     //si uso %s sale bien el valor, si uso c%, sale otro
     //pero en la funcion copy_items_into_list tengo que hacerlo reves!!
    printk("\nvalor de dest es %s\n y el len es %i",dest,len);
    
     int num_elem=size_cbuffer_t (cbuffer);
     int porcentaje = (((float)num_elem)/capacidad)*100;
           /* Insertar en el buffer */
    // printk("\nnumero de cpu es %i\n",numero_cpu);
     
     if(porcentaje <= emergency_threshold){
        printk("\nporcentaje es %i\n",porcentaje);

//entra sesion critica
         write_lock_irqsave(&sp,flags);

        insert_items_cbuffer_t(cbuffer,dest,1);
         //insert_cbuffer_t(cbuffer,*dest);

         printk("\nNo esta lleno\n");
        write_unlock_irqrestore(&sp,flags);
  //sale sesion critica
     }else{
         printk("\nesta lleno\n");
         if(numero_cpu%2 == 0){
          printk("\nejecuta cpu 1\n");
            schedule_work_on(1,&my_work);
         }
         else{
          printk("\nejecuta cpu 0\n");
            schedule_work_on(0,&my_work);
         }

     }
      
        /* Re-activate the timer one second from now */
    mod_timer( &(my_timer), jiffies + HZ); 
}



//operaciones de entrada salida
static const struct file_operations proc_entry_modtimer = {
    .read = read_timer,
    //.write = modlist_write,    
};

//operaciones de entrada salida
static const struct file_operations proc_entry_modconfig = {
    .write = write_config,
    .read = read_config,
};


int init_timer_module( void )
{
   
    /* Inicialización del buffer */  
    cbuffer = create_cbuffer_t(MAX_ITEMS_CBUF);

  if (!cbuffer) {
    return -ENOMEM;
  }
    /* Create timer */
    init_timer(&my_timer);
    /* Initialize field */
    my_timer.data=0;
    my_timer.function=fire_timer;
    my_timer.expires=jiffies + HZ;  /* Activate it one second from now */
    /* Activate the timer for the first time */
    add_timer(&my_timer); 
    /* Initialize work structure (with function) */
    INIT_WORK(&my_work, copy_items_into_list );

    int ret = 0;
    INIT_LIST_HEAD(&modlist); /* Initialize the list */
    proc_entry = proc_create( "modconfig", 0666, NULL, &proc_entry_modconfig);
    if (proc_entry == NULL) 
    {
      printk("\nse ha generado el modulo\n");
        ret = -ENOMEM;
    } 

     proc_entry = proc_create( "modtimer", 0666, NULL, &proc_entry_modtimer);
    if (proc_entry == NULL) 
    {
      printk("\nse ha generado el modulo\n");
        ret = -ENOMEM;
    }   
    return ret;
  
}


void cleanup_timer_module( void ){
  
  del_timer_sync(&my_timer);
  /* Wait until all jobs scheduled so far have finished */
  flush_scheduled_work();
  destroy_cbuffer_t(cbuffer);
  remove_proc_entry("modconfig", NULL);
  /* Wait until completion of the timer function (if it's currently running) and delete timer */
  
  //print_list(&modlist);
 // limpiar(&modlist);
}

module_init( init_timer_module );
module_exit( cleanup_timer_module );


MODULE_DESCRIPTION("timermod Module");

