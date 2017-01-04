#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/timer.h>
//#include "cbuffer.h"

MODULE_LICENSE("GPL");
#define MAX_ITEMS_CBUF  5
#define MAX_CHARS_KBUF  10


struct timer_list my_timer; /* Structure that describes the kernel timer */
//cbuffer_t* cbuffer; /* Buffer circular */ 


/* Function invoked when timer expires (fires) */
static void fire_timer(unsigned long data)
{
  static char flag=0;
        
        if (flag==0)
            printk(KERN_INFO "Tic\n");
        else
            printk(KERN_INFO "Tac\n");           
  
        flag=~flag;
        
        /* Re-activate the timer one second from now */
  mod_timer( &(my_timer), jiffies + HZ); 
}

int init_timer_module( void )
{

    /* Inicialización del buffer */  
  //  cbuffer = create_cbuffer_t(MAX_ITEMS_CBUF);

//  if (!cbuffer) {
  //  return -ENOMEM;
//  }
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
  /* Wait until completion of the timer function (if it's currently running) and delete timer */
  del_timer_sync(&my_timer);
  //  destroy_cbuffer_t(cbuffer);
}

module_init( init_timer_module );
module_exit( cleanup_timer_module );


