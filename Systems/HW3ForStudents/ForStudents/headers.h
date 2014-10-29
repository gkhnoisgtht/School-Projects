/*-------------------------------------------------------------------------*
 *---									---*
 *---		headers.h						---*
 *---									---*
 *---	    This file declares the common inclusion sequence and	---*
 *---	common low-level types used in the massTransit program.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2013 May 10		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

/*---			Common inclusions:				---*/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>	// For sleep()

#include <list>

#include <ncurses.h>	// For screen control
#include <pthread.h>	// For pthreads



/*---			Common primitive types:				---*/

//  PURPOSE:  To represent one of two transit lines.
typedef	enum
{
    REDLINE,
    BROWNLINE,
    NUM_LINES,
    MIN_LINE	= REDLINE,
    MAX_LINE	= BROWNLINE
} line_t;


//  PURPOSE:  To represent one of two directions in which trains may head.
typedef	enum
{
    NORTH,
    SOUTH,
    NUM_DIRECTIONS,
    MIN_DIRECTION	= NORTH,
    MAX_DIRECTION	= SOUTH
} direction_t;


//  PURPOSE:  To define an abbreviated name integer name.
typedef	unsigned int uint;


/*---			Common constants:				---*/

//  PURPOSE:  To tell the maximum length of C strings.
const uint MAX_STRING_LEN = 256;

//  PURPOSE:  To tell maximum number of trains simultaneously allowed on a
//	single track.
const uint MAX_ALLOWED_NUM_TRAINS_ON_TRACK = 1;

//  PURPOSE:  To tell the number of Train instances to make.
const uint NUM_TRAINS = 16;


/*---			Common macros and templated fncs:		---*/

//  PURPOSE:  To free() 'ptr' and set it equal to 'NULL'.  No return value.
template<class T> inline void safeFree (T*& ptr)
{
    free(ptr);
    ptr = NULL;
}


//  PURPOSE:  To delete() 'ptr' and set it equal to 'NULL'.  No return value.
template<class T> inline void safeDelete (T*& ptr)
{
    delete(ptr);
    ptr = NULL;
}



/*---	Declaration of classes and fncs so compiler doesn't freak:	---*/

class Train;

class Track;

class MassTransit;

void* simulateTrain (void* vPtr);



/*---		Inclusion of header files unique to this program:	---*/

#include "TrainLocation.h"
#include "Station.h"
#include "Track.h"
#include "Train.h"
#include "MassTransit.h"
