#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdio.h>  

// This function swaps bit at positions p1 and p2 in an integer n
//http://quiz.geeksforgeeks.org/how-to-swap-two-bits-in-a-given-integer/
int swapBits(unsigned int n, unsigned int p1, unsigned int p2)
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
}

int main(int arg, char *argv[]) {
	int r=0;
	char* buff=argv[1];

	if(sscanf(buff,"%i",&r)==1){
		printf("exito por tener metido el valor %i\n",r);
		int s = swapBits(r, 1, 2);
		
		printf("numero cambiado %i\n",s);
		return 0;
	}
		printf("error\n");
	

  return 1;
}
