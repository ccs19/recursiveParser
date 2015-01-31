#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "SymbolTable.h"

//Prototypes
inline static int DoubleVectorSize(Vector *);

//Constants
const int DEFAULT_SYMBOL_TABLE_SIZE = 64;

//symbolTable -- pointer to symbolTable to initialize
Vector *InitVector()
{
    Vector * symbolTable = malloc(sizeof(Vector));
    if(symbolTable == NULL)
        return 0;
    else
    {
        symbolTable->result = 0;
        symbolTable->size = 0;
        symbolTable->capacity = DEFAULT_SYMBOL_TABLE_SIZE;
        symbolTable->symbols = malloc(sizeof(void*) * symbolTable->capacity);
        return symbolTable;
    }
} //Complete



/*symbol -- Symbol to add
symbolTable -- Vector to add symbol to
*/
int AddToVector(Vector *symbolTable, void *symbol)
{
    int success = 1; //Assume result
    if(symbolTable->capacity == symbolTable->size) //Increase size if full
    {
        success = DoubleVectorSize(symbolTable);
    }
    symbolTable->symbols[symbolTable->size++] = symbol;
    return success;
} //Complete

/*Frees memory alloc'd to symbol table
symbolTable -- The symbol table to free*/
void DestroyVector(Vector *symbolTable)
{
    free(symbolTable->symbols);
}//TODO Check for memory leaks

/*
Checks if a specific value is present in the symbol table
For efficiency, in this program we scan from "top to bottom"
with the top being the last element

If item is found, its index is set in the Vector->result parameter
If item is not found, Vector->result is set to -1
 */
void ItemExistsInVector(Vector *symbolTable, void *symbol)
{
    int vectorSize = symbolTable->size-1;
    symbolTable->result = -1; //If not found, result is -1
    while(vectorSize > 0)
    {
        char* str1 = (char*) GetFromVector(symbolTable, vectorSize);
        if(strcmp(str1, (char*)symbol) == 0)
        {
            symbolTable->result = vectorSize;
            return;
        }
        else
            vectorSize--;
    }

}


void* GetFromVector(Vector *symbolTable, int index)
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
inline static int DoubleVectorSize(Vector *symbolTable)
{
    void **symbols = realloc(symbolTable->symbols, sizeof(void*) * symbolTable->capacity * 2);
    if(symbols != NULL)
    {
        symbolTable->symbols = symbols; //Assign realloc'd symbol table to new symbol table
        symbolTable->capacity *= 2; //Double symbol table size
        return 1;
    }
    else
        return 0;
}//Complete