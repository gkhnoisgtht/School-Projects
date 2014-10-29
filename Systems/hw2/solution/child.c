#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<string.h>


int		shouldRun	= 1;

int		childNum;

pid_t		pinataPid;


void	stop	(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  printf("Child %d stopping\n",childNum);
  fflush(stdout);
  shouldRun	= 0;
}


void	toldItsYourTurn_or_toldOfLoss
		(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{

  if  (info->si_pid == getppid())
  {
    printf("Child %d: I'm going to whack at it!\n",childNum);
    kill(pinataPid,SIGUSR1);
  }
  else
  {
    switch (rand() % 4)
    {
    case 0 :
      printf("Child %d: Damn, I missed the pinata completely!\n",childNum);
      break;
    case 1 :
      printf("Child %d: Hey I hit it!  What's that thing made of?  Kevlar?\n",childNum);
      break;
    case 2 :
      printf("Child %d: A swing and a miss!\n",childNum);
      break;
    case 3 :
      printf("Child %d: Curses!  I think I just weakened it for the next child!\n",childNum);
      break;
    }
    kill(getppid(),SIGUSR1);
  }

  fflush(stdout);
}


void	win	(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  printf("Oh yeah!  All that candy is MINE baby!\n");
  fflush(stdout);
  kill(getppid(),SIGUSR2);
}


int	main	(int	argc,
		 char*	argv[]
		)
{
  //  I.  Application validity check:
  if  (argc < 3)
  {
    fprintf(stderr,"Usage: %s <pinataPid> <childNum>\n",
    	    pinataPid
	   );
    exit(EXIT_FAILURE);
  }

  //  II.  Set up:
  //  II.A.  Get 'pinataPid':
  pinataPid	= strtol(argv[1],NULL,10);
  childNum	= strtol(argv[2],NULL,10);

  //  II.B.  Install signal handlers:
  struct sigaction sa;

  memset ( &sa, '\0', sizeof ( struct sigaction ) );
  sa.sa_flags |= SA_SIGINFO;
  sigemptyset ( &sa.sa_mask );

  sa.sa_sigaction = stop;
  sigaction(SIGINT, &sa, NULL);

  sa.sa_sigaction = toldItsYourTurn_or_toldOfLoss;
  sigaction(SIGUSR1, &sa, NULL);

  sa.sa_sigaction = win;
  sigaction(SIGUSR2, &sa, NULL);

  //  III.  Wait to be told what to do:
  while  (shouldRun)
    sleep(1);

  //  IV.  Finished:
  return(EXIT_SUCCESS);
}

