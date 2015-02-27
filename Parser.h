/*
 * =====================================================================================
 *
 *	Author: Christopher Schneider
 *	File Name: Parser.h
 *	Assignment Number: 2
 *
 *	Description: Parser header file. Handles all string parsing and writing the .out
 *               "compiled" data to a file
 * =====================================================================================
 */

#ifndef _PARSER_H
#define _PARSER_H

#include "Constants.h"




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindDigit
    Extracts the entire number
    @return                -- NUM
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindDigit(int*, FILE*, Vector*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindSymbol
    Extracts the entire symbol from input
    @return                -- Returns ID, BEGIN, or END
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindSymbol(int*, FILE*, Vector*, Vector*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenFileStream
    Opens the file stream
    @param  fileName       -- The name of the file to open
    @param  filePtr        -- A file pointer
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int OpenInputFileStream(const char *);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleEndLine
    Gracefully handles end of line statements
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int HandleEndLine(int*, FILE*, int*);


void DoArithmetic(const char*, Vector*);


RegisterCounter* InitRegisterStruct();


void ExtractSymbol(int*, char*, int* , int*, FILE*);

void PrintNextOperand(const char*, Vector*);

void AppendResult();


void SetAssignmentOperator(int);

int GetAssignmentOperatorValue();

void SetRegisterCount(int);

void EmptyAssignmentString();

void PrintResult();

void AppendPostfix(Vector *postfixResult);

int OpenOutputFileStream(const char*);

void CloseOutputFileStream(int, Vector*);
#endif