/*-------------------------------------------------------------------------*
 *---									---*
 *---		main.cpp						---*
 *---									---*
 *---	    This file defines the high-level routines used in the	---*
 *---	massTransit program.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include "headers.h"

/*
 *	Compile and link with:
g++ -c main.cpp
g++ -c TrainLocation.cpp
g++ -c Station.cpp
g++ -c Track.cpp
g++ -o massTransit main.o TrainLocation.o Station.o Track.o -lpthread -lncurses
 *
 */

//  PURPOSE:  To be the function that a pthread instance runs to simulate
//	the train '*(Train*)vPtr'.  Return 'vPtr'.
void* simulateTrain	(void* vPtr)
{
  //  I.  Applicability validity check:
  if (vPtr == NULL)
    return(NULL);

  //  II.  Simulate Train:
  //  II.A.  Get ptr to 'Train' instance:
  Train* trainPtr = (Train*)vPtr;

  //  II.B.  Continue to simulate until MassTransit system signals to stop:
  while ( trainPtr->getMassTransit().getShouldContinue() )
  {
    //  II.B.1.  Pause:
    usleep((rand() % 1000) * 10000);

    //  II.B.2.  Quit if shouldn't continue:
    if  ( !trainPtr->getMassTransit().getShouldContinue() )
      break;

    //  II.B.3.  Attempt to leave current location to arrive at next:
    if  ( trainPtr->getLocPtr()->canLeave(trainPtr) )
    {
      TrainLocation* currentPtr = trainPtr->getLocPtr();
      TrainLocation* nextPtr = currentPtr->nextLocPtr(trainPtr);

      currentPtr->leave(trainPtr);
      nextPtr->arrive(trainPtr);
      trainPtr->getMassTransit().update();
    }

  }

  if  (trainPtr->getLocPtr() != NULL)
    trainPtr->getLocPtr()->leave(trainPtr);

  //  III.  Finished:
  return((void*)trainPtr);
}


//  PURPOSE:  To run the Mass Transit simulator with random number 'argv[1]'
//	assuming 'argc'.  Returns 'EXIT_SUCCESS' to OS.
int	main (int argc, const char*	argv[])
{
  //  I.  Application validity check:

  //  II.  Do simulation:
  //  II.A.  Reset random number generator if given seed on cmd line:
  if  (argc > 1)
    srand(atoi(argv[1]));

  //  II.B.  Create MassTransit simulator:
  MassTransit cta;

  //  II.C.  Turn on ncurses:
  initscr();

  //  II.D.  Do simulation:
  try
  {
    cta.simulate(60);
  }
  catch (const char* cPtr)
  {
    move(20,10);	addstr(cPtr);
    sleep(10);
  }

  //  II.E.  Turn off ncurses:
  sleep(5);
  endwin();

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
