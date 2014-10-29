/*-------------------------------------------------------------------------*
 *---									---*
 *---		Station.cpp						---*
 *---									---*
 *---	    This file defines a class that represents a Station:	---*
 *---	where several Train instances may be without incident. 		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include "headers.h"



//  PURPOSE:  To release resources.  No parameters.  No return value.
Station::~Station() throw()
{
    //  I.  Application validity check:
    //  II.  Release resources:
    //  III.  Finished:
}


//  PURPOSE:  To return 'true' if '*trainPtr' can leave '*this'
//	TrainLocation, or 'false' otherwise.
bool Station::canLeave(Train* trainPtr) const throw()
{
    //  I.  Application validity check:

    //  II.  Return value:
    return( isFirstTrain(trainPtr) );
}


//  PURPOSE:  To return a pointer to the next TrainLocation for '*trainPtr'
//  	after it leaves '*this'.
TrainLocation* Station::nextLocPtr (Train* trainPtr)
{
    //  I.  Application validity check:

    //  II.  Return track:
    return(trackArray[trainPtr->getLine()][trainPtr->getDirection()]);
}


//  PURPOSE:  To make '*trainPtr' arrive at '*this'.  No return value.
//  YOUR CODE SOMEWHERE IN HERE TO LOCK AND UNLOCK 'getLockPtr()'
//  WHAT NEEDS TO BE PROTECTED?
void Station::arrive(Train* trainPtr) throw()
{
    //  I.  Application validity check:

    //  II.  Switch '*trainPtr' direction if it cannot go any further in its
    //  	   current direction:
    pthread_mutex_lock(TrainLocation::getLockPtr());

    if  (getTrackPtr(trainPtr->getLine(),trainPtr->getDirection()) == NULL)
        trainPtr->switchDiretion();

    enqueue(trainPtr);
    trainPtr->setLocPtr(this);

    pthread_mutex_unlock(TrainLocation::getLockPtr());
}


//  PURPOSE:  To make '*trainPtr' leave '*this'.  No return value.
//  YOUR CODE SOMEWHERE IN HERE TO LOCK AND UNLOCK 'getLockPtr()'
//  WHAT NEEDS TO BE PROTECTED?
void Station::leave(Train* trainPtr) throw()
{
    //  I.  Application validity check:

    //  II.  Make '*trainPtr' leave '*this':
    pthread_mutex_lock(TrainLocation::getLockPtr());

    dequeue(trainPtr);
    trainPtr->setLocPtr(NULL);

    pthread_mutex_unlock(TrainLocation::getLockPtr());


    //  III.  Finished:
}
