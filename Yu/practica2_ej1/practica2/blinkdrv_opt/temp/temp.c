#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>




int main() {
	//unsigned int sample_colors[]={0x000011, 0x110000, 0x001100, 0x000000};
	int r=0;
	int r2=0;
	char* buff="11:111100";

	printf("buff es %s\n",buff);
	unsigned int color;
	color=111111;

	printf("valor color >> 2 es %i \n",color >> 2);
	printf("valor color << 2 es %i \n",color >> 2);
	printf("valor color >> 2 es %i \n",color >> 2);
	printf("valor color >> 2 es %i \n",color >> 2);

	if(sscanf(buff,"%i:%i",&r,&r2)==2){
		/*printf("r1 es %i, r2 es %i\n",r,r2 );
		printf("hola valor 1 es %i\n",((color>>2) & r2));
		printf("valor 2 es %i\n",((color>>8) & r2));
		printf("valor 3 es %i\n",(color & r2));	
		*/
		return 0;
	}
		printf("error\n");
	

  return 1;
}
/*
printk("hola valor 1 es %i\n",((color>>16) & c));
		printk("valor 2 es %i\n",((color>>8) & c));
		printk("valor 3 es %i\n",(color & c));	
*/