#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include<stdio.h>
#ifdef__i386__
#define
__NR_HELLO 353
#else
#define
__NR_HELLO 316
#endif
long lin_hello(void) 
{
  return(long) syscall(__NR_HELLO);
}
int main() 
{
  return lin_hello();
}