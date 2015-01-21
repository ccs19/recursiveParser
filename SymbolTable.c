#include <stdio.h>
#include <malloc.h>
#include "SymbolTable.h"

//Prototypes
inline static int DoubleSymbolTableSize(SymbolTable*);

//Constants
const int DEFAULT_SYMBOL_TABLE_SIZE = 100;

//symbolTable -- pointer to symbolTable to initialize
SymbolTable* InitSymbolTable()
{
    SymbolTable* symbolTable = malloc(sizeof(SymbolTable));
    if(symbolTable == NULL)
        return 0;
    else
    {
        symbolTable->size = 0;
        symbolTable->capacity = DEFAULT_SYMBOL_TABLE_SIZE;
        symbolTable->symbols = malloc(sizeof(void*) * symbolTable->capacity);
        return symbolTable;
    }
} //Complete



/*symbol -- Symbol to add
symbolTable -- SymbolTable to add symbol to
*/
int AddToSymbolTable(SymbolTable* symbolTable, void* symbol)
{
    int success = 1; //Assume success
    if(symbolTable->capacity == symbolTable->size) //Increase size if full
    {
        success = DoubleSymbolTableSize(symbolTable);
    }
    symbolTable->symbols[symbolTable->size++] = symbol;
    return success;
} //Complete

/*Frees memory alloc'd to symbol table
symbolTable -- The symbol table to free*/
void FreeSymbolTable(SymbolTable* symbolTable)
{
    free(symbolTable->symbols);
}//TODO Check for memory leaks

/*
Checks if a specific value is present in the symbol table
 */
int IsInSymbolTable(SymbolTable* symbolTable, void* symbol)
{
    //TODO Fill in return values
    return 1;
}


void* GetFromSymbolTable(SymbolTable* symbolTable, int index)
{
    if (index < 0 || index > symbolTable->size)
    {
        return 0;
    }
    else
    {
        return symbolTable->symbols[index];
    }
}//Should be complete


//Symbol Table to double in size.
//This should not be called from outside this implementation.
inline static int DoubleSymbolTableSize(SymbolTable* symbolTable)
{
    void **symbols = realloc(symbolTable->symbols, sizeof(void*) * symbolTable->capacity * 2);
    if(symbols != NULL)
    {
        symbolTable->symbols = symbols;
        symbolTable->capacity *= 2;
        return 1;
    }
    else
        return 0;
}//Complete