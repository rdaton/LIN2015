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


#define MAX_ITEMS_CBUF  10
DEFINE_RWLOCK(sp);


MODULE_LICENSE("GPL");

struct timer_list my_timer; /* Structure that describes the kernel timer */
cbuffer_t* cbuffer; /* Buffer circular */ 
struct work_struct my_work;/* Work descriptor */
static struct proc_dir_entry *proc_entry ;

/* Tipo de nodo */
typedef struct{
    struct list_head list;
    char* data;
} tNodo;

/* Lista enlazada */
struct list_head modlist;


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
        if (item->data!=NULL){
            vfree (item->data);
            vfree(item); 
        }
    }
}

void print_list(struct list_head *list) {
        tNodo* item=NULL;
  struct list_head* cur_node=NULL;
  //trace_printk(KERN_INFO "%s\n","imprimiendo");
  
  //sección critica lista de enteros
  //read_lock(&rwl);
  list_for_each(cur_node, list) 
  {
  // item points to the structure wherein the links are embedded 
    item = list_entry(cur_node,tNodo, list);
  //trace_printk(KERN_INFO "%i\n",item->data);
  }
  //read_unlock(&rwl);
  //fin sección critica lista de enteros
  
}

static int add(char* valor) 
{
  tNodo* unNodo=(tNodo*)(vmalloc(sizeof (tNodo)));
  if (unNodo==NULL){
    if (valor!=NULL)
        vfree(valor);
        vfree(unNodo);
     return -ENOMEM;    
  }
    
  unNodo->data = valor;
  list_add_tail(&(unNodo->list), &modlist);
  printk("\nse ha metido el nuevo dato\n");
  print_list(&modlist);
  return 0;
}



/* Work's handler function */
static void copy_items_into_list ( struct work_struct *work )
{
    int num_temp=size_cbuffer_t (cbuffer);
    char temp[num_temp];
    int i= 0;
    unsigned long flags = 0;
    int r;
      read_lock_irqsave(&sp,flags);
    /* Obtener el primer elemento del buffer y eliminarlo */
    while(!is_empty_cbuffer_t ( cbuffer ) && i < num_temp){
        temp[i] =remove_cbuffer_t (cbuffer); 
        i++;
    }
     read_unlock_irqrestore(&sp,flags);

    i=0;
    while(i < num_temp){
        //add(&(temp[i]));
     // if(sscanf(temp[i],"%i\n",&r)==1){
       // add(r);
         printk("\nvalor metido es %i\n",r);
          i++;
      }
     
      printk(KERN_INFO "\ncopiar elementos a la lista\n");
      print_list(&modlist);

   
}

int timer_period_ms=1;
int emergency_threshold=80;//por centaje 80%
int max_random= 100;

/* Function invoked when timer expires (fires) */
static void fire_timer(unsigned long data)
{   
    float capacidad=MAX_ITEMS_CBUF;
    int numero_cpu =  smp_processor_id();
    unsigned int num_aleatorio=5;
    unsigned long flags = 0;
   char* ini;
    char* a=ini;
    //int len=0;

    /*num_aleatorio = (unsigned int)(get_random_int());
 
    while (num_aleatorio > max_random-1){
        num_aleatorio = get_random_int();
    }*/

    
     //a+=sprintf(a,"%i\n",num_aleatorio);

     //len= a-ini;
     printk("\nvalor de len es %i\n",a);
     int num_elem=size_cbuffer_t (cbuffer);
     int porcentaje = (((float)num_elem)/capacidad)*100;
           /* Insertar en el buffer */
    // printk("\nnumero de cpu es %i\n",numero_cpu);
     
     if(porcentaje < emergency_threshold){
        printk("\nporcentaje es %i\n",porcentaje);

//entra sesion critica
         write_lock_irqsave(&sp,flags);

        // insert_items_cbuffer_t(cbuffer,a,len);
          

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
      
     
    //printk("valor de numero es %i \n",num_aleatorio);
        
        /* Re-activate the timer one second from now */
    mod_timer( &(my_timer), jiffies + HZ); 
}

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
  /*  proc_entry = proc_create( "modlist", 0666, NULL, &proc_entry_fops);
    if (proc_entry == NULL) 
    {
        ret = -ENOMEM;
    }   
    return ret;
    */
}


void cleanup_timer_module( void ){
  
  del_timer_sync(&my_timer);
  /* Wait until all jobs scheduled so far have finished */
  flush_scheduled_work();
  destroy_cbuffer_t(cbuffer);
  /* Wait until completion of the timer function (if it's currently running) and delete timer */
  
  print_list(&modlist);
  limpiar(&modlist);
}

module_init( init_timer_module );
module_exit( cleanup_timer_module );


MODULE_DESCRIPTION("timermod Module");

