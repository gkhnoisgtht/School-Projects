#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<string.h>

int	shouldRun	= 1;

void	stop	(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  printf("Pinata stopping\n");
  fflush(stdout);
  shouldRun	= 0;
}


void	attemptToHitPinata
		(int		sig,
		 siginfo_t*	info,
		 void*		data
		)
{
  int	isBroken	= (rand() % 20) == 19;
  int	signalToSend	= (isBroken ? SIGUSR2 : SIGUSR1);

  kill(info->si_pid,signalToSend);
}



int	main	(int	argc,
		 char*	argv[]
		)
{
  //  I.  Application validity check:
  if  (argc < 2)
  {
    fprintf(stderr,"Usage: %s <randNumSeed>\n",
	    argv[0]
	   );
    exit(EXIT_FAILURE);
  }

  //  II.  Prepare for operations:
  //  II.A.  Prepare random number generator:
  int	seed	= strtol(argv[1],NULL,10);

  srand(seed);

  //  II.B.  Prepare signal handlers:
  struct sigaction sa;

  memset ( &sa, '\0', sizeof ( struct sigaction ) );
  sa.sa_flags |= SA_SIGINFO;
  sigemptyset ( &sa.sa_mask );

  sa.sa_sigaction = stop;
  sigaction(SIGINT, &sa, NULL);

  sa.sa_sigaction = attemptToHitPinata;
  sigaction(SIGUSR1, &sa, NULL);

  //  III.  Wait for someone to attempt to hit pinata:
  while  (shouldRun)
    sleep(1);

  //  IV.  Finished:
  return(EXIT_SUCCESS);
}
