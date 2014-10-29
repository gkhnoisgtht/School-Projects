/*-------------------------------------------------------------------------*
 *---									---*
 *---		Train.h							---*
 *---									---*
 *---	    This file declares a class that simulates one train for a	---*
 *---	MassTransit instance.  This class is designed so that each	---*
 *---	instance is done by its own thread.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class	Train
{
    //  I.  Member vars:
    //  PURPOSE:  To tell the id of the train.
    int identity;

    //  PURPOSE:  To tell the line that '*this' Train serves.
    line_t line;

    //  PURPOSE:  To tell the direction in which '*this' Train is currently
    //	headed.
    direction_t direction;

    //  PURPOSE:  To tell where '*this' Train is.
    TrainLocation* locPtr;

    //  PURPOSE:  To refer to the MassTransit system in which '*this' Train
    //	operates.
    MassTransit& massTransit;

    //  II.  Disallowed auto-generated methods:
    //  No default constructor:
    Train();

    //  No copy constructor:
    Train(const Train&);

    //  No copy assignment op:
    Train operator=(const Train&);

protected :
    //  III.  Protected methods:

public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To initialize '*this' Train instance to have id 'newId', to
    //	run on line 'newLine' in direction 'newDir', to start at location
    //	'*newLocPtr' on MassTransit system 'newMassTransit'.  No return value.
    Train(int newId, line_t newLine, direction_t newDir, TrainLocation* newLocPtr,
          MassTransit& newMassTransit) throw():
        identity(newId), line(newLine), direction(newDir), locPtr(newLocPtr), massTransit(newMassTransit)
    { }

    //  PURPOSE:  To release resources.  No parameters.  No return value.
    ~Train () throw() { }

    //  V.  Accessors:
    //  PURPOSE:  To return the id of the train.  No parameters.
    int getIdentity() const throw()
    {
        return(identity);
    }

    //  PURPOSE:  To return the line that '*this' Train serves.  No parameters.
    line_t getLine() const throw()
    {
        return(line);
    }

    //  PURPOSE:  To return the direction in which '*this' Train is currently
    //	headed.  No parameters.
    direction_t getDirection() const throw()
    {
        return(direction);
    }

    //  PURPOSE:  To return where '*this' Train is.  No parameters.
    TrainLocation* getLocPtr() const throw()
    {
        return(locPtr);
    }

    //  PURPOSE:  To return the MassTransit system in which '*this' Train
    //	operates.
    MassTransit& getMassTransit() const throw()
    {
        return(massTransit);
    }

    //  VI.  Mutators:
    //  PURPOSE:  To switch directions.  No parameters.  No return value.
    void switchDiretion() throw()
    {
        if  (getDirection() == NORTH)
            direction = SOUTH;
        else
            direction = NORTH;
    }

    //  PURPOSE:  To place '*this' Train at '*ptr'.  No return value.
    void setLocPtr(TrainLocation* ptr) throw()
    {
        locPtr = ptr;
    }

    //  VII.  Methods that do main and misc. work of class:
    //  PURPOSE:  To print the name of '*this' to the ncurses-controlled screen.
    //	No parameters.  No return value.
    void print () const throw()
    {
        char text[MAX_STRING_LEN];

        snprintf(text,MAX_STRING_LEN,"%s%d ", (const char*)((getLine() == REDLINE) ? "Red" : "Brn"), getIdentity());
        addstr(text);
    }
};
