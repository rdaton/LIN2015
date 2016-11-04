#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>


#define
__NR_LEDS 316

long led(void) 
{
  return(long) syscall(__NR_LEDS);
}

int main(int arg, char *argv[]) {
	int r=0;
	char* buff=argv[1];

	if(sscanf(buff,"%i",&r)==1){
		led(r);
		return 0;
	}
		printf("error\n");
	

  return 1;