/*--*
 *--*		File inclusions:
 *--*/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


/*--*
 *--*		Constant definitions:
 *--*/
//  PURPOSE:  To tell the character that separates the main name from the file
//	extension in a filename.
#define	EXTENSION_CHAR	'.'

//  PURPOSE:  To tell the maximum length (or width) of strings.
#define	MAX_LINE 64

//  PURPOSE:  To tell the number of fields to print.
#define	NUM_FIELDS	2

//  PURPOSE:  To tell the minimum width of any field.
#define	MIN_FIELD_WIDTH	3

//  PURPOSE:  To tell the maximum width of any field.
#define	MAX_FIELD_WIDTH	(MAX_LINE - (NUM_FIELDS-1)*MIN_FIELD_WIDTH)


/*--*
 *--*		Function definitions:
 *--*/

//  PURPOSE:  To return '0' if 'width' is not a legal field width or
//	a non-'0' integer if it is.
int	isLegalFieldWidth(int width)
{
    //  I.  Application validity check:

    //  II.  Return value:
    return( (width >= MIN_FIELD_WIDTH) && (width <= MAX_FIELD_WIDTH) );
}


//  PURPOSE:  To have the user enter the width of the field named 'fieldName'
//	and to return the legal entry.
int	askFieldWidth (const char* fieldName)
{
    //  I.  Application validity check:

    //  II.  Ask field width:
    //  II.A.  Each iteration tries to get a valid field width:
    char line[MAX_LINE];
    int	toReturn;
    char* cPtr;

    do
    {
        printf("Please enter the width of the %s width (%d-%d): ", fieldName,MIN_FIELD_WIDTH,MAX_FIELD_WIDTH);
        fgets(line,MAX_LINE,stdin);
        toReturn = strtol(line,&cPtr,10);
    }
    while  ( (cPtr == line)  ||  !isLegalFieldWidth(toReturn) );

    //  III.  Finished:
    return(toReturn);
}


//  PURPOSE:  To obtain the widths of both the filename and extension fields
//	from either the command line, or by asking the user.  'argc' tells
//	the number of items that were on the command line, 'argv[]' is an
//	array of pointers to the individual arguments.  'filenameWidthPtr'
//	points to the integer that tells the width of the given filename
//	field.  'extWidthPtr' points to the integer that tells the width of
//	the extension field.  No return value.
void	obtainFieldWidths	(int argc, char* argv[], int* filenameWidthPtr, int* extWidthPtr)
{
    //  I.  Application validity check:

    //  II.  Obtain field widths:
    //  II.A.  Set '*filenameWidthPtr' and '*extWidthPtr' to initially illegal
    //  	     values to force checking them in II.C.:
    *filenameWidthPtr = MAX_LINE;
    *extWidthPtr = MAX_LINE;

    //  II.B.  Attempt to gain field widths from command line ('argv[]'):
    if  ( (argc < 2) || !isLegalFieldWidth(*filenameWidthPtr = strtol(argv[1],NULL,10)))
        *filenameWidthPtr = askFieldWidth("filename");

    if  ( (argc < 3) || !isLegalFieldWidth(*extWidthPtr = strtol(argv[2],NULL,10)))
        *extWidthPtr = askFieldWidth("extension");

    //  II.C.  Each iteration asks the user for the field widths and then checks
    //	     that they are legal:
    while ( (*filenameWidthPtr + *extWidthPtr) > MAX_LINE )
    {
        printf("The combined width must be less than or equal to %d.\n",MAX_LINE);
        *filenameWidthPtr = askFieldWidth("filename");
        *extWidthPtr = askFieldWidth("extension");
    }

    //  III. Finished:
}


//  PURPOSE:  To ask the user for a filename (given name and extension), and
//	to write the first 'filenameWidth'-1 characters of it into 'filename'.
//	No return value.
void obtainFilename (char* filename, int filenameWidth)
{
    //  I.  Application validity check:

    //  II.  Ask for and get file name:
    //  II.A.  Ask for and get file name:
    printf("Filename (or blank line to quit): ");
    fgets(filename,filenameWidth,stdin);

    //  II.B.  Remove ending '\n' from 'filename' (if present):
    char* cPtr = strchr(filename,'\n');

    if  (cPtr != NULL)
        *cPtr = '\0';

    //  III.  Finished:
}


//  PURPOSE:  To return '0' if 'cPtr' points to an empty string, or non-'0'
//	otherwise.
int	isBlank (const char* cPtr)
{
    //  I.  Application validity check:

    //  II.  Fast-forward past space chars:
    while ( isspace(*cPtr) )
        cPtr++;

    //  III.  Return value:
    return(*cPtr == '\0');
}


