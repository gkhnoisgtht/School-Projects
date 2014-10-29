//#include "p1.c"
#include <stdlib.h>
#include <stdio.h>

//  PURPOSE:  To hold the length of the array pointed to by 'intArray'.
int length;


//  PURPOSE:  To hold the array of integers.
int* intArray;


//  PURPOSE:  To hold the maximum value that may place in array 'intArray'.
int maxRandVal;

//  PURPOSE:  To hold the minimum value for 'length'.
extern int minArrayLen;


//  PURPOSE:  To hold the maximum value for 'length'.
extern int maxArrayLen;


//  PURPOSE:  To hold the maximum value for 'maxRandVal'.
extern int maxMaxRandVal;

//  PURPOSE:  To:
//  	(1) have user enter 'length' (by calling 'enterValue()'),
//	(2) have user enter 'maxRandVal' (by calling 'enterValue()'), 
//	(3) define 'intArray' (say 'intArray =(int*)calloc(length,sizeof(int))')
//	(4) fill 'intArray' with random numbers between 0-'maxRandVal'
//	    (use expression '(rand() % (maxRandVal+1))')
//	No parameters.  No return value.
void createArray(){
	enterValue("array's length", minArrayLen, maxArrayLen, &length);
	enterValue("maximum random value", minArrayLen, maxMaxRandVal, &maxRandVal);
	intArray = (int*) calloc(length, sizeof(int));
	int i;	
	for(i = 0; i < length; i++){
		intArray[i]=rand() % (maxRandVal+1);
	}
}


//  PURPOSE:  To print the values in 'intArray[]'.  No parameters.  No return
//	value.
void printArray(){
	int i;	
	for(i = 0; i < length; i++){
		printf("intArray[%d] = %d\n", i, intArray[i]);
	}
}


//  PURPOSE:  To free 'intArray'.  No parameters.  No return value.
void freeArray(){
	if(intArray){
		free(intArray);
		intArray = NULL;
	}
}
