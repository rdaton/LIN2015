#include <stdio.h>
#include <unistd.h> /* close */
#include <fcntl.h> /* open */
#include <errno.h> /* perror */
#include <linux/kd.h> /* Keyboard macros */
#include <sys/ioctl.h> /* ioctl */

int main() {
int tty = open("/dev/console", 0), led;
unsigned long int arg;

if (tty<3) {
perror("open: ");
return -1;
}
if (ioctl(tty,KDGKBTYPE, &arg) > 0) perror("ioctl: ");
if (arg == KB_101) puts("You have a 101 key keyboard.");

for (led=1; led<9; led++) {
if (ioctl(tty,KDSETLED, led) > 0) perror("ioctl led on: ");
printf("LED %d on...hit enter", led);
getchar();
if (ioctl(tty,KDSETLED, led+0xff) > 0) perror("ioctl led off: ");
printf("off (hit enter)\n");
getchar();
}

close(tty);

return 0;
}