//  PURPOSE:  To tokenize the whole filename 'wholeFilename' into the given
//	filename (the first 'filenameWidth'-1 chars of which are to be written
//	into the space pointed to by 'filename'), and the extension (the first
//	'extensionWidth'-1 chars of which are to be written into the space
//	pointed to by 'extension').  No return value.
void tokenizeFilename (const char* wholeFilename, char* filename, int filenameWidth, char* extension, int extensionWidth)
{
    //  I.  Application validity check:

    //  II.  Tokenize 'wholeFilename':
    //  II.A.  Blank both strings:
    filename[0]	= '\0';
    extension[0] = '\0';

    //  II.B.  Fast-forward past any spaces:
    //  II.B.1.  Each iteration fast-forwards past one space char:
    int	sourceInd;

    for  (sourceInd = 0;  isspace(wholeFilename[sourceInd]);  sourceInd++);

    //  II.B.2.  If have exhausted chars in 'wholeFilename[]' then end:
    if  (wholeFilename[sourceInd] == '\0')
        return;

    //  II.C.  Write the given filename portion into 'filename':
    //  II.C.1.  Each iteration copies on more char from 'wholeFilename[]' to
    //	       'filename[]':
    int	destInd;

    for  (destInd = 0;  destInd < filenameWidth-1;  sourceInd++, destInd++)
    {
        if  ( (wholeFilename[sourceInd] == '\0') ||  (wholeFilename[sourceInd] == EXTENSION_CHAR))
            break;

        filename[destInd] = wholeFilename[sourceInd];
    }

    //  II.C.2.  End 'filename[]' string:
    filename[destInd] = '\0';

    //  II.C.3.  Finished if at end of 'wholeFilename[]':
    if (wholeFilename[sourceInd] == '\0')
        return;

    //  II.D.  Write the extension portion into 'extension':
    //  II.D.1.  Each iteration copies on more char from 'wholeFilename[]' to
    //	       'extension[]':
    for (   destInd = 0, // Reset 'destInd' (to beginning of 'extension[]')
            sourceInd++; // Pass 'EXTENSION_CHAR'
            destInd < extensionWidth;
            sourceInd++, destInd++
        )
    {
        if (wholeFilename[sourceInd] == '\0')
            break;

        extension[destInd] = wholeFilename[sourceInd];
    }

    //  II.D.2.  End 'filename[]' string:
    extension[destInd] = '\0';

    //  III.  Finished:
}


//  PURPOSE:  To return a string allocated from memory on the HEAP that
//	contains stylized formatting of the filename with given name
//	'filename' and extension 'extension'.  The stylized string has
//	the first 'filenameWidth' characters of 'filename' (left justified)
//	followed by a single space character, followed by the first
//	'extensionWidth' characters of 'extension' (left justified), followed
//	by the '\0' character.
//
//	For example, assume 'filenameWidth' == 8 and 'extensionWidth' == 3:
//	'filename'	'extension'	Returned string:
//	----------	-----------	----------------
//	"filename"	"txt"		"filename txt"
//	"longFileName"	"text"		"longFile tex"
//	"short"		"t"		"short    t  "
//	"MyClass"	"h"		"MyClass  h  "
//	"MyClass"	"cpp"		"MyClass  cpp"
//	"MyClass"	"cpp~"		"MyClass  cpp"
char* getDescription (const char* filename, int filenameWidth, const char* extension, int extensionWidth)
{
    //  I.  Application validity check:

    //  II.  Construct heap string:
    //  YOUR CODE HERE TO IMPLEMENT THE FUNCTION
    char* toReturn = malloc((filenameWidth + extensionWidth + 1) * sizeof(char));
    memset(toReturn, ' ', (filenameWidth + extensionWidth + 1));
    int i;
    int j = 0;
    for(i = 0; i < (filenameWidth + extensionWidth + 1); i++)
    {
        if(i < strlen(filename) && i < filenameWidth){
            toReturn[i] = filename[i];
        }

        if(i > filenameWidth && j < strlen(extension)){
            toReturn[i] = extension[j];
            j++;
        }
    }
    toReturn[(filenameWidth + extensionWidth + 1)] = '\0';

    //  III.  Finished:
    return(toReturn);
}


//  PURPOSE:  To obtain field widths, and then for each entered filename, to
//	print it out in a stylized fashion which conforms to the field widths.
//	'argc' tells the number of command-line arguments and 'argv[]' points
//	to each individual command line argument.  Returns 'EXIT_SUCCESS' to
//	OS.
int	main	(int argc, char* argv[])
{
    //  I.  Application validity check:

    //  II.  Do program:
    //  II.A.  Get field widths:
    int	filenameWidth;
    int	typeWidth;

    obtainFieldWidths(argc,argv,&filenameWidth,&typeWidth);

    //  II.B.  Each iteration obtains a filename and prints it out in
    //	     a stylized fashion:
    while  (1)
    {
        char	wholeFilename[MAX_LINE];
        char	filename[MAX_LINE];
        char	extension[MAX_LINE];

        obtainFilename(wholeFilename,MAX_LINE);

        if  ( isBlank(wholeFilename) )
            break;

        tokenizeFilename(wholeFilename,filename,MAX_LINE,extension,MAX_LINE);
        char* cPtr = getDescription(filename,filenameWidth,extension,typeWidth);

        printf("\"%s\"\n",cPtr);
        //  YOUR CODE HERE TO RELEASE MEMORY OBTAINED ABOVE
        free(cPtr);
    }

    //  III.  Finished:
    return(EXIT_SUCCESS);
}
