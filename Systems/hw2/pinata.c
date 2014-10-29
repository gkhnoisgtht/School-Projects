#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0

bool shouldRun = true;
int controllerPid = 0;


void exit_handler(int sig, siginfo_t *info, void *context)
{
    printf("Pinata stopping\n");
    fflush(stdout);
    shouldRun = false;
}

void child_handler(int sig, siginfo_t *info, void *context)
{
    int val             = rand() % 20;
    int	isBroken	    = val == 19;
    int	signalToSend	= (isBroken ? SIGUSR2 : SIGUSR1);
    int pid             = info->si_pid;

    fflush(stdout);

    kill(pid, signalToSend);
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stdout,"Usage: %s <randNumSeed>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;

    char *ptr;
    //Random seed
    srand(strtol(argv[1], &ptr,10));
    controllerPid = getppid();

        struct sigaction sa_child, sa_exit, sa_stop;
    memset(&sa_child, 0, sizeof(struct sigaction));
    memset(&sa_exit, 0, sizeof(struct sigaction));

    sa_child.sa_handler = child_handler;
    sa_exit.sa_handler = exit_handler;

    sigaction(SIGUSR1, &sa_exit, 0);
    sigaction(SIGUSR2, &sa_child, 0);

    sa_child.sa_flags = SA_SIGINFO;
    sa_exit.sa_flags = SA_SIGINFO;


    while (shouldRun)
    {
        sleep(1);
    }
    return 0;
}

