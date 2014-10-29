/*-------------------------------------------------------------------------*
 *---									---*
 *---		headers.h						---*
 *---									---*
 *---	    This file defines constants common to both pdfServer.c and	---*
 *---	pdfClient.c							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2014 February 25	Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//  PURPOSE:  To define the number of times the pdfServer serves clients
//	before it quits.
#define		NUM_SERVER_ITERS	4


//  PURPOSE:  To define the length of most general buffers and lines.
#define		LINE_LEN		256


//  PURPOSE:  To define the length of filename buffers.
#define		FILENAME_LEN		64


//  PURPOSE:  To define the length of numeric text buffers.
#define		NUM_TEXT_LEN		64


//  PURPOSE:  To define the lowest legal port number for the pdfServer to
//	attempt to monopolize.
#define		LO_LEGAL_PORT_NUM	1001


//  PURPOSE:  To define the highest legal port number for the pdfServer to
//	attempt to monopolize.
#define		HI_LEGAL_PORT_NUM	65535


//  PURPOSE:  To define the character that separates a hostname from a host
//	port number.
#define		HOSTNAME_PORTNUM_SEPARATORY_CHAR	\
					':'


//  PURPOSE:  To define the maximum number of clients that the OS will queue
//	while the pdfServer is wait to handle them.
#define		MAX_NUM_QUEUING_CLIENTS	5


//  PURPOSE:  To define a prefix template for temporary files.
#define		TEMP_FILENAME_PREFIX	"djTempFile_%d_"


//  PURPOSE:  To tell the character that starts the extension portion of
//	filenames.
#define		EXTENSION_CHAR		'.'


//  PURPOSE:  To tell the character that ends a directory name in a filepath.
#define		DIRECTORY_CHAR		'/'


//  PURPOSE:  To tell the extension portion of text files.
#define		TEXT_EXT		".txt"


//  PURPOSE:  To tell the length of 'TEXT_EXT'.
#define		TEXT_EXT_LEN		(sizeof(TEXT_EXT)-1)


//  PURPOSE:  To tell the extension portion of postscript files.
#define		POSTSCRIPT_EXT		".ps"


//  PURPOSE:  To tell the length of 'POSTSCRIPT_EXT'.
#define		POSTSCRIPT_EXT_LEN	(sizeof(POSTSCRIPT_EXT)-1)


//  PURPOSE:  To tell the extension portion of portable document format files.
#define		PDF_EXT			".pdf"


//  PURPOSE:  To tell the length of 'PDF_EXT'.
#define		PDF_EXT_LEN		(sizeof(PDF_EXT)-1)


//  PURPOSE:  To tell the ending sequence expected of 'mkstemp()'
#define		MKSTEMP_STRING		"XXXXXX"


//  PURPOSE:  To define the default hostname to whom pdfClient should try to
//	connect.
#define		HOSTNAME		"localhost.localdomain"


//  PURPOSE:  To define an illegal file descriptor value.
#define		ERROR_FD		-1

int numChildren = 0;


