#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct{
    int result;     //Sets various function return values
    int size;       //Number of items currently in symbol table
    int capacity;   //Max size of symbol table
    void** items;   //Item in vector
} Vector;

/*
    FUNCTION: InitVector
    Allocates memory for a new vector
    @return Pointer to initialized vector
 */
Vector *InitVector();


/*  FUNCTION: AddToVector
    Adds a symbol to vector. On success, 1 is returned
    @param  item        -- item to add
    @param  vector      -- Vector to add symbol to
    @return             -- 1 on success
*/
int AddToVector(Vector*, void*);




/*  FUNCTION: DestroyVector
    Frees memory alloc'd to vector
    vector -- The vector table to free*/
void DestroyVector(Vector*);


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
void ItemExistsInVector(Vector*, void*);


/*FUNCTION: GetFromVector
    Returns a pointer to an item from a vector based on an index
    @param Vector to search
    @param Index to look into
    @return the item on success, 0 on failure
 */
void *GetFromVector(Vector*, int);

#endif