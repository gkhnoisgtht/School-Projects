/*-------------------------------------------------------------------------*
 *---									---*
 *---		Station.h						---*
 *---									---*
 *---	    This file declares a class that represents a Station:	---*
 *---	where several Train instances may be without incident. 		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class	Station : public TrainLocation
{
    //  I.  Member vars:
    //  PURPOSE:  To keep points to the tracks leading away from '*this' Station.
    Track* trackArray[NUM_LINES][NUM_DIRECTIONS];

    //  II.  Disallowed auto-generated methods:
    //  No default constructor:
    Station();

    //  No copy constructor:
    Station(const Station&);

    //  No copy assignment op:
    Station operator= (const Station&);

protected :
    //  III.  Protected methods:

public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To create a station named 'newNameCPtr' with no trains, and
    //	initially no tracks.
    Station(const char* newNameCPtr )	throw() : TrainLocation(newNameCPtr)
    {
        //  I.  Applicability validity check:

        //  II.  Initialize other members:
        for (uint line = (uint)MIN_LINE;  line <= (uint)MAX_LINE;  line++)
            for (uint dir = (uint)MIN_DIRECTION;  dir <= (uint)MAX_DIRECTION;  dir++)
                trackArray[line][dir] = NULL;

        //  III.  Finished:
    }


    //  PURPOSE:  To release resources.  No parameters.  No return value.
    ~Station() throw();

    //  V.  Accessors:
    //  PURPOSE:  To return a ptr to the Track leading away from '*this' Station
    //	for line 'line' in direction 'dir'.
    Track* getTrackPtr(line_t line, direction_t	direction) const throw()
    {
        //  I.  Application validity check:

        //  II.  Return value:
        return(trackArray[(uint)line][(uint)direction]);
    }


    //  VI.  Mutators:
    //  PURPOSE:  To note that Track '*trackPtr' leads out of '*this' Station
    //	for line 'line' in direction 'dir'.  No return value.
    void setTrackPtr(line_t line, direction_t direction, Track* trackPtr) throw()
    {
        //  I.  Application validity check:

        //  II.  Set value:
        trackArray[(uint)line][(uint)direction] = trackPtr;

        //  III.  Finished:
    }


    //  VII.  Methods that do main and misc. work of class:
    //  PURPOSE:  To return 'true' if '*trainPtr' can leave '*this'
    //	TrainLocation, or 'false' otherwise.
    bool canLeave(Train* trainPtr) const throw();

    //  PURPOSE:  To return a pointer to the next TrainLocation for '*trainPtr'
    //  	after it leaves '*this'.
    TrainLocation* nextLocPtr(Train* trainPtr);

    //  PURPOSE:  To make '*trainPtr' arrive at '*this'.  No return value.
    void arrive(Train* trainPtr) throw();

    //  PURPOSE:  To make '*trainPtr' leave '*this'.  No return value.
    void leave(Train* trainPtr) throw();
};
