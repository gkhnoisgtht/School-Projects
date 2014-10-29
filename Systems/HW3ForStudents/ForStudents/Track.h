/*-------------------------------------------------------------------------*
 *---									---*
 *---		Track.h							---*
 *---									---*
 *---	    This file declares a class that represents a length of	---*
 *---	Track: where only 'MAX_ALLOWED_NUM_TRAINS_ON_TRACK' Train	---*
 *---	instances may be without incident. 				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class	Track : public TrainLocation
{
    //  I.  Member vars:
    //  PURPOSE:  To hold ptrs to the termini of '*this' Track.
    Station* termini[NUM_DIRECTIONS];

    //  PURPOSE:  To hold the condition to signal the availability of '*this'
    //	Track.
    //  YOUR CODE HERE TO DEFINE A pthread_cond_t INSTANCE
    pthread_cond_t train_cond;

    //  II.  Disallowed auto-generated methods:
    //  No default constructor:
    Track ();

    //  No copy constructor:
    Track	(const Track&);

    //  No copy assignment op:
    Track	operator=(const Track&);

protected :
    //  III.  Protected methods:

public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To create a station named 'newNameCPtr' with no trains that
    //	connects Station instances '*northTerminusPtr' on its northern end
    //	with '*southTerminusPtr' on its southern end.
    Track (const char* newNameCPtr, Station* northTerminusPtr, Station*	southTerminusPtr) throw() : TrainLocation(newNameCPtr)
    {
        //  I.  Application validity check:

        //  II.  Initialize other members:
        termini[NORTH]	= northTerminusPtr;
        termini[SOUTH]	= southTerminusPtr;

        //  YOUR CODE HERE TO INITIALIZE YOUR CONDITION
        pthread_cond_init(&train_cond, NULL);
        //  III.  Finished:
    }


    //  PURPOSE:  To release resources.  No parameters.  No return value.
    ~Track () throw()
    {
        //  YOUR CODE HERE TO DESTROY YOUR CONDITION
        pthread_cond_destroy(&train_cond);
    }

    //  V.  Accessors:
    //  PURPOSE:  To return the terminus at the end of direction 'direction'.
    Station& getTerminus (direction_t direction ) const throw()
    {
        return(*termini[direction]);
    }

    //  VI.  Mutators:

    //  VII.  Methods that do main and misc. work of class:
    //  PURPOSE:  To return a pointer to the next TrainLocation for '*trainPtr'
    //  	after it leaves '*this'.
    TrainLocation* nextLocPtr(Train* trainPtr);

    //  PURPOSE:  To make '*trainPtr' arrive at '*this'.  No return value.
    void arrive(Train* trainPtr) throw();

    //  PURPOSE:  To make '*trainPtr' leave '*this'.  No return value.
    void leave(Train*	trainPtr) throw();

};
