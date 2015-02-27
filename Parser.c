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
FILE* compiledFile;
char compiledFileName[FILENAME_MAX];

//Postfix decoration
const char POSTFIX_START[MAX_SYMBOL_SIZE] = "*****[\0";
const char POSTFIX_END[MAX_SYMBOL_SIZE] = "]*****\n\0";
const char FILE_EXTENSION[MAX_SYMBOL_SIZE] = ".out\0";



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
    if( assignmentOperator == 1 || GetPrintResult() == 1 )
    {
        assignmentOperator = 0;
        memcpy(assignmentTo, symbol, MAX_SYMBOL_SIZE);
    }
    else
        PrintNextOperand(symbol, postfixResult);
    return ID;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleEndLine
    Gracefully handles end of line statements and returns current line number
    @param      nextChar   -- Pointer to current character
    @param      file       -- The file to handle
    @param      lineNumber -- The line number counter
    @return                -- The current line number
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DoArithmetic(const char* operator, Vector* postfixResult)
{
    char registerOperation[MAX_REGISTER_SIZE];
    registerOperation[0] = '\0';
    sprintf(registerOperation, "R%d = R%d %s R%d\n",
            rc.registerCount - 2,
            rc.registerCount - 2,
            operator, rc.registerCount - 1);                    //Print Registernum = Register operator register
    fprintf(compiledFile, "%s", registerOperation);
    rc.registerCount--;
    char *operatorToAdd = malloc(sizeof(char) * OPERATOR_SIZE);
    memcpy(operatorToAdd, operator, sizeof(OPERATOR_SIZE));          //Copy operator to table for postfix
    InsertSymbol(postfixResult, operatorToAdd);
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ExtractSymbol(int* nextChar, char *symbol, int *underScoreCount, int *symbolSize, FILE* file)
{
    if(*nextChar == UNDERSCORE) (*underScoreCount)++;
    else (*underScoreCount) = 0;
    if((*underScoreCount) > 1) PrintSyntaxError(IllegalIdentifier); //If consecutive underscores, invalid
    if((*symbolSize) == MAX_SYMBOL_SIZE-1) PrintSyntaxError(SymbolBufferOverflow); //If no room for null char
    symbol[(*symbolSize)++] = *nextChar;
    *nextChar = fgetc(file);
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrintNextOperand(const char* operand, Vector* postfixResult)
{
    sprintf(nextOperand, "R%d = %s\n", rc.registerCount++, operand); //Print register num and operand string
    fprintf(compiledFile,"%s",nextOperand);
    char *operandToAdd = malloc(sizeof(char) * MAX_SYMBOL_SIZE);
    operandToAdd[0] = '\0';
    strcat(operandToAdd, operand);                //Copy operand to table for postfix printing
    InsertSymbol(postfixResult, operandToAdd);
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AppendResult(Vector* postfixResult)
{
    assignmentBuffer[0] = '\0';     //Empty assignmentBuffer
    char tempString[MAX_SYMBOL_SIZE];           //Temp string to hold results and postfix
    tempString[0] = '\0';

    sprintf(tempString, "%s = R0\n", assignmentTo);
    strcat(assignmentBuffer,tempString);
    SetAssignmentOperator(1);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AppendPostfix(Vector *postfixResult) {
    char tempString[MAX_SYMBOL_SIZE];
    tempString[0] = '\0';

    strcat(assignmentBuffer, POSTFIX_START);

    int i;
    for(i = 0; i < postfixResult->size-1; i++)
    {
        sprintf(tempString, "%s,", TableLookupByIndex(postfixResult, i));
        strcat(assignmentBuffer, tempString);   //Concat all postfix to assnBuffer
    }
    sprintf(tempString,"%s", TableLookupByIndex(postfixResult, i));
    strcat(assignmentBuffer, tempString);

    strcat(assignmentBuffer, POSTFIX_END);

    EmptyTable(postfixResult);      //Empty the postfix table and create a new one
    NewSymbolTable(postfixResult);

}


void SetAssignmentOperator(int num)
{
    assignmentOperator = num;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   SetRegisterCount
    Sets the register counter to the desired number
    @param      count      -- The value to set the register to
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SetRegisterCount(int count)
{
    rc.registerCount = count;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   PrintResult
    Prints the assignmentBuffer containing the final assignment statement
    and the postFix to the output file.
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrintResult()
{
    fprintf(compiledFile, "%s", assignmentBuffer);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenOutputFileStream
    Opens the file stream for writing. The name of the output file is set to fileName.FILE_EXTENSION
    @param  fileName       -- The name of the input file
    @return                -- 1 on success, 0 on failure
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int OpenOutputFileStream(const char* fileName)
{
    char* token;
    compiledFileName[0] = '\0';
    token = strtok((char*)fileName, ".");  //If file has extension, remove it

    strcat(compiledFileName, token);
    strcat(compiledFileName, FILE_EXTENSION);
    compiledFile = fopen(compiledFileName, "w");

    printf("%s\n", compiledFileName);
    return (compiledFile == NULL ? 0 : 1); //If failed to open file, return 0, else 1;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenOutputFileStream
    Closes the output file and saves or deletes it if compilation fails.
    @param  success        -- If 1, the output file is saved. If 0, the output is not saved
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void CloseOutputFileStream(int success)
{
    if(success == 1)
    {
        fclose(compiledFile);
    }
    else
    {
        fclose(compiledFile);
        remove(compiledFileName);
    }
}
