/*-------------------------------------------------------------------------*
 *---									---*
 *---		TrainLocation.h						---*
 *---									---*
 *---	    This file declares a class that serves as the base class	---*
 *---	for classes that represent where a Train can be: Station and 	---*
 *---	Track.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class	TrainLocation
{
    //  I.  Member vars:
    //  PURPOSE:  To point to the name of '*this' TrainLocation:
    char* nameCPtr;

    //  PURPOSE:  To keep track of the trains at '*this' TrainLocation:
    std::list<Train*> trainPtrQueue;

    //  PURPOSE:  To lock '*this' so only one 'Train' thread instance at a time
    //	may access it.
    //  YOUR CODE HERE TO DEFINE A MUTEX
    pthread_mutex_t train_lock;

    //  II.  Disallowed auto-generated methods:
    //  No default constructor:
    TrainLocation();

    //  No copy constructor:
    TrainLocation(const TrainLocation&);

    //  No copy assignment op:
    TrainLocation& operator=(const TrainLocation&);

protected :
    //  III.  Protected methods:

public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To create a TrainLocation named 'newNameCPtr' with no trains.
    //	No return value.
    TrainLocation(const char* newNameCPtr) throw() : nameCPtr(strndup(newNameCPtr,MAX_STRING_LEN-1))
    {
        //  YOUR CODE HERE TO INITIALIZE YOUR MUTEX
        pthread_mutex_init(&train_lock, NULL);
    }

    //  PURPOSE:  To release resources.  No parameters.  No return value.
    virtual ~TrainLocation()
    {
        //  YOUR CODE HERE TO DESTROY YOUR MUTEX
        safeFree(nameCPtr);
        pthread_mutex_destroy(&train_lock);
    }

    //  V.  Accessors:
    //  PURPOSE:  To return the name of '*this' station.  No parameters.
    const char*	getNameCPtr() const throw()
    {
        return(nameCPtr);
    }

    //  PURPOSE:  To return the number of trains.  No parameters.
    uint getNumTrains() const throw()
    {
        return(trainPtrQueue.size());
    }

    //  PURPOSE:  To return a pointer to 'lock'.  No parameters.
    pthread_mutex_t* getLockPtr() throw()
    {
        return(&train_lock);
    }

    //  VI.  Mutators:

    //  VII.  Methods that do main & misc work of class:
    //  PURPOSE:  To put '*trainPtr' into the end of 'trainPtrQueue'.  No return
    //	value.
    void enqueue(Train*	trainPtr) throw()
    {
        trainPtrQueue.push_back(trainPtr);
    }

    //  PURPOSE:  To remove 'trainPtr' from 'trainPtrQueue'.  No return value.
    void dequeue(Train*	trainPtr) throw()
    {
        trainPtrQueue.remove(trainPtr);
    }

    //  PURPOSE:  To return 'true' if '*trainPtr' is the first 'Train' instance,
    //	or 'false' otherwise.
    bool isFirstTrain(const Train* trainPtr) const throw();

    //  PURPOSE:  To print '*this' and its Train instances.  No parameters.
    void print() const throw();

    //  PURPOSE:  To return 'true' if '*trainPtr' can leave '*this'
    //	TrainLocation, or 'false' otherwise.
    virtual bool canLeave(Train* trainPtr) const throw()
    {
        return(true);
    }

    //  PURPOSE:  To return a pointer to the next TrainLocation for '*trainPtr'
    //  	after it leaves '*this'.
    virtual TrainLocation*	nextLocPtr(Train* trainPtr) = 0;

    //  PURPOSE:  To make '*trainPtr' arrive at '*this'.  No return value.
    virtual void arrive(Train* trainPtr) throw() = 0;

    //  PURPOSE:  To make '*trainPtr' leave '*this'.  No return value.
    virtual void leave(Train* trainPtr) throw() = 0;

};
