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


/*----*
 *----*		Declaration of constants:
 *----*/
#define		LINE_LEN		    16
#define		PINATA_PROG_NAME	"pinata"
#define		CHILD_PROG_NAME		"child"


/*----*
 *----*		Definition of global vars:
 *----*/
int		    shouldRun = 1;
int	        isWaitingForTurn;
pid_t*		childPidArray;
pid_t		pinataPid;


/*----*
 *----*		Definition of global fncs:
 *----*/

/*  PURPOSE:  To change the global state so that the program knows both that
 *	the current child process has finished its turn, and that it the game
 *	is over (that child won).  Ignores parameters.  No return value.
 */
void turnOverStopGame(int sig, siginfo_t* info, void* data)
{
    fprintf(stdout, "Game Stopping\n");
    shouldRun		    = 0;
    isWaitingForTurn	= 0;
}


/*  PURPOSE:  To change the global state so that the program knows that the
 *	current child process has finished its turn, but that it the game is
 *	not yet over (that child lost).  Ignores parameters.  No return value.
 */
void turnOver(int sig, siginfo_t* info, void* data)
{
    fprintf(stdout, "child lost\n");
    isWaitingForTurn	= 0;
}


/*  PURPOSE:  To reap all child processes that have already finished.  Ignores
 *	parameters.  No return value.
 */
void child(int sig, siginfo_t* info, void*	data)
{
    fprintf(stdout, "reaping children\n");
    int	status;
    pid_t finishedId;// = info->si_pid;
    //status = kill(finishedId, SIGINT);
    while ((finishedId = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("Child %d terminated with status %d\n", finishedId, status);
    }
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
    char	line[LINE_LEN];

    //  II.A.1.  Get 'numChildren' (must be greater than or equal to 1):
    int recieved = 1;
    while (recieved)
    {
        printf("How many children are there? ");
        int items_read = scanf ("%d", &numChildren);
        if (items_read)
        {
            recieved = 0;
        }
        else
        {
            /**** Erroneous input, get rid of it and retry! */
            scanf ("%*[^\n]");
        }
    }


    //  II.A.2.  Get 'randomNumberSeed' (from 0-32767):
    recieved = 1;
    while (recieved)
    {
        printf("What random seed do you want to use? (0-32767) ");
        int items_read = scanf ("%d", &randomNumberSeed);
        if (items_read)
        {
            recieved = 0;
        }
        else
        {
            /**** Erroneous input, get rid of it and retry! */
            scanf ("%*[^\n]");
        }
    }


    //  II.B.  Prepare game:
    //  II.B.1.  Initialize 'childPidArray':
    childPidArray = (pid_t*)calloc(numChildren,sizeof(pid_t));

    //  II.B.2.  Install signal handlers:
    struct sigaction sa_child, sa_turn, sa_stop;
    memset(&sa_child, 0, sizeof(struct sigaction));
    memset(&sa_turn, 0, sizeof(struct sigaction));
    memset(&sa_stop, 0, sizeof(struct sigaction));

    sa_child.sa_handler = child;
    sa_turn.sa_handler = turnOver;
    sa_stop.sa_handler = turnOverStopGame;

    sigaction(SIGUSR1, &sa_turn, 0);
    sigaction(SIGUSR2, &sa_stop, 0);
    sigaction(SIGCHLD, &sa_child, 0);

    sa_child.sa_flags = SA_SIGINFO;
    sa_turn.sa_flags = SA_SIGINFO;
    sa_stop.sa_flags = SA_SIGINFO;


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
        // YOUR CODE HERE TO LAUNCH PINATA_PROG_NAME WITH 'line' AS THE 1ST ARGUMENT
        execl("./pinata", "pinata", line, (char*)0);
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
            execl("./child", "child", line, numText, (char*)0);
            fprintf(stderr,"Could not find program %s! :(\n",CHILD_PROG_NAME);
            exit(EXIT_FAILURE);
        }

    }

    //  II.D.  Play game:
    //  II.D.1.  Wait a sec' for all child processes to compose themselves:
    sleep(1);

    //  II.D.2.  Each iteration tells does one turn of one pinata-whacking
    //  	       child process:
    int	currentChild = 0;

    while  (1)
    {
        printf("Child %d's turn:\n",currentChild);
        isWaitingForTurn = 1;

        kill(childPidArray[currentChild],SIGUSR1);

        while  (isWaitingForTurn)
        {
            sleep(3);
        }

        if  ( !shouldRun )
        {
            break;
        }

        currentChild++;

        if  (currentChild >= numChildren)
        {
            currentChild = 0;
        }
    }

    printf("Child %d won!\n",currentChild);

    //  II.E.  Clean up after game:
    //  II.E.1.  Tell all processes to end themselves:
    for  (currentChild = 0;  currentChild < numChildren;  currentChild++)
    {
        printf("Killing child %d\n", currentChild);
        kill(childPidArray[currentChild],SIGINT);
        sleep(1);
    }

    printf("Killing pinata\n");
    kill(pinataPid,SIGINT);
    sleep(1);

    //  II.E.2.  Clean up memory:
    free(childPidArray);

    //  III.  Finished:
    return(EXIT_SUCCESS);
}
