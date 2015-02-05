#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "SymbolTable.h"

//Prototypes
inline static int DoubleVectorSize(Vector *);

//Constants
const int DEFAULT_SYMBOL_TABLE_SIZE = 64;

/*
    FUNCTION: InitVector
    Allocates memory for a new vector
    @return Pointer to initialized vector
 */
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
}



/*  FUNCTION: AddToVector
    Adds a symbol to vector. On success, 1 is returned
    @param  item        -- item to add
    @param  vector      -- Vector to add item to
    @return             -- 1 on success
*/
int AddToVector(Vector *vector, void *item)
{
    int success = 1; //Assume result
    if(vector->capacity == vector->size) //Increase size if full
    {
        success = DoubleVectorSize(vector);
    }
    vector->items[vector->size++] = item;
    return success;
}

/*  FUNCTION: DestroyVector
    Frees memory alloc'd to vector
    vector -- The vector table to free*/
void DestroyVector(Vector *vector)
{
    int i;
    for(i = vector->size-1; i > 0; i--) {
        free(vector->items[i]);
    }
    free(vector->items);
    free(vector);
}

/*
    FUNCTION: ItemExistsInVector
    Checks if a specific item is present in the vector
    For efficiency, in this program we scan from "top to bottom"
    with the top being the last element

    If item is found, its index is set in the Vector->result parameter
    If item is not found, Vector->result is set to -1

    @param vector   --  The vector to examine
    @param item   --  The item to look for
 */
void ItemExistsInVector(Vector *vector, void *item)
{
    int vectorSize = vector->size-1;
    vector->result = -1; //If not found, result is -1
    while(vectorSize > 0)
    {
        char* str1 = (char*) GetFromVector(vector, vectorSize);
        if(strcmp(str1, (char*)item) == 0)
        {
            vector->result = vectorSize;
            return;
        }
        else
            vectorSize--;
    }
}

/*FUNCTION: GetFromVector
    Returns a pointer to an item from a vector based on an index
    @param Vector to search
    @param Index to look into
    @return the item on success, 0 on failure
 */
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


/*FUNCTION: DoubleVectorSize
    Doubles a vector's size.
    @param Vector to double in size
    @return 1 on success, 0 on failure
 */
inline static int DoubleVectorSize(Vector *vector)
{
    void **symbols = realloc(vector->items, sizeof(void*) * vector->capacity * 2);
    if(symbols != NULL)
    {
        vector->items = symbols; //Assign realloc'd symbol table to new symbol table
        vector->capacity *= 2; //Double symbol table size
        return 1;
    }
    else
        return 0;
}