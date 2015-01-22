#include "SymbolTable.h"

#ifndef LEX_H
#define LEX_H


int ReadySymbolTable();

int Lexan();

int OpenFileStream();

int InsertIntoSymbolTable(char* symbol);

char *GetSymbol(FILE* m_file);


#endif