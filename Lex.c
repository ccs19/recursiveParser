




#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Lex.h"
#include "Constants.h"



//Globals
FILE *m_file;               //File pointer
Vector * symbolTable;       //Symbol Table
Vector * postfixResult;
int m_lineNumber = 1;       //Current line number
int m_lookAhead = 0;
int nextChar = 0;
int printResult = 0;


//Constant operator strings
const char TIMES_SYMBOL[OPERATOR_SIZE] = "*\0";
const char DIVIDES_SYMBOL[OPERATOR_SIZE] = "/\0";
const char PLUS_SYMBOL[OPERATOR_SIZE] = "+\0";
const char MINUS_SYMBOL[OPERATOR_SIZE] = "-\0";
const char EQUALS_SYMBOL[OPERATOR_SIZE] = "=\0";


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
        printf("Failed to open file %s\n", argv[1]);
        return 0;
    }
    else
    {
        EmptyAssignmentString();
        SetAssignmentOperator(1);  //First assignment statement
        m_lookAhead = Lexan();
        Match(BEGIN);
        while(m_lookAhead != END)
        {
            SetRegisterCount(0);  //Reset registers
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
    @return                -- 0 if fails, 1 if success
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int ReadySymbolTable()
{
    NewSymbolTable(symbolTable);
    if(symbolTable == NULL) // Failed to allocate memory if this fails
    {
        return 0;
    }
    else
    {
        char *begin = malloc(sizeof(char) * MAX_SYMBOL_SIZE);
        char *end = malloc(sizeof(char) * MAX_SYMBOL_SIZE);
        char *dash = malloc(sizeof(char) * MAX_SYMBOL_SIZE);
        begin[0] = end[0] = dash[0] = '\0';
        strcat(begin, "begin");
        strcat(end, "end");
        strcat(dash, "-");

        InsertSymbol(symbolTable, dash);
        InsertSymbol(symbolTable, begin);
        InsertSymbol(symbolTable, end);

        //TODO Clean this up
        NewSymbolTable(postfixResult);

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
        if(nextChar == SEMICOLON)
        {
            m_lineNumber = HandleEndLine(&nextChar, m_file, &m_lineNumber);
            printResult = 1;
            AssignResultAndPrintPostfix(postfixResult);
        }
        else if(nextChar == TILDA) //Handle comments
        {
            while(nextChar == TILDA) {
                if (nextChar == TILDA)//NOM NOM comments
                {                  //Eat line of comments
                    while (nextChar != NEWLINE) {
                        nextChar = fgetc(m_file);
                    }
                    m_lineNumber++;
                }
            }
        }
        else if(nextChar == SPACE || nextChar == TAB)
        {
            ; //do nothing if space or tab
        }
        else if(isdigit(nextChar))
        {
            return FindDigit(&nextChar, m_file, postfixResult);
        }
        else if(isalpha(nextChar))
        {
            return FindSymbol(&nextChar, m_file, symbolTable, postfixResult);
        }
        else if(nextChar == EOF)
            return DONE;
        else if(nextChar == NEWLINE && m_lookAhead == BEGIN)
        {
            ;
        }
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
    if(m_lookAhead != EQUALS)
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
    while(m_lookAhead == TIMES || m_lookAhead == DIVIDES)
    {
        int operator = m_lookAhead;
        Match(m_lookAhead);
        Factor();
        if(operator == TIMES)
        {
            DoArithmetic(TIMES_SYMBOL, postfixResult);
        }
        else if(operator == DIVIDES)
        {
            DoArithmetic(DIVIDES_SYMBOL, postfixResult);
        }
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
    else if(m_lookAhead == OPEN_PAREN)
    {
        Match(OPEN_PAREN);
        Expression();
        Match(CLOSE_PAREN);
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
    while(m_lookAhead == PLUS || m_lookAhead == MINUS)
    {
        int operator = m_lookAhead;
        Match(m_lookAhead);
        Term();
        if(operator == PLUS)
        {
            DoArithmetic(PLUS_SYMBOL, postfixResult);
        }
        else if(operator == MINUS)
        {
            DoArithmetic(MINUS_SYMBOL, postfixResult);
        }
    }
    if(printResult == 1)
    {
        printResult = 0;
        PrintResult();
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