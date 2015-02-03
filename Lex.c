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


//DEBUG
char* lastSymbol;

//Constants
const int MAX_SYMBOL_SIZE = 256;


int main(int argc, const char* argv[]) {
    if(!ReadySymbolTable()) //This should only fail if malloc fails
    {
        printf("Failed to initialize Vector. Exiting.\n");
    }
    else if(!OpenFileStream(argv[1])) //We failed to open file
    {
        printf("Failed to open file.");
    }
    else
    {
        m_lookAhead = Lexan();
        Match(BEGIN);
        while(m_lookAhead != END)
        {
            AssignStatement();
        }
        Match(END);
        Match('.');
    }

    PrintSymbols();

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
        InsertSymbol(symbolTable, "-");
        InsertSymbol(symbolTable, "begin");
        InsertSymbol(symbolTable, "end");
        return 1;
    }
}

/*===========================================================================*/

int Lexan()
{
    for(;;) //while( 1 )
    {
        nextChar = fgetc(m_file);
        while(nextChar == '~') {
            if (nextChar == '~')//NOM NOM comments
            {                  //Eat line of comments
                while (nextChar != '\n') {
                    nextChar = fgetc(m_file);
                }
                nextChar = fgetc(m_file);
                m_lineNumber++;
            }
        }
        if(nextChar == ' ' || nextChar == '\t')
        {
           ; //do nothing if space or tab
        }
        else if(nextChar == ';')
        {
            HandleEndLine();
        }
        else if(isdigit(nextChar)
                || (nextChar == '-'      //If negative
                && (m_lookAhead != NUM   //And lookahead not a NUM
                &&  m_lookAhead != ID    //And lookahead not an ID
                &&  m_lookAhead != ')'   //And lookahead not a close paren
                &&  m_lookAhead != '(')) //And lookahead not a open paren
                )
        {
            return FindDigit();
        }
        else if(isalpha(nextChar) ||
                m_lookAhead == '(') //If character
        {
            return FindSymbol();
        }
        else if(nextChar == EOF)
            return DONE;
        else
            return nextChar;
    }
}
/*===========================================================================*/

int OpenFileStream(const char* fileName) //TODO add argument from argv
{
    m_file = fopen(fileName, "r");
    return (m_file == NULL ? 0 : 1); //If failed to open file, return 0, else 1;
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
            break;
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
        case UnexpectedAssignment:
            printf("\'Unexpected Assignment\'");
            break;
        case InvalidLineEnding:
            printf("\'Invalid Line Ending\'");
            break;
        default:
            printf("\'Unknown Error\'");
            break;
    }
    fflush(stdout);
    exit(1);
}


/*===========================================================================*/

int FindSymbol()
{
    int underScoreCount = 0;                        //Number of consecutive underscores
    int symbolSize = 0;                             //Size of symbol
    char *symbol = malloc(sizeof(MAX_SYMBOL_SIZE)); //Var to hold symbol
    lastSymbol = symbol;
    symbol[symbolSize] = nextChar;
    while(isalpha(nextChar) || nextChar == '_' ||
            (isdigit(nextChar) && symbolSize != 0) //Hack job to account for negative numbers and symbols
            || (nextChar == '-' && symbolSize == 0))
    {
        if(nextChar == '_') underScoreCount++;
        else underScoreCount = 0;
        if(underScoreCount > 1) PrintSyntaxError(IllegalIdentifier); //If consecutive underscores, invalid
        if(symbolSize == MAX_SYMBOL_SIZE-1) PrintSyntaxError(SymbolBufferOverflow); //If no room for null char
        symbol[symbolSize++] = nextChar;
        nextChar = fgetc(m_file);
        if(symbol[symbolSize-1] == '-')
            symbolSize--;
    }
    symbol[symbolSize] = '\0'; //Insert null char
    ungetc(nextChar, m_file);
    IsSymbolInTable(symbolTable, symbol);
    if(underScoreCount != 0)            //Check for symbol ending in underscore
    {
        PrintSyntaxError(IllegalIdentifier);
    }
    else if(symbolSize == 0)
    {
        return FindDigit();
    }
    else if(symbolTable->result == NOT_IN_TABLE)
    {
        InsertSymbol(symbolTable, symbol); //TODO Fix memory leak
    }
    else if(symbolTable->result == BEGIN_INDEX)
    {
        return BEGIN;
    }
    else if(symbolTable->result == END_INDEX)
    {
        return END;
    }
    return ID;
}


/*===========================================================================*/

void Match(int type)
{
    if(m_lookAhead == type)
        m_lookAhead = Lexan();
    else
        PrintSyntaxError(SyntaxError);
}




/*===========================================================================*/


void AssignStatement()
{
    Match(ID);
    if(m_lookAhead != '=')
    {
        PrintSyntaxError(UnexpectedAssignment);
    }
    else
    {
        Match(m_lookAhead);
        Expression();
    }

}

/*===========================================================================*/


void Term()
{
    Factor();
    while(m_lookAhead == '*' || m_lookAhead == '/')
    {
        Match(m_lookAhead);
        Factor();
    }
}

/*===========================================================================*/

void Factor()
{
    if(m_lookAhead == ID)
    {
        Match(ID);
    }
    else if(m_lookAhead == NUM)
    {
        Match(NUM);
    }
    else if(m_lookAhead == '(')
    {
        Match('(');
        Expression();
        Match(')');
    }
    else
        PrintSyntaxError(MissingParen);
}

/*===========================================================================*/


void Expression()
{
    Term();
    while(m_lookAhead == '+' || m_lookAhead == '-')
    {
        Match(m_lookAhead);
        Term();
    }

}


/*===========================================================================*/


void PrintSymbols()
{
    int i;
    printf("Symbol List\n===============");
    for(i = symbolTable->size-1; i != 0; i--)
        printf("\n%s ", TableLookupByIndex(symbolTable,i));
    EmptyTable(symbolTable); //We're done! Good times were had by all!
    fclose(m_file);
}


/*===========================================================================*/



int FindDigit()
{
    if(nextChar == '-')
        nextChar =  fgetc(m_file); //To ensure negative numbers are accounted for
    while(isdigit(nextChar)) //Tested good! :D
    {
        nextChar = fgetc(m_file);
    }
    ungetc(nextChar, m_file);
    return NUM;
}


/*===========================================================================*/

void HandleEndLine()
{
    nextChar = fgetc(m_file);
    while( nextChar == ' ' || nextChar == '\t' ) //ignore spaces and tabs
        nextChar == fgetc(m_file);
    if( nextChar == '\n' || nextChar == EOF)
        m_lineNumber++;
}