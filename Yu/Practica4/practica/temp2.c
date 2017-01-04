#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/random.h>
#include "cbuffer.h"


#define MAX_ITEMS_CBUF  5
#define MAX_CHARS_KBUF  10

MODULE_LICENSE("GPL");

struct timer_list my_timer; /* Structure that describes the kernel timer */
cbuffer_t* cbuffer; /* Buffer circular */ 


/* Function invoked when timer expires (fires) */
static void fire_timer(unsigned long data)
{   
    unsigned int max_random= 100;
    int numero_cpu =  smp_processor_id();
    unsigned int num_aleatorio=0;
    num_aleatorio = (unsigned int)(get_random_int());
 
   // while (num_aleatorio > max_random-1 || num_aleatorio < 0){
    while (num_aleatorio > max_random-1){
        num_aleatorio = get_random_int();
    }

           /* Insertar en el buffer */
     // insert_cbuffer_t(cbuffer,&num_aleatorio); 
    printk("valor de numero es %i \n",num_aleatorio);
        
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
    return 0;
}


void cleanup_timer_module( void ){
    destroy_cbuffer_t(cbuffer);
  /* Wait until completion of the timer function (if it's currently running) and delete timer */
  del_timer_sync(&my_timer);
}

module_init( init_timer_module );
module_exit( cleanup_timer_module );


MODULE_DESCRIPTION("timermod Module");

