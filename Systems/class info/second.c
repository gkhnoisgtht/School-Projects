#include "123header.h"

#define	SIZE	10

void	enterBeginEnd	()
{
  char	carray[SIZE];
  int	isLegal	= 0;

  while  ( !isLegal )
  {
    printf("Please enter a character: ");
    fgets(carray,SIZE,stdin);
    begin = carray[0];

    printf("Please enter another character: ");
    fgets(carray,SIZE,stdin);
    end = carray[0];

    isLegal  = (end >= begin);
  }

}
    
