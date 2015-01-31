#include "SymbolTable.h"

#ifndef LEX_H
#define LEX_H


#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401
#define DONE 402


enum errmsg{
    MissingSquareBracket, //Missing bracket
    MissingParen,        //Mismatch parenthesis
    IllegalNumber,        //Too many decimal Places
    IllegalIdentifier,    //Unknown identifier, e.g. %$# or consecutive underscores
    UnterminatedComment,  //Comment not properly terminated
    SymbolBufferOverflow,  //If symbol length exceeds maximum symbol size
    SyntaxError
};
typedef enum errmsg ErrorMessage;


#define BEGIN_INDEX 1
#define END_INDEX 2
#define NOT_IN_TABLE -1

int ReadySymbolTable();

int Lexan();

int OpenFileStream();

void PrintSyntaxError(ErrorMessage);

int FindSymbol(int);

void Match(int);
#endif