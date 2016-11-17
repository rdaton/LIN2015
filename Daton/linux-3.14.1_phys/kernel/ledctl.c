#include <linux/init.h>
#include <linux/tty.h>      /* For fg_console */
#include <linux/kd.h>       /* For KDSETLED */
#include <linux/vt_kern.h>
#include <linux/syscalls.h>/*For SYSCALL_DEFINEi()*/
#include <linux/kernel.h>
#define ALL_LEDS_ON 0x7
#define ALL_LEDS_OFF 0


struct tty_driver* kbd_driver= NULL;

// This function swaps bit at positions p1 and p2 in an integer n
//http://quiz.geeksforgeeks.org/how-to-swap-two-bits-in-a-given-integer/
unsigned int swapBits(unsigned int n, unsigned int p1, unsigned int p2)
{
    /* Move p1'th to rightmost side */
    unsigned int bit1 =  (n >> p1) & 1;
 
    /* Move p2'th to rightmost side */
    unsigned int bit2 =  (n >> p2) & 1;
 
    /* XOR the two bits */
    unsigned int x = (bit1 ^ bit2);
 
    /* Put the xor bit back to their original positions */
    x = (x << p1) | (x << p2);
 
    /* XOR 'x' with the original number so that the
       two sets are swapped */
    unsigned int result = n ^ x;
    
    return result;
}



/* Get driver handler */
struct tty_driver* get_kbd_driver_handler(void){
  printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
  return vc_cons[fg_console].d->port.tty->driver;
}

/* Set led state to that specified by mask */
int set_leds(struct tty_driver* handler, unsigned int mask){
  unsigned int newMask=swapBits(mask, 1, 2);
  return (handler->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,newMask);
}



SYSCALL_DEFINE1(ledctl,unsigned int,mask)
{
  kbd_driver= get_kbd_driver_handler();
  set_leds(kbd_driver,ALL_LEDS_OFF);
  int unReturn= set_leds(kbd_driver,mask);
  if (unReturn!=0)
    printk("Algo ha ido mal \n");
  return unReturn;
}




