#include "SymbolTable.h"

#ifndef LEX_H
#define LEX_H


#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401
#define DONE 402


enum errmsg{
    MissingSquareBracket,   //Missing bracket
    MissingParen,           //Mismatch parenthesis
    IllegalNumber,          //Too many decimal Places
    IllegalIdentifier,      //Unknown identifier, e.g. %$# or consecutive underscores
    UnterminatedComment,    //Comment not properly terminated
    SymbolBufferOverflow,   //If symbol length exceeds maximum symbol size
    SyntaxError,            //Generic syntax error
    UnexpectedAssignment,   //Invalid assignment
    InvalidLineEnding       //If improperly terminated line
};
typedef enum errmsg ErrorMessage;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Defines for vector manipulation for ease of reading and
 *automatic typecasting
 *Where applicable, the m_success variable is set after each function call*/
#define NewSymbolTable(TheSymbolTable) TheSymbolTable = InitVector()
#define InsertSymbol(TheSymbolTable, sym) TheSymbolTable->result = AddToVector(TheSymbolTable, (void*)sym)
#define TableLookupByIndex(TheSymbolTable, index) (char*)GetFromVector(TheSymbolTable,index)
#define EmptyTable(TheSymbolTable) DestroyVector(TheSymbolTable)
#define IsSymbolInTable(TheSymbolTable, sym) ItemExistsInVector(TheSymbolTable, (void*)sym)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#define BEGIN_INDEX 1
#define END_INDEX 2
#define NOT_IN_TABLE -1

int ReadySymbolTable();

int Lexan();

int OpenFileStream(const char*);

void PrintSyntaxError(ErrorMessage);

int FindSymbol();

int FindDigit();

void Match(int);

void AssignStatement();

void Expression();

void Term();

void Factor();

void PrintSymbols();

void HandleEndLine();
#endif