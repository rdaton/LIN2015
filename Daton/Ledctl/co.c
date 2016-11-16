#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>  



int main(int arg, char *argv[]) {
	int r=0;
	char* buff=argv[1];

	if(sscanf(buff,"%i",&r)==1){
		printf("exito por tener metido el valor %i\n",r);
		return 0;
	}
		printf("error\n");
	

  return 1;
}
