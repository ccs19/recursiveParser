




#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Lex.h"


//Globals
FILE *m_file;               //File pointer
Vector * symbolTable;       //Symbol Table
int m_lineNumber = 1;       //Current line number
int m_lookAhead = 0;
int nextChar = 0;

//Constants
const int MAX_SYMBOL_SIZE = 256;



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   main
    Readies the symbol table and handles matching of begin and end
    @param  argv[1]           -- The file to open
    @return                   -- int
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int main(int argc, const char* argv[]) {
    if(!ReadySymbolTable()) //This should only fail if malloc fails
    {
        printf("Failed to initialize Vector. Exiting.\n");
        return 0;
    }
    else if(argc != 2)
    {
        printf("Usage: ./%s <filename>\n", argv[1]);
        return 0;
    }
    else if(!OpenFileStream(argv[1])) //We failed to open file
    {
        printf("Failed to open file.");
        return 0;
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
    EmptyTable(symbolTable); //We're done! Good times were had by all!
    return 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ReadySymbolTable
    Readies the symbol table and adds the first two entries
    @param  port           -- The port number to bind the listen socket
    @return                -- 0 if fails, 1 if success
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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
        char *begin = malloc(sizeof(MAX_SYMBOL_SIZE));
        char *end = malloc(sizeof(MAX_SYMBOL_SIZE));
        char *dash = malloc(sizeof(MAX_SYMBOL_SIZE));
        begin[0] = end[0] = dash[0] = '\0';
        strcat(begin, "begin");
        strcat(end, "end");
        strcat(dash, "-");

        InsertSymbol(symbolTable, dash);
        InsertSymbol(symbolTable, begin);
        InsertSymbol(symbolTable, end);
        return 1;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Lexan
    Lexical analyzer. Returns a token type or an int corresponding to a char
    @return                -- The token or char value
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int Lexan()
{
    for(;;) //while( 1 )
    {
        nextChar = fgetc(m_file);
        if(nextChar == ';')
        {
            HandleEndLine();
        }
        else if(nextChar == '~') //Handle comments
        {
            while(nextChar == '~') {
                if (nextChar == '~')//NOM NOM comments
                {                  //Eat line of comments
                    while (nextChar != '\n') {
                        nextChar = fgetc(m_file);
                    }
                    m_lineNumber++;
                }
            }
        }
        else if(nextChar == ' ' || nextChar == '\t')
        {
           ; //do nothing if space or tab
        }
        else if(isdigit(nextChar))
        {
            return FindDigit();
        }
        else if(isalpha(nextChar))
        {
            return FindSymbol();
        }
        else if(nextChar == EOF)
            return DONE;
        else
            return nextChar;
    }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenFileStream
    Opens the file stream
    @param  fileName       -- The name of the file to open
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int OpenFileStream(const char* fileName) //TODO add argument from argv
{
    m_file = fopen(fileName, "r");
    return (m_file == NULL ? 0 : 1); //If failed to open file, return 0, else 1;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   PrintSyntaxError
    Accepts an ErrorMessage enumerated type and prints an error
    @param  errorMessage   -- The enumerated ErrorMessage type to print
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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
        case ExpectedAssignment:
            printf("\'Expected Assignment Operator\'");
            break;
        case InvalidLineEnding:
            printf("\'Invalid Line Ending\'");
            break;
        default:
            printf("\'Unknown Error\'");
            break;
    }
    printf("\n");
    fflush(stdout);
    exit(1);
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindSymbol
    Extracts the entire symbol from input
    @return                -- Returns ID, BEGIN, or END
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindSymbol()
{
    int underScoreCount = 0;                        //Number of consecutive underscores
    int symbolSize = 0;                             //Size of symbol
    char *symbol = malloc(sizeof(MAX_SYMBOL_SIZE)); //Var to hold symbol
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
    ungetc(nextChar, m_file);
    IsSymbolInTable(symbolTable, symbol);
    if(underScoreCount != 0)            //Check for symbol ending in underscore
    {
        free(symbol);
        PrintSyntaxError(IllegalIdentifier);
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


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Match
    Verifies the correct lookahead type
    @param  type           -- The expected type
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Match(int type)
{
    if (m_lookAhead == type)
    {
        m_lookAhead = Lexan();
    }
    else
    {
        PrintSyntaxError(SyntaxError);
    }
}




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   AssignStatement
    Handles equals sign
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void AssignStatement()
{
    Match(ID);
    if(m_lookAhead != '=')
    {
        PrintSyntaxError(ExpectedAssignment);
    }
    else
    {
        Match(m_lookAhead);
        Expression();
    }

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Term
    Handles multiplication and division
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Term()
{
    Factor();
    while(m_lookAhead == '*' || m_lookAhead == '/')
    {
        Match(m_lookAhead);
        Factor();
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Factor
    Handles matching of ID, NUM, and parentheses
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


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
        PrintSyntaxError(SyntaxError);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Expression
    Handles + and - operations
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Expression()
{
    Term();
    while(m_lookAhead == '+' || m_lookAhead == '-')
    {
        Match(m_lookAhead);
        Term();
    }

}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   PrintSymbols
    Prints the found symbols
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrintSymbols()
{
    int i, j = 1;
    printf("\n========================\nThis is a valid program!\n========================\n\n");
    printf("===============\nSymbol List\n===============");
    for(i = symbolTable->size-1; i > 2; i--) {
        printf("\n%4d  %s  ", j++, TableLookupByIndex(symbolTable,i));
    }
    fclose(m_file);
    printf("\n");
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindDigit
    Extracts the entire number
    @return                -- NUM
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindDigit()
{
    while(isdigit(nextChar)) //Tested good! :D
    {
        nextChar = fgetc(m_file);
    }
    ungetc(nextChar, m_file);
    return NUM;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleEndLine
    Gracefully handles end of line statements
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void HandleEndLine()
{
    nextChar = fgetc(m_file);
    while( nextChar == ' ' || nextChar == '\t' ) //ignore spaces and tabs
        nextChar = fgetc(m_file);
    if( nextChar == '\n' || nextChar == EOF)
        m_lineNumber++;
}
