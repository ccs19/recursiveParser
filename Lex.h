



#include "SymbolTable.h"

#ifndef LEX_H
#define LEX_H


#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401
#define DONE 402


//Constants
#define MAX_SYMBOL_SIZE 256
#define MAX_REGISTER_SIZE 512

//Operators
#define TIMES '*'
#define DIVIDES '/'
#define OPEN_PAREN '('
#define CLOSE_PAREN ')'
#define PLUS '+'
#define MINUS '-'
#define SPACE ' '
#define EQUALS '='

//Other
#define UNDERSCORE '_'
#define NEWLINE '\n'
#define TAB '\t'
#define TILDA '~'
#define SEMICOLON ';'

//Array indices for table
#define BEGIN_INDEX 1
#define END_INDEX 2
#define NOT_IN_TABLE -1
#define START_INDEX 4

enum errmsg{
    MissingSquareBracket,   //Missing bracket
    MissingParen,           //Mismatch parenthesis
    IllegalNumber,          //Too many decimal Places
    IllegalIdentifier,      //Unknown identifier, e.g. %$# or consecutive underscores
    UnterminatedComment,    //Comment not properly terminated
    SymbolBufferOverflow,   //If symbol length exceeds maximum symbol size
    SyntaxError,            //Generic syntax error
    ExpectedAssignment,     //Invalid assignment
    InvalidLineEnding       //If improperly terminated line
};
typedef enum errmsg ErrorMessage;

typedef struct {
    int registerCount;
    char assignTo[MAX_SYMBOL_SIZE];
}RegisterCounter;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Defines for vector manipulation for ease of reading and
 *automatic typecasting
 *Where applicable, the result variable is set after each function call*/
#define NewSymbolTable(TheSymbolTable) TheSymbolTable = InitVector()
#define InsertSymbol(TheSymbolTable, sym) TheSymbolTable->result = AddToVector(TheSymbolTable, (void*)sym)
#define TableLookupByIndex(TheSymbolTable, index) (char*)GetFromVector(TheSymbolTable,index)
#define EmptyTable(TheSymbolTable) DestroyVector(TheSymbolTable)
#define IsSymbolInTable(TheSymbolTable, sym) ItemExistsInVector(TheSymbolTable, (void*)sym)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/






/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ReadySymbolTable
    Readies the symbol table and adds the first two entries
    @param  port           -- The port number to bind the listen socket
    @return                -- 0 if fails, 1 if success
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int ReadySymbolTable();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Lexan
    Lexical analyzer. Returns a token type or an int corresponding to a char
    @return                -- The token or char value
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Lexan();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenFileStream
    Opens the file stream
    @param  fileName       -- The name of the file to open
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int OpenFileStream(const char*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   PrintSyntaxError
    Accepts an ErrorMessage enumerated type and prints an error
    @param  errorMessage   -- The enumerated ErrorMessage type to print
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrintSyntaxError(ErrorMessage);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindSymbol
    Extracts the entire symbol from input
    @return                -- Returns ID, BEGIN, or END
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindSymbol();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   FindDigit
    Extracts the entire number
    @return                -- NUM
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int FindDigit();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Match
    Verifies the correct lookahead type
    @param  type           -- The expected type
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Match(int);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   AssignStatement
    Handles equals sign
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AssignStatement();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Expression
    Handles + and - operations
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Expression();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Term
    Handles multiplication and division
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Term();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   Factor
    Handles matching of ID, NUM, and parentheses
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Factor();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   PrintSymbols
    Prints the found symbols
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrintSymbols();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleEndLine
    Gracefully handles end of line statements
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void HandleEndLine();

void PrintNextOperand(const char*);

void DoArithmetic(const char*);

void ExtractSymbol(char *symbol, int *underScoreCount, int *symbolSize);

void InitRegisterStruct();
#endif