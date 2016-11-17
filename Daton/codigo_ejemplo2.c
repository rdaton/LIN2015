struct timer_list my_timer;
struct tty_driver *my_driver;
char kbledstatus = 0;

#define BLINK_DELAY   HZ/5
#define CAP           0x01
#define NUM           0x02
#define NUM_CAP       0x03
#define SCROLL        0x04
#define NUM_SCROLL    0x05
#define CAP_SCROLL    0x06
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

static void my_timer_func(unsigned long ptr)
{
    int *pstatus = (int *)ptr;

    if (*pstatus == ALL_LEDS_ON)
        *pstatus = RESTORE_LEDS;

    else
       {
        *pstatus = ALL_LEDS_ON;
        *pstatus = CAP;
        *pstatus =NUM;
        *pstatus = NUM_CAP;
        *pstatus = SCROLL;
        *pstatus = NUM_SCROLL; 
        *pstatus = CAP_SCROLL;    
        }

    (my_driver->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED,
                *pstatus);

    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);
}

/*
  Keyboard LED Init Function
*/
static int __init kbleds_init(void)
{
    int i;

    printk(KERN_INFO "kbleds: loading\n");
    printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);

    for (i = 0; i < MAX_NR_CONSOLES; i++) 
    {
      if (!vc_cons[i].d)
        break;
      printk(KERN_INFO "console[%i/%i] #%i, tty %lx\n", i,
             MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
             (unsigned long)vc_cons[i].d->vc_tty);
    }

    printk(KERN_INFO "kbleds: finished scanning consoles\n");

    my_driver = vc_cons[fg_console].d->vc_tty->driver;

    printk(KERN_INFO "kbleds: tty driver magic %x\n", my_driver->magic);

    /*
     * Set up the LED blink timer the first time
     */
    init_timer(&my_timer);
    my_timer.function = my_timer_func;
    my_timer.data = (unsigned long)&kbledstatus;
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    return 0;
}
