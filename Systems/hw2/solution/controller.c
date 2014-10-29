/*--------------------------------------------------------------------------*
 *----									----*
 *----		controller.c						----*
 *----									----*
 *----	    This program controls version 2.0 of the pinata-whacking	----*
 *----	simulator.							----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 2.0		2014 January 27		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

/*----*
 *----*		Common include sequence:
 *----*/
#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<string.h>
#include	<unistd.h>
#include	<wait.h>


/*----*
 *----*		Declaration of constants:
 *----*/
#define		LINE_LEN		16
#define		PINATA_PROG_NAME	"pinata"
#define		CHILD_PROG_NAME		"child"


/*----*
 *----*		Definition of global vars:
 *----*/
int		shouldRun		= 1;
int		isWaitingForTurn;
pid_t*		childPidArray;
pid_t		pinataPid;


/*----*
 *----*		Definition of global fncs:
 *----*/

/*  PURPOSE:  To change the global state so that the program knows both that
 *	the current child process has finished its turn, and that it the game
 *	is over (that child won).  Ignores parameters.  No return value.
 */
void	turnOverStopGame
		(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  shouldRun		= 0;
  isWaitingForTurn	= 0;
}


/*  PURPOSE:  To change the global state so that the program knows that the
 *	current child process has finished its turn, but that it the game is
 *	not yet over (that child lost).  Ignores parameters.  No return value.
 */
void	turnOver	(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  isWaitingForTurn	= 0;
}


/*  PURPOSE:  To reap all child processes that have already finished.  Ignores
 *	parameters.  No return value.
 */
void	child	(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  int	status;
  pid_t	finishedId;

  while  ( (finishedId = waitpid(-1,&status,WNOHANG)) > 0)
    if  (WIFEXITED(status) )
      printf("%d has finished.\n",finishedId);
    else
      printf("%d has crashed.\n",finishedId);
}



/*  PURPOSE:  To simulate the pinata-whacking game.  Ignores command line
 *	parameters.  Returns EXIT_SUCCESS to OS on success or EXIT_FAILURE
 *	otherwise.
 */
int	main	()
{
  //  I.  Application validity check:

  //  II.  Do simulation:
  //  II.A.  Get simulation parameters:
  int		numChildren;
  int		randomNumberSeed;
  char		line[LINE_LEN];

  //  II.A.1.  Get 'numChildren':
  do
  {
    printf("Number of children? ");
    fgets(line,LINE_LEN,stdin);
    numChildren = strtol(line,NULL,10);
  }
  while  (numChildren < 1);


  //  II.A.2.  Get 'randomNumberSeed':
  do
  {
    printf("Random number seed? ");
    fgets(line,LINE_LEN,stdin);
    randomNumberSeed = strtol(line,NULL,10);
  }
  while  ( (randomNumberSeed < 0) || (randomNumberSeed > 32767) );


  //  II.B.  Prepare game:
  //  II.B.1.  Initialize 'childPidArray':
  childPidArray	      = (pid_t*)calloc(numChildren,sizeof(pid_t));

  //  II.B.2.  Install signal handlers:
  struct sigaction sa;

  memset ( &sa, '\0', sizeof ( struct sigaction ) );
  sa.sa_flags |= SA_SIGINFO;
  sigemptyset ( &sa.sa_mask );

  sa.sa_sigaction = child;
  sigaction(SIGCHLD, &sa, NULL);

  sa.sa_sigaction = turnOver;
  sigaction(SIGUSR1, &sa, NULL);

  sa.sa_sigaction = turnOverStopGame;
  sigaction(SIGUSR2, &sa, NULL);

  //  II.C.  Launch child processes:
  //  II.C.1.  Launch pinata process:
  pinataPid = fork();

  if  (pinataPid == -1)
  {
    fprintf(stderr,"Your OS is being fork()-bombed! :(\n");
    exit(EXIT_FAILURE);
  }

  if  (pinataPid == 0)
  {
    snprintf(line,LINE_LEN,"%d",randomNumberSeed);
    execl(PINATA_PROG_NAME,PINATA_PROG_NAME,line,NULL);
    fprintf(stderr,"Could not find program %s! :(\n",PINATA_PROG_NAME);
    exit(EXIT_FAILURE);
  }

  //  II.C.2.  Launch pinata-whacking child process(es):
  int	i;

  for  (i = 0;  i < numChildren;  i++)
  {
    childPidArray[i] = fork();

    if  (childPidArray[i] == -1)
    {
      fprintf(stderr,"Your OS is being fork()-bombed! :(\n");
      exit(EXIT_FAILURE);
    }

    if  (childPidArray[i] == 0)
    {
      char	numText[LINE_LEN];

      snprintf(line,LINE_LEN,"%d",pinataPid);
      snprintf(numText,LINE_LEN,"%d",i);
      execl(CHILD_PROG_NAME,CHILD_PROG_NAME,line,numText,NULL);
      fprintf(stderr,"Could not find program %s! :(\n",CHILD_PROG_NAME);
      exit(EXIT_FAILURE);
    }

  }

  //  II.D.  Play game:
  //  II.D.1.  Wait a sec' for all child processes to compose themselves:
  sleep(1);

  //  II.D.2.  Each iteration tells does one turn of one pinata-whacking
  //  	       child process:
  int	currentChild	= 0;

  while  (1)
  {
    printf("Child %d's turn:\n",currentChild);
    isWaitingForTurn = 1;
    kill(childPidArray[currentChild],SIGUSR1);

    while  (isWaitingForTurn)
      sleep(3);

    if  ( !shouldRun )
      break;

    currentChild++;

    if  (currentChild >= numChildren)
      currentChild = 0;
  }

  printf("Child %d won!\n",currentChild);

  //  II.E.  Clean up after game:
  //  II.E.1.  Tell all processes to end themselves:
  for  (currentChild = 0;  currentChild < numChildren;  currentChild++)
  {
    kill(childPidArray[currentChild],SIGINT);
    sleep(1);
  }

  kill(pinataPid,SIGINT);
  sleep(1);

  //  II.E.2.  Clean up memory:
  free(childPidArray);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
