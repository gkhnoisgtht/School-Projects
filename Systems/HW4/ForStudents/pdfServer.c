/*-------------------------------------------------------------------------*
 *---									---*
 *---		pdfServer.cpp						---*
 *---									---*
 *---	    This file defines a server that receives a joDoc word	---*
 *---	processor file over a socket, creates a PDF file of it, and	---*
 *---	sends that PDF file back over the socket to the client.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	---	---*
 *---									---*
 *---	Version 1.0		2012 May 25		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*
 *---									---*
 *---		pdfServer.c						---*
 *---									---*
 *---	    Revised to:							---*
 *---	(1) receive a text file over a socket, create a PDF file of it,	---*
 *---	    and to send that PDF file back to the client.		---*
 *---	(2) be implemented in C instead of C++.				---*
 *---	(3) use more modern library fncs (strtol() instead of atoi(),	---*
 *---	     sigaction() instead of signal(), etc.)			---*
 *---									---*
 *---	----	----	----	----	----	----	----	---	---*
 *---									---*
 *---	Version 2.0		2014 February 23	Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc pdfServer-ForStudents.c -o pdfServer -lncurses

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>		// For memset()
#include	<sys/types.h>
#include	<sys/stat.h>		// For stat()
#include	<unistd.h>
#include	<fcntl.h>		// For open()
#include	<sys/socket.h>		// For socket()
#include	<netinet/in.h>		// For INADDR_ANY
#include	<signal.h>		// For sigaction
#include	<wait.h>		// For WNOHANG
#include	<ncurses.h>		// For initscr(), addstr(), etc.

#include	"headers.h"

//  PURPOSE:  To acknowledge the ending of child processes so they don't
//	stay zombies.  Uses a while-loop to get one or more processes that
//	have ended.  Ignores 'sig'.  No return value.
void childHandler(int sig)
{
    //  I.  Application validity check:

    //  II.  'wait()' for finished child(ren):
    //  YOUR CODE HERE
    int status;
    pid_t pid;

    while((pid = waitpid(-1, &status, WNOHANG)) >= 0)
    {
        if (WIFEXITED(status) != 0)
            printf("Process %d succeeded.\n", pid);
        else if (WIFSIGNALED(status) != 0)
            printf("Process %d failed.\n", pid);
        else
            printf("Process %d crashed.\n", pid);

    }
    numChildren--;

    //  III.  Finished:
}


//  PURPOSE:  To install 'childHandler()' as the 'SIGCHLD' handler.  No
//	parameters.  No return value.
void installChildHandler()
{
    //  I.  Application validity check:

    //  II.  Install handler:
    //  YOUR CODE HERE
    struct sigaction sa;

    memset(&sa, '\0', sizeof( struct sigaction));
    sa.sa_flags |= SA_NOCLDSTOP | SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = childHandler;
    sigaction(SIGCHLD, &sa, NULL);


    //  III.  Finished:
}


//  PURPOSE:  To note that signal 'SIGCHLD' is to be ignored.  No parameters.
//	No return value.
void ignoreSigChld()
{
    //  I.  Application validity check:

    //  II.  Ignore SIGCHLD:
    //  YOUR CODE HERE
    struct sigaction sa;

    memset(&sa, '\0', sizeof( struct sigaction));
    sa.sa_flags |= SA_NOCLDSTOP | SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = SIG_IGN;
    sigaction(SIGCHLD, &sa, NULL);
    //  III.  Finished:
}


//  PURPOSE:  To return a port number to monopolize, either from the
//	'argv[1]' (if 'argc' > 1), or by asking the user (otherwise).
int	obtainPortNumber(int argc, char* argv[])
{
    //  I.  Application validity check:

    //  II.  Obtain port number:
    int portNum;

    if  (argc > 1)
    {
        //  II.A.  Attempt to obtain port number from 'argv[1]':
        portNum = strtol(argv[1],NULL,10);
    }
    else
    {
        //  II.B.  Attempt to obtain port number from user:
        char numText[NUM_TEXT_LEN];

        do
        {
            printf("Port number (%d-%d)? ",LO_LEGAL_PORT_NUM,HI_LEGAL_PORT_NUM);
            fgets(numText,NUM_TEXT_LEN,stdin);
            portNum = strtol(numText,NULL,10);
        }
        while ((portNum < LO_LEGAL_PORT_NUM) || (portNum > HI_LEGAL_PORT_NUM));

    }
    //  III.  Finished:
    return(portNum);
}


//  PURPOSE:  To return a socket for listening for clients if obtained one from
//	OS, or to send an appropriate error msg to 'stderr' and return -1
//	otherwise.  Sets '*portPtr' to the number of the port to use.
//	No parameters.
int createListeningSocket(int argc,char* argv[],int* portPtr)
{
    //  I.  Applicability validity check:
    //  II.  Create server socket:
    int socketDescriptor = ERROR_FD;
    *portPtr = obtainPortNumber(argc, argv);
    //  YOUR CODE HERE
    int i;
    while(socketDescriptor < 0)
    {
        socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    }

    struct sockaddr_in serv_addr;
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(*portPtr);

    if(bind(socketDescriptor, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        exit(EXIT_FAILURE);
    if(listen(socketDescriptor, MAX_NUM_QUEUING_CLIENTS) < 0)
        exit(EXIT_FAILURE);

    //  III.  Finished:
    return(socketDescriptor);
}


//  PURPOSE:  To attempt to obtain the filename and file size from the client
//	via 'clientDescriptor' and to put them into 'filename' and
//	'*filesizePtr' respectively.  'maxFilenameLen' tells the length of the
//	'filename' char array.  Returns '1' if successful or '0' otherwise.
int	didReadNameAndSize(int clientDescriptor, char* filename, int maxFilenameLen, int* filesizePtr)
{
    //  I.  Application validity check:

    //  II.  Read filename and file size:
    //  YOUR CODE HERE
    char fileNameBuffer[maxFilenameLen];
    bzero(fileNameBuffer,maxFilenameLen);
    if(read(clientDescriptor, fileNameBuffer, maxFilenameLen) < 0)
    {
        fprintf(stderr,"Could not read filename.\n");
        return(0);
    }
    sprintf(filename, fileNameBuffer, maxFilenameLen);

    char fileSizeBuffer[NUM_TEXT_LEN];
    bzero(fileSizeBuffer,NUM_TEXT_LEN);
    if(read(clientDescriptor, fileSizeBuffer, NUM_TEXT_LEN) < 0)
    {
        fprintf(stderr, "Could not read size of file %s.\n",filename);
        return(0);
    }

    int bytes = strtol(fileSizeBuffer,NULL,10);
    if(bytes < 0)
    {
        fprintf(stderr,"Illegal file size for file %s.\n",filename);
        return(0);
    }

    *filesizePtr = bytes;
    //  III.  Finished:
    return(1);
}

//  PURPOSE:  To do the work of handling the client with whom socket descriptor
//	'clientDescriptor' communicates.  Returns 'EXIT_SUCCESS' on success or
//	'EXIT_FAILURE' otherwise.
int	serveClient	(int clientDescriptor, const char* filename, int fileSize)
{
    //  I.  Application validity check:

    //  II.  Serve client:
    //  YOUR CODE HERE

    char fileBuffer[fileSize];
    if(read(clientDescriptor, fileBuffer, fileSize) < 0)
    {
        printf("Unable to read file: %s.\n",filename);
        exit(1);
    }
    int pid = getpid();


//-----------------------------------------------------------------------------------------------

    // generate prefix
    char *prefix = malloc(snprintf(NULL, 0, TEMP_FILENAME_PREFIX, pid) + 1);
    sprintf(prefix, TEMP_FILENAME_PREFIX, pid);

    // create file names with extentions
    char *tmpTxtFileName = malloc(snprintf(NULL, 0, "%s%s%s", prefix, filename, TEXT_EXT) + 1);
    sprintf(tmpTxtFileName, "%s%s%s", prefix, filename, TEXT_EXT);

    char *tmpPSFileName = malloc(snprintf(NULL, 0, "%s%s%s", prefix, filename, POSTSCRIPT_EXT) + 1);
    sprintf(tmpPSFileName, "%s%s%s", prefix, filename, POSTSCRIPT_EXT);

    char *tmpPDFFileName = malloc(snprintf(NULL, 0, "%s%s%s", prefix, filename, PDF_EXT) + 1);
    sprintf(tmpPDFFileName, "%s%s%s", prefix, filename, PDF_EXT);

//----------------------------------------------------------------------------------------------

    // write to files
    int ftxt = open(tmpTxtFileName, O_WRONLY|O_CREAT, 0666);
    write(ftxt, fileBuffer, fileSize);
    close(ftxt);
    // create ps file
    int fps = open(tmpPSFileName, O_CREAT, 0666);
    close(fps);
    // create pdf file
    int fpdf = open(tmpPDFFileName, O_CREAT,0666);
    close(fpdf);

//-----------------------------------------------------------------------------------------------

    // fork processes
    pid_t psPid;
    int status;
    psPid = fork();
    if  (psPid < 0)
        exit(1);
    else if  (psPid == 0)
    {
        // child converting file
        execl("/usr/bin/enscript","enscript","-B",tmpTxtFileName,"-p",tmpPSFileName,"-q",NULL);
    }
    else
    {
        // parent
        wait(&status);
    }


//------------------------------------------------------------------------------------------------

    pid_t pdfChild;
    // forking other child
    pdfChild = fork();
    if  (pdfChild < 0)
        exit(1);
    else if  (pdfChild == 0)
    {
        // child converting postscript file
        execl("/usr/bin/ps2pdf12","ps2pdf12",tmpPSFileName,tmpPDFFileName,NULL);
    }
    else
    {
        // parent
        wait(&status);
    }


//--------------------------------------------------------------------------------------------------

    char pdfSize[NUM_TEXT_LEN];
    int size = getFileSize(tmpPDFFileName);
    sprintf(pdfSize, "%d", size);
    if(write(clientDescriptor, pdfSize, NUM_TEXT_LEN) < 0)
    {
        fprintf(stderr, "Unable to send file size\n");
        exit(EXIT_FAILURE);
    }

    char pdfFileBuffer[size];
    int pdfFile = open(tmpPDFFileName, O_RDONLY, 0660);
    if(read(pdfFile, pdfFileBuffer, size) < 0)
    {
        fprintf(stderr, "Unable to read file: %s\n", tmpPDFFileName);
        exit(EXIT_FAILURE);
    }

    if(write(clientDescriptor, pdfFileBuffer, size) < 0)
    {
        fprintf(stderr, "Unable to send file: %s\n", pdfFileBuffer);
        exit(EXIT_FAILURE);
    }


//--------------------------------------------------------------------------------------------------

    deleteFile(tmpPDFFileName);
    deleteFile(tmpPSFileName);
    deleteFile(tmpTxtFileName);

    //  III.  Finished:
    return(EXIT_SUCCESS);
}

int getFileSize (const char * filename)
{
    struct stat sb;
    if (stat (filename, & sb) != 0)
    {
        fprintf(stderr, "Unable to get the file size\n");
        exit (EXIT_FAILURE);
    }
    return sb.st_size;
}


int deleteFile(const char* filename)
{
    if (remove(filename) != 0)
    {
        fprintf(stderr, "Unable to deleted file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
}

//  PURPOSE:  To start the graphical system and print the header telling
//	the user to connect to port 'port' via the text:
//		"pdfServer running on port %d.\n".
//	No return value.
void startGraphicsAndShowTitle(int port)
{
    //  I.  Application validity check:

    //  II.  Start ncurses and show title:
    char	text[LINE_LEN];
    snprintf(text,LINE_LEN,"pdfServer running on port %d.\n",port);
    printf(text);

    //  III.  Finished:
}


//  PURPOSE:  To do the work of the server: waiting for clients, fork()-ing
//	child processes to handle them, and going back to wait for more
//	clients.  No return value.
void doServer(int socketDescriptor)
{
    //  I.  Application validity check:

    //  II.  Serve clients:
    //  II.A.  Each iteration serves one client:
    char text[LINE_LEN];
    int i;

    for(i = 0;  i < NUM_SERVER_ITERS;  i++)
    {
        int		clientDescriptor = accept(socketDescriptor,NULL,NULL);
        char	filename[FILENAME_LEN];
        int		fileSize;
        pid_t	childPid;

        if  (clientDescriptor < 0)
            continue;

        if  (!didReadNameAndSize(clientDescriptor,filename,FILENAME_LEN,&fileSize))
            continue;

        childPid = fork();

        if  (childPid < 0)
            continue;
        else if  (childPid == 0)
        {
            numChildren++;
            ignoreSigChld();
            close(socketDescriptor);
            int status = serveClient(clientDescriptor,filename,fileSize);
            close(clientDescriptor);
            numChildren--;
            exit(status);
        }
        else
        {
            snprintf(text,LINE_LEN,"%d: %s (%d bytes) (process %d) ...\n",i,filename,fileSize,childPid);
            printf(text);	//  COMMENT THIS OUT
            close(clientDescriptor);
        }

    }

    //  II.B.  Wait for all children to finish:
    //  YOUR CODE HERE?
    while (numChildren > 0)
        sleep(1);
    //  III.  Finished:
}


//  PURPOSE:  To turn graphics off.  No parameters.  No return value.
void endGraphics()
{
    //  DO NOTHING
}


int main(int argc, char* argv[])
{
    //  I.  Application validity check:

    //  II.  Serve clients

    //  II.A.  Get socket file descriptor:
    int port;
    int socketDescriptor = createListeningSocket(argc,argv,&port);
    if  (socketDescriptor == -1)
        return(EXIT_FAILURE);

    //  II.B.  Set up SIGCHLD handler:
    installChildHandler();

    //  II.C.  Handle clients:
    startGraphicsAndShowTitle(port);
    doServer(socketDescriptor);

    //  III.  Will never get here:
    return(EXIT_SUCCESS);
}


