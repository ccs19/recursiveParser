#include <stdio.h>
#include <ctype.h>

#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Lex.h"
#include "SymbolTable.h"


//Globals
FILE *m_file;               //File pointer
Vector * symbolTable;   //Symbol Table
int m_lineNumber = 1;       //Current line number
int m_lookAhead = 0;
int nextChar = 0;

//Constants
const int MAX_SYMBOL_SIZE = 256;


int main(int argc, const char* argv[]) {
    if(!ReadySymbolTable()) //This should only fail if malloc fails
    {
        printf("Failed to initialize Vector. Exiting.\n");
    }
    else if(!OpenFileStream()) //We failed to open file
    {
        printf("Failed to open file.");
    }
    else
    {
        m_lookAhead = Lexan();
        Match(BEGIN);
        while(m_lookAhead != END)
        {
            m_lookAhead = Lexan();
        }
        if(fgetc(m_file) != '.')
        {
            PrintSyntaxError(SyntaxError);
        }
    }
    int i;
   for(i = symbolTable->size-1; i != 0; i--)
        printf("\n%s ", TableLookupByIndex(symbolTable,i));
    EmptyTable(symbolTable); //We're done! Good times were had by all!

    return 0;
}
/*===========================================================================*/
int ReadySymbolTable()
{
    NewSymbolTable(symbolTable);
    //symbolTable = InitVector();
    if(symbolTable == NULL) // Failed to allocate memory if this fails
    {
        return 0;
    }
    else
    {
        AddToVector(symbolTable, "-");
        AddToVector(symbolTable, "begin");
        AddToVector(symbolTable, "end");
        return 1;
    }
}

/*===========================================================================*/

int Lexan()
{

    while( 1 )
    {
        nextChar = fgetc(m_file);
        if(nextChar == ' ' || nextChar == '\t')
        {
           ; //do nothing if space or tab
        }
        else if(nextChar == '\n')
        {
            m_lineNumber++; //If newline increase line count.
        }
        else if(isdigit(nextChar) || nextChar == '.') //If digit or decimal
        {
            int decimalCount = 0;
            while(isdigit(nextChar) || nextChar == '.') //Tested good! :D TODO: Need to test negative numbers!
            {
                if(nextChar == '.') decimalCount++;
                if(decimalCount > 1) PrintSyntaxError(IllegalNumber); //if multiple decimal points found, invalid number
                nextChar = fgetc(m_file);
                printf("%c", nextChar);
            }
            ungetc(nextChar, m_file);
            return NUM;
        }
        else if(isalpha(nextChar)) //If character
        {
            return FindSymbol(nextChar);
        }
    }
}
/*===========================================================================*/

int OpenFileStream() //TODO add argument from argv
{
    m_file = fopen("/home/christopher/ClionProjects/recusriveParser/temp.txt", "r");

    return (m_file == NULL ? 0 : 1); //If failed to open file, return 0, else 1;
}
/*===========================================================================*/

char *GetSymbol(FILE* m_file)
{
    ungetc(1, m_file);
    return 0;
}



/*===========================================================================*/
void PrintSyntaxError(ErrorMessage errorMessage)
{
    printf("\nLine %d contains error ",m_lineNumber);
    switch(errorMessage)
    {
        case MissingSquareBracket:
            printf("\'Missing square bracket\'");
            break;
        case MissingParen:
            printf("\'Parend Mismatch\'");
            break;
        case IllegalNumber:
            printf("\'Illegal Number\'");
            break;//Too many decimal Places
        case IllegalIdentifier:
            printf("\'Illegal Identifier\'");
            break;
        case UnterminatedComment:
            printf("\'Unterminated Comment\'");
            break;
        case SymbolBufferOverflow:
            printf("\'Symbol size exceeds max of %d\'", MAX_SYMBOL_SIZE);
            break;
        case SyntaxError:
            printf("\'Syntax Error\'");
            break;
        default:
            printf("\'Unknown Error\'");
            break;
    }
    fflush(stdout);
    exit(1);
}


/*===========================================================================*/

//TODO TEST THIS CODE!!!!!
int FindSymbol(int nextChar)
{
    int underScoreCount = 0; //Number of consecutive underscores
    int symbolSize = 0;      //Size of symbol
    char *symbol = malloc(sizeof(MAX_SYMBOL_SIZE)); //Symbol array
    //int position = -1;          //Location of symbol in table

    symbol[symbolSize] = nextChar;
    while(isalpha(nextChar) || nextChar == '_' || isdigit(nextChar)) //
    {
        if(nextChar == '_') underScoreCount++;
        else underScoreCount = 0;
        if(underScoreCount > 1) PrintSyntaxError(IllegalIdentifier); //If consecutive underscores, invalid
        if(symbolSize == MAX_SYMBOL_SIZE-1) PrintSyntaxError(SymbolBufferOverflow); //If no room for null char
        symbol[symbolSize++] = nextChar;
        nextChar = fgetc(m_file);
    }
    symbol[symbolSize] = '\0'; //Insert null char
    //printf("Symbol: %s\n", symbol);
    ungetc(nextChar, m_file);
    IsSymbolInTable(symbolTable, symbol);
    if(underScoreCount != 0)
    {
        PrintSyntaxError(IllegalIdentifier);
    }
    else if(symbolTable->result == -1)
    {
        InsertSymbol(symbolTable, symbol); //TODO Fix memory leak
    }
    else if(symbolTable->result == 1) //Begin index
    {
        return BEGIN;
    }
    else if(symbolTable->result == 2) //End index
    {
        return END;
    }
    return ID;
}


void Match(int type)
{
    if(m_lookAhead == type)
        m_lookAhead = Lexan();
    else
        PrintSyntaxError(SyntaxError);
}