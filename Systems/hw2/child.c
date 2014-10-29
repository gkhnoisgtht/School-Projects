#include	<stdlib.h>
#include	<stdio.h>
#include    <string.h>
#include	<signal.h>
typedef int bool;
#define true 1
#define false 0

bool shouldRun = true;
int pinataPid = 0;
int childNum = 0;
int controllerPid = 0;


void exit_handler(int sig, siginfo_t* info, void*	data)
{
    printf("Child process stopping\n");
    fflush(stdout);
    shouldRun = false;
}

void sigusr1_handler(int sig, siginfo_t* info, void* data)
{
    int pid = info->si_pid;
    if (pid == pinataPid){
        //failed
        fprintf(stdout, "Now its my turn!\n");
        //fflush(stdout);
        kill(pinataPid, SIGUSR1);
    } else {
        //child's turn
        fprintf(stdout,"I was so close!\n");
        //fflush(stdout);
        kill(controllerPid, SIGUSR1);
    }
}

void sigusr2_handler(int sig, siginfo_t* info, void*	data){
    printf("I won!\n");
    fflush(stdout);
    kill(controllerPid, SIGUSR2);
}


int main(int argc, char **argv)
{

    if(argc != 3)
    {
        fprintf(stdout,"Usage: %s <pinataPid> <childNum>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ptr;
    pinataPid = strtol(argv[1], &ptr,10);
    childNum = strtol(argv[2], &ptr,10);
    controllerPid = getppid();

    struct sigaction sa_sigusr1, sa_sigusr2, sa_exit;

    memset(&sa_sigusr1, 0, sizeof(struct sigaction));
    memset(&sa_sigusr2, 0, sizeof(struct sigaction));
    memset(&sa_exit, 0, sizeof(struct sigaction));

    sa_sigusr1.sa_handler = sigusr1_handler;
    sa_sigusr2.sa_handler = sigusr2_handler;
    sa_exit.sa_handler    = exit_handler;

    sigaction(SIGCHLD, &sa_sigusr1, 0);
    sigaction(SIGUSR1, &sa_sigusr2, 0);
    sigaction(SIGUSR2, &sa_exit, 0);

    sa_sigusr1.sa_flags = SA_SIGINFO;
    sa_sigusr2.sa_flags = SA_SIGINFO;
    sa_exit.sa_flags = SA_SIGINFO;

    while (shouldRun){
        sleep(1);
    }
    return 0;
}
