#ifndef SYMBOL_TABLEC_H
#define SYMBOL_TABLEC_H


#include <stddef.h> //For size_t type


#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401


//NOTE: This is just a vector. The struct has a void ptr
//so we can use whatever type we want. I just wanted to write
//a resizeable "array" on my own that I could re-use whenever
//I wanted a dynamically resizeable type. Credit goes to Google searches.




typedef struct{
    int size; //Number of items currently in symbol table
    int capacity; //Max size of symbol table
    void** symbols; //Symbol
} SymbolTable;

//symbolTable -- pointer to symbolTable to initialize
//return 1 on success, else failure
SymbolTable* InitSymbolTable();

//symbol -- Symbol to add
//symbolTable -- SymbolTable to add symbol to
//size -- size of item to add to table
//return 1 on success, else failure
int AddToSymbolTable(SymbolTable*, void*);

//Frees memory alloc'd to symbol table
//symbolTable -- The symbol table to free
void FreeSymbolTable(SymbolTable* );


int IsInSymbolTable(SymbolTable* , void* );

void *GetFromSymbolTable(SymbolTable* , int );




#endif