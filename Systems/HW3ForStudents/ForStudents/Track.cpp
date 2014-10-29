/*-------------------------------------------------------------------------*
 *---									---*
 *---		Track.cpp						---*
 *---									---*
 *---	    This file defines a class that represents a length of	---*
 *---	Track: where only 'MAX_ALLOWED_NUM_TRAINS_ON_TRACK' Train	---*
 *---	instances may be without incident. 				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"headers.h"



//  PURPOSE:  To return a pointer to the next TrainLocation for '*trainPtr'
//  	after it leaves '*this'.
TrainLocation* Track::nextLocPtr(Train* trainPtr )
{
    //  I.  Application validity check:

    //  II.  Return track:
    return(termini[trainPtr->getDirection()]);
}


//  PURPOSE:  To make '*trainPtr' arrive at '*this'.  No return value.
//  YOUR CODE SOMEWHERE IN HERE TO LOCK 'getLockPtr()' WAIT FOR SIGNAL
//  IF TRACK IS IN USE.
void Track::arrive (Train* trainPtr) throw()
{
    //  I.  Application validity check:

    //  II.  Make '*trainPtr' arrive at '*this':
    //  II.A.  Get lock on track:
    //  ????

    pthread_mutex_lock(TrainLocation::getLockPtr());

    if  ( !trainPtr->getMassTransit().getShouldContinue() )
    {
        //  YOUR CODE HERE TO UNLOCK
        pthread_mutex_unlock(TrainLocation::getLockPtr());
        //  YOUR CODE HERE TO SIGNAL THAT '*this' IS AVAILABLE
        pthread_cond_signal(&train_cond);

        return;
    }


    //  II.B.  Wait until '*this' Track is clear:
    while  (getNumTrains() >= MAX_ALLOWED_NUM_TRAINS_ON_TRACK)
    {
        //  ????
        pthread_cond_wait(&train_cond, TrainLocation::getLockPtr());

        if  ( !trainPtr->getMassTransit().getShouldContinue() )
        {
            //  YOUR CODE HERE TO UNLOCK
            pthread_mutex_unlock(TrainLocation::getLockPtr());
            //  YOUR CODE HERE TO SIGNAL THAT '*this' IS AVAILABLE
            pthread_cond_signal(&train_cond);

            return;
        }

    }

    //  II.C.  Put '*trainPtr' here:
    enqueue(trainPtr);
    trainPtr->setLocPtr(this);

    //  III.  Finished:
}


//  PURPOSE:  To make '*trainPtr' leave '*this'.  No return value.
//  YOUR CODE SOMEWHERE IN HERE TO UNLOCK 'getLockPtr()' AND SIGNAL TRACK
//  IS AVAILABLE.
void Track::leave (Train* trainPtr ) throw()
{
    //  I.  Application validity check:

    //  II.  Make '*trainPtr' leave '*this':
    dequeue(trainPtr);
    trainPtr->setLocPtr(NULL);

    pthread_mutex_unlock(TrainLocation::getLockPtr());
    pthread_cond_signal(&train_cond);
    //  III.  Finished:
}
