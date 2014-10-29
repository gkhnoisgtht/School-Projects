/*-------------------------------------------------------------------------*
 *---									---*
 *---		TrainLocation.cpp					---*
 *---									---*
 *---	    This file defines a class that serves as the base class	---*
 *---	for classes that represent where a Train can be: Station and 	---*
 *---	Track.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include "headers.h"


//  PURPOSE:  To return 'true' if '*trainPtr' is the first 'Train' instance,
//	or 'false' otherwise.
bool TrainLocation::isFirstTrain(const Train* trainPtr) const throw()
{
    //  I.  Application validity check:

    //  II.  Return value:
    return( (trainPtrQueue.size()  >= 1)  &&  (trainPtrQueue.front() == trainPtr) );
}


//  PURPOSE:  To print '*this' and its Train instances.  No parameters.
void TrainLocation::print() const throw()
{
    //  I.  Application validity check:

    //  II.  Print '*this':
    char	text[MAX_STRING_LEN];

    snprintf(text,MAX_STRING_LEN,"%s [",getNameCPtr());
    addstr(text);

    std::list<Train*>::const_iterator end = trainPtrQueue.end();

    for  (std::list<Train*>::const_iterator
            iter  = trainPtrQueue.begin();
            iter != end;
            iter++
         )
        (*iter)->print();

    addstr("]");

    //  III.  Finished:
}
