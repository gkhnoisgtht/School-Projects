#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<wait.h>

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

  int status;

  wait(&status);

  if  ( WIFEXITED(status) )
    printf("Child ended properly and returned %d\n",WEXITSTATUS(status));
  else
    printf("Child crashed\n");

  printf("Thanks Junior!\n");
  return(EXIT_SUCCESS);
}
