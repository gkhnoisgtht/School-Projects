#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>

#define		LINE_LEN	64

int	main	()
{
  char	line[LINE_LEN];

  printf("Waddya want!? ");
  fgets(line,LINE_LEN,stdin);
  char* cPtr = strchr(line,'\n');

  if  (cPtr != NULL)
    *cPtr = '\0';

  pid_t child = fork();

  if  (child == 0)
  {
    execl(line,line,NULL);
    fprintf(stderr,"I could not find %s\n",line);
    exit(EXIT_FAILURE);
  }

  printf("Junior, you handle it.\n");
  return(EXIT_SUCCESS);
}
