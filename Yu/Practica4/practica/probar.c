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
DEFINE_SPINLOCK(sp);


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
    list_for_each(cur_node, list) 
    {
    /* item points to the structure wherein the links are embedded */
    item = list_entry(cur_node,tNodo, list);
    //trace_printk(KERN_INFO "%i\n",item->data);
    }
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

     spin_lock_irqsave(sp,2);
    /* Obtener el primer elemento del buffer y eliminarlo */
    while(!is_empty_cbuffer_t ( cbuffer ) && i < num_temp){
        temp[i] =remove_cbuffer_t (cbuffer); 
        i++;
    }

    spin_unlock_irqrestore(sp,1);

    i=0;
    while(i < num_temp){
        add(&(temp[i]));
        i++;
    }
     

    printk(KERN_INFO "\ncopiar elementos a la lista\n");
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
    /*num_aleatorio = (unsigned int)(get_random_int());
 
    while (num_aleatorio > max_random-1){
        num_aleatorio = get_random_int();
    }*/

     char* a = (char*)&num_aleatorio;
     int num_elem=size_cbuffer_t (cbuffer);
     int porcentaje = (((float)num_elem)/capacidad)*100;
           /* Insertar en el buffer */
     
     if(porcentaje < emergency_threshold){
        printk("\nporcentaje es %i\n",porcentaje);

        spin_lock_irqsave(sp,1);

         insert_items_cbuffer_t(cbuffer,a,1);
         printk("\nNo esta lleno\n");
        
     }else{

         spin_unlock_irqrestore(sp,2);

         printk("\nesta lleno\n");
         
         schedule_work(&my_work);
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
   destroy_cbuffer_t(cbuffer);
  /* Wait until completion of the timer function (if it's currently running) and delete timer */
  del_timer_sync(&my_timer);
  /* Wait until all jobs scheduled so far have finished */
  flush_scheduled_work();

  remove_proc_entry("modlist", NULL);
  limpiar(&modlist);
}

module_init( init_timer_module );
module_exit( cleanup_timer_module );


MODULE_DESCRIPTION("timermod Module");

