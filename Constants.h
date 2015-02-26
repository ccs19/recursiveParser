#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "SymbolTable.h"

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

#define OPERATOR_SIZE 2

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


#endif

