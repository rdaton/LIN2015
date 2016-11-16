#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>


#define __NR_LEDS 317

long led(int l) 
{
  return(long) syscall(__NR_LEDS,l);
}

int main(int arg, char *argv[]) {
	int r;
	for(r=0;r<8;r++){
		led(r);
	}
		

  return 0;
  }