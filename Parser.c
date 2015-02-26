#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Parser.h"
#include "Lex.h"
#include "Constants.h"

//Next operand and register counter
RegisterCounter rc;
char nextOperand[MAX_SYMBOL_SIZE];         //Buffer to hold operands
char assignmentTo[MAX_SYMBOL_SIZE];        //Buffer to hold registers
char assignmentBuffer[MAX_SYMBOL_SIZE*4];  //Buffer to hold result and postfix
int assignmentOperator;                    //Determines whether or not we have an assignment




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindDigit
    Extracts the entire number
    @return                -- NUM
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindDigit(int* nextChar, FILE* file, Vector* postfixResult)
{
    char nextDigit[MAX_SYMBOL_SIZE];
    int digitIndex = 0;
    while(isdigit(*nextChar))
    {
        nextDigit[digitIndex++] = *nextChar; //Put digit into string buffer
        *nextChar = fgetc(file);
    }
    ungetc(*nextChar, file);
    nextDigit[digitIndex] = '\0';           //Null terminate digit string
    PrintNextOperand(nextDigit, postfixResult);
    return NUM;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindSymbol
    Extracts the entire symbol from input
    @return                -- Returns ID, BEGIN, or END
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindSymbol(int* nextChar, FILE* file, Vector* symbolTable, Vector* postfixResult)
{
    int underScoreCount = 0;                        //Number of consecutive underscores
    int symbolSize = 0;                             //Size of symbol
    char *symbol = malloc(sizeof(MAX_SYMBOL_SIZE)); //Var to hold symbol
    symbol[symbolSize] = *nextChar;
    while(isalpha(*nextChar) || *nextChar == UNDERSCORE || isdigit(*nextChar)) //
    {
        ExtractSymbol(nextChar, symbol, &underScoreCount, &symbolSize, file);
    }
    symbol[symbolSize] = '\0'; //Insert null char
    ungetc(*nextChar, file);
    IsSymbolInTable(symbolTable, symbol);
    if(underScoreCount != 0)            //Check for symbol ending in underscore
    {
        free(symbol);
        PrintSyntaxError(IllegalIdentifier);
    }
    else if(symbolTable->result == NOT_IN_TABLE)
    {
        InsertSymbol(symbolTable, symbol);
    }
    else if(symbolTable->result == BEGIN_INDEX)
    {
        return BEGIN;
    }
    else if(symbolTable->result == END_INDEX)
    {
        return END;
    }
    if( assignmentOperator == 1 )
    {
        assignmentOperator = 0;
        memcpy(assignmentTo, symbol, MAX_SYMBOL_SIZE);
    }
    else
        PrintNextOperand(symbol, postfixResult);
    return ID;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenFileStream
    Opens the file stream
    @param  fileName       -- The name of the file to open
    @param  filePtr        -- A file pointer
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int OpenFileStream(const char*);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleEndLine
    Gracefully handles end of line statements and returns current line number
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int HandleEndLine(int* nextChar, FILE* file, int* lineNumber)
{
    *nextChar = fgetc(file);
    while( *nextChar == SPACE || *nextChar == TAB ) //ignore spaces and tabs
        *nextChar = fgetc(file);
    if( *nextChar == NEWLINE || *nextChar == EOF)
    {
        int line = *lineNumber;
        return ++line;
    }
    return *lineNumber;
}

////////////////////////////////////////////////////////////////
void DoArithmetic(const char* operator, Vector* postfixResult)
{
    char registerOperation[MAX_REGISTER_SIZE];
    registerOperation[0] = '\0';
    sprintf(registerOperation, "R%d = R%d %s R%d\n",
            rc.registerCount - 2,
            rc.registerCount - 2,
            operator, rc.registerCount - 1);                    //Print Registernum = Register operator register
    printf("%s", registerOperation);
    rc.registerCount--;
    char *operatorToAdd = malloc(sizeof(char) * OPERATOR_SIZE);
    memcpy(operatorToAdd, operator, sizeof(OPERATOR_SIZE));          //Copy operator to table for postfix
    InsertSymbol(postfixResult, operatorToAdd);
}



////////////////////////////////////////////////////////////////
void ExtractSymbol(int* nextChar, char *symbol, int *underScoreCount, int *symbolSize, FILE* file)
{
    if(*nextChar == UNDERSCORE) (*underScoreCount)++;
    else (*underScoreCount) = 0;
    if((*underScoreCount) > 1) PrintSyntaxError(IllegalIdentifier); //If consecutive underscores, invalid
    if((*symbolSize) == MAX_SYMBOL_SIZE-1) PrintSyntaxError(SymbolBufferOverflow); //If no room for null char
    symbol[(*symbolSize)++] = *nextChar;
    *nextChar = fgetc(file);
}


////////////////////////////////////////////////////////////////
void PrintNextOperand(const char* operand, Vector* postfixResult)
{
    sprintf(nextOperand, "R%d = %s\n", rc.registerCount++, operand); //Print register num and operand string
    printf("%s",nextOperand);
    char *operandToAdd = malloc(sizeof(char) * MAX_SYMBOL_SIZE);
    operandToAdd[0] = '\0';
    strcat(operandToAdd, operand);                //Copy operand to table for postfix printing
    InsertSymbol(postfixResult, operandToAdd);
}


////////////////////////////////////////////////////////////////
void AssignResultAndPrintPostfix(Vector* postfixResult)
{
    assignmentBuffer[0] = '\0';     //Empty assignmentBuffer
    char tempString[256];           //Temp string to hold results and postfix
    tempString[0] = '\0';

    sprintf(tempString, "%s = R0\n", assignmentTo);
    strcat(assignmentBuffer,tempString);
    int i;
    for(i = 0; i < postfixResult->size-1; i++)
    {
        sprintf(tempString, "%s,", TableLookupByIndex(postfixResult, i));
        strcat(assignmentBuffer, tempString);   //Concat all postfix to assnBuffer
    }
    sprintf(tempString,"%s\n", TableLookupByIndex(postfixResult, i));
    strcat(assignmentBuffer, tempString);


    EmptyTable(postfixResult);      //Empty the postfix table and create a new one
    NewSymbolTable(postfixResult);
    SetAssignmentOperator(1);
    EmptyAssignmentString();        //Empty the assignTo string
}




void SetAssignmentOperator(int num)
{
    assignmentOperator = num;
}

void SetRegisterCount(int count)
{
    rc.registerCount = count;
}

//Empty
void EmptyAssignmentString()
{
    assignmentTo[0] = '\0';
}

void PrintResult()
{
    printf("%s", assignmentBuffer);
}