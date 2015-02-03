#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct{
    int result;     //Sets various function return values
    int size;       //Number of items currently in symbol table
    int capacity;   //Max size of symbol table
    void** items;   //Item in vector
} Vector;

//symbolTable -- pointer to symbolTable to initialize
//return 1 on result, else failure
Vector *InitVector();

//symbol -- Symbol to add
//symbolTable -- Vector to add symbol to
//size -- size of item to add to table
//return 1 on result, else failure
int AddToVector(Vector*, void*);

//Frees memory alloc'd to symbol table
//Vector -- The vector to free
void DestroyVector(Vector*);

void ItemExistsInVector(Vector*, void*);

void *GetFromVector(Vector*, int);

#endif