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
    Vector * vector = malloc(sizeof(Vector));
    if(vector == NULL)
        return 0;
    else
    {
        vector->result = 0;
        vector->size = 0;
        vector->capacity = DEFAULT_SYMBOL_TABLE_SIZE;
        vector->items = malloc(sizeof(void*) * vector->capacity);
        return vector;
    }
} //Complete



/*symbol -- Symbol to add
symbolTable -- Vector to add symbol to
*/
int AddToVector(Vector *vector, void *symbol)
{
    int success = 1; //Assume result
    if(vector->capacity == vector->size) //Increase size if full
    {
        success = DoubleVectorSize(vector);
    }
    vector->items[vector->size++] = symbol;
    return success;
} //Complete

/*Frees memory alloc'd to symbol table
symbolTable -- The symbol table to free*/
void DestroyVector(Vector *symbolTable)
{
    free(symbolTable->items);
}//TODO Check for memory leaks

/*
Checks if a specific value is present in the symbol table
For efficiency, in this program we scan from "top to bottom"
with the top being the last element

If item is found, its index is set in the Vector->result parameter
If item is not found, Vector->result is set to -1
 */
void ItemExistsInVector(Vector *vector, void *symbol)
{
    int vectorSize = vector->size-1;
    vector->result = -1; //If not found, result is -1
    while(vectorSize > 0)
    {
        char* str1 = (char*) GetFromVector(vector, vectorSize);
        if(strcmp(str1, (char*)symbol) == 0)
        {
            vector->result = vectorSize;
            return;
        }
        else
            vectorSize--;
    }

}


void* GetFromVector(Vector *vector, int index)
{
    if (index < 0 || index > vector->size)
    {
        return 0;
    }
    else
    {
        return vector->items[index];
    }
}//Should be complete


//Symbol Table to double in size.
//This should not be called from outside this implementation.
inline static int DoubleVectorSize(Vector *symbolTable)
{
    void **symbols = realloc(symbolTable->items, sizeof(void*) * symbolTable->capacity * 2);
    if(symbols != NULL)
    {
        symbolTable->items = symbols; //Assign realloc'd symbol table to new symbol table
        symbolTable->capacity *= 2; //Double symbol table size
        return 1;
    }
    else
        return 0;
}//Complete