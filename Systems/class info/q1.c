/* q1.c
 */
#include <stdlib.h>
#include <stdio.h>

#define	 unsigned int		uint

#define	 LENGTH			((uint) 1024*64)

int	 initializeArray	(uint		len,
	 			 int*		intArray
	 			)
{
  uint	i;

  for  (i = 0;  i < len;  i++)
    intArray[i] = (rand() % 64);
}



uint	countAdjacent	(int		index,
			 int*		intArray,
			 int		direction
			)
{
  uint	i;
  uint	sum	= 0;

  for  (i = 0;  i < index;  i++)
    if  ( intArray[i] == (intArray[i+1] + direction) )
      sum++;

  return(sum);
}



uint	funkyFunction		(uint		len,
				 int*		intArray
				)
{
  uint	i;
  uint	sum	= 0;

  for  (i = 0;  i < len-1;  i++)
    if  ( (i % 16) == 0x4 )
      sum += 4*countAdjacent(len-2,intArray,+1);
    else
      sum += 5*countAdjacent(len-2,intArray,-1);

  return(sum);
}


int	main	()
{
  int*		intArray	= (int*)calloc(LENGTH,sizeof(int));

  initializeArray(LENGTH,intArray);
  printf("funkyFunction() == %d\n",funkyFunction(LENGTH,intArray));
  free(intArray);
  return(EXIT_SUCCESS);
}
