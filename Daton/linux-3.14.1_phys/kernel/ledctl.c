#include <asm-generic/errno.h>
#include <linux/init.h>
#include <linux/tty.h>      /* For fg_console */
#include <linux/kd.h>       /* For KDSETLED */
#include <linux/vt_kern.h>
#include <linux/syscalls.h>/*For SYSCALL_DEFINEi()*/
#include <linux/kernel.h>
#include <asm-generic/errno.h>
#define ALL_LEDS_ON 0x7


struct tty_driver* kbd_driver= NULL;


/* Get driver handler */
struct tty_driver* get_kbd_driver_handler(void){
   printk(KERN_INFO "modleds: loading\n");
   printk(KERN_INFO "modleds: fgconsole is %x\n", fg_console);
   return vc_cons[fg_console].d->port.tty->driver;
}

/* Set led state to that specified by mask */
inline int set_leds(struct tty_driver* handler, unsigned int mask){
   printk (KERN_INFO "máscara 2 de leds es %i\n", mask);

    return (handler->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,mask);
}



SYSCALL_DEFINE1(ledctl,unsigned int,mask)
{
  kbd_driver= get_kbd_driver_handler();
   printk (KERN_INFO "máscara de leds es %i\n", mask);
   return set_leds(kbd_driver,ALL_LEDS_ON);
}




