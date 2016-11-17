#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>




int main() {
	int r=0;
	int r2=0;
	char* buff="12:2222";

	printf("buff es %s\n",buff);
	
	if(sscanf(buff,"%i:%i",&r,&r2)==1){
		printf("r1 es %i, r2 es %i\n",r,r2 );
		
		return 0;
	}
		printf("error\n");
	

  return 1;
}