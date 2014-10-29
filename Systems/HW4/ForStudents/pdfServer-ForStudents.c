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
void	childHandler	(int	sig
			)
{
  //  I.  Application validity check:

  //  II.  'wait()' for finished child(ren):
  //  YOUR CODE HERE

  //  III.  Finished:
}


//  PURPOSE:  To install 'childHandler()' as the 'SIGCHLD' handler.  No
//	parameters.  No return value.
void	installChildHandler	()
{
  //  I.  Application validity check:

  //  II.  Install handler:
  //  YOUR CODE HERE

  //  III.  Finished:
}


//  PURPOSE:  To note that signal 'SIGCHLD' is to be ignored.  No parameters.
//	No return value.
void	ignoreSigChld	()
{
  //  I.  Application validity check:

  //  II.  Ignore SIGCHLD:
  //  YOUR CODE HERE

  //  III.  Finished:
}


//  PURPOSE:  To return a port number to monopolize, either from the
//	'argv[1]' (if 'argc' > 1), or by asking the user (otherwise).
int	obtainPortNumber	(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Obtain port number:
  int	portNum;

  if  (argc > 1)
  {
    //  II.A.  Attempt to obtain port number from 'argv[1]':
    portNum = strtol(argv[1],NULL,10);
  }
  else
  {
    //  II.B.  Attempt to obtain port number from user:
    char	numText[NUM_TEXT_LEN];

    do
    {
      printf("Port number (%d-%d)? ",LO_LEGAL_PORT_NUM,HI_LEGAL_PORT_NUM);
      fgets(numText,NUM_TEXT_LEN,stdin);
      portNum = strtol(numText,NULL,10);
    }
    while  ((portNum < LO_LEGAL_PORT_NUM) || (portNum > HI_LEGAL_PORT_NUM));

  }

  //  III.  Finished:
  return(portNum);
}


//  PURPOSE:  To return a socket for listening for clients if obtained one from
//	OS, or to send an appropriate error msg to 'stderr' and return -1
//	otherwise.  Sets '*portPtr' to the number of the port to use.
//	No parameters.
int 	createListeningSocket	(int	argc,
				 char*	argv[],
				 int*	portPtr
				)
{
  //  I.  Applicability validity check:

  //  II.  Create server socket:
  int socketDescriptor = ERROR_FD;
  //  YOUR CODE HERE

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To attempt to obtain the filename and file size from the client
//	via 'clientDescriptor' and to put them into 'filename' and
//	'*filesizePtr' respectively.  'maxFilenameLen' tells the length of the
//	'filename' char array.  Returns '1' if successful or '0' otherwise.
int	didReadNameAndSize
			(int	clientDescriptor,
			 char*	filename,
			 int	maxFilenameLen,
			 int*	filesizePtr
			)
{
  //  I.  Application validity check:

  //  II.  Read filename and file size:
  //  YOUR CODE HERE

  //  III.  Finished:
  return(1);
}


//  PURPOSE:  To do the work of handling the client with whom socket descriptor
//	'clientDescriptor' communicates.  Returns 'EXIT_SUCCESS' on success or
//	'EXIT_FAILURE' otherwise.
int	serveClient	(int		clientDescriptor,
			 const char*	filename,
			 int		fileSize
			)
{
  //  I.  Application validity check:

  //  II.  Serve client:
  //  YOUR CODE HERE

  //  III.  Finished:
  return(EXIT_SUCCESS);
}


//  PURPOSE:  To start the graphical system and print the header telling
//	the user to connect to port 'port' via the text:
//		"pdfServer running on port %d.\n".
//	No return value.
void	startGraphicsAndShowTitle   	(int 	port
					)
{
  //  I.  Application validity check:

  //  II.  Start ncurses and show title:
  char	text[LINE_LEN];

  //  YOUR CODE HERE?
  snprintf(text,LINE_LEN,"pdfServer running on port %d.\n",port);
  printf(text);
  //  YOUR CODE HERE?

  //  III.  Finished:
}


//  PURPOSE:  To do the work of the server: waiting for clients, fork()-ing
//	child processes to handle them, and going back to wait for more
//	clients.  No return value.
void	doServer	(int	socketDescriptor
			)
{
  //  I.  Application validity check:

  //  II.  Serve clients:
  //  II.A.  Each iteration serves one client:
  char	text[LINE_LEN];
  int	i;

  for  (i = 0;  i < NUM_SERVER_ITERS;  i++)
  {
    int		clientDescriptor = accept(socketDescriptor,NULL,NULL);
    char	filename[FILENAME_LEN];
    int		fileSize;
    pid_t	childPid;

    if  (clientDescriptor < 0)
      continue;

    if  (!didReadNameAndSize(clientDescriptor,filename,FILENAME_LEN,&fileSize))
      continue;

    childPid	= fork();

    if  (childPid < 0)
      continue;
    else
    if  (childPid == 0)
    {
      ignoreSigChld();
      close(socketDescriptor);
      int status = serveClient(clientDescriptor,filename,fileSize);
      close(clientDescriptor);
      exit(status);
    }
    else
    {
      snprintf(text,LINE_LEN,"%d: %s (%d bytes) (process %d) ...\n",
	       i,filename,fileSize,childPid
	      );
      printf(text);	//  COMMENT THIS OUT
      // YOUR CODE HERE TO print 'text' using ncurses
      refresh();
      close(clientDescriptor);
    }

  }

  //  II.B.  Wait for all children to finish:
  //  YOUR CODE HERE?

  //  III.  Finished:
}


//  PURPOSE:  To turn graphics off.  No parameters.  No return value.
void	endGraphics	()
{
  //  I.  Application validity check:

  //  II.  Turn graphics off:
  //  YOUR CODE HERE?


  //  III.  Finished:
}


int	main	(int	argc,
		 char*	argv[]
		)
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
  endGraphics();

  //  III.  Will never get here:
  return(EXIT_SUCCESS);
}
