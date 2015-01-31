#include "SymbolTable.h"

#ifndef LEX_H
#define LEX_H


#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401


enum errmsg{
    MissingSquareBracket, //Missing bracket
    MissingParend,        //Mismatch parenthesis
    IllegalNumber,        //Too many decimal Places
    IllegalIdentifier,    //Unknown identifier, e.g. %$# or consecutive underscores
    UnterminatedComment,  //Comment not properly terminated
    SymbolBufferOverflow  //If symbol length exceeds maximum symbol size
};
typedef enum errmsg ErrorMessage;


int ReadySymbolTable();

int Lexan();

int OpenFileStream();

int InsertIntoSymbolTable(char* symbol);

char *GetSymbol(FILE* m_file);

void PrintSyntaxError(ErrorMessage);

void FindSymbol(char*);
#endif