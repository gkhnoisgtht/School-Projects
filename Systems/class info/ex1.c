#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>

int	dataVar	= 2;
int	bssVar	= 0;

int	main	()
{
  int	stackVar= 5;
  int*	heapPtr	= (int*)malloc(sizeof(int));

  *heapPtr = 1;
  pid_t	child = fork();

  if  (child == 0)
  {
    dataVar++;
    bssVar++;
    stackVar++;
    (*heapPtr)++;
  }
  else
  {
    dataVar--;
    bssVar--;
    stackVar--;
    (*heapPtr)--;
  }

  printf("Process %d stack: %d\theap: %d\t data: %d\tbss: %d\n",
	 getpid(),stackVar,*heapPtr,dataVar,bssVar);
  return(EXIT_SUCCESS);
}
