#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <iostream>  

using namespace std;

int main(int arg, char *argv[]) 
{
	int r=0;
	if(sscanf(argv,"%i",&r)==1){
		cout<<"error de argumeno"<<endl;
		return 1;
	}

	cout<<"error de argumeno"<<endl;

  return 0;
}
