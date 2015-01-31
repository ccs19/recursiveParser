#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

//NOTE: This is just a vector. The struct has a void ptr
//so we can use whatever type we want. I just wanted to write
//a resizeable "array" on my own that I could re-use whenever
//I wanted a dynamically resizeable type. Credit goes to Google searches
//and experimentation

/*~~~~~~~~~~~~~~~~*/

#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401

/*~~~~~~~~~~~~~~~~*/




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*Defines for adding to Vector for ease of reading and
 *automatic typecasting
 *Where applicable, the m_success variable is set after each function call*/
#define NewSymbolTable(TheSymbolTable) TheSymbolTable = InitVector()
#define InsertSymbol(TheSymbolTable, sym) TheSymbolTable->result = AddToVector(TheSymbolTable, (void*)sym)
#define TableLookupByIndex(TheSymbolTable, index) (char*)GetFromVector(TheSymbolTable,index)
#define EmptyTable(TheSymbolTable) DestroyVector(TheSymbolTable)
#define IsSymbolInTable(TheSymbolTable, sym) ItemExistsInVector(TheSymbolTable, (void*)sym)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct{
    int result;    //Sets various function return values
    int size;       //Number of items currently in symbol table
    int capacity;   //Max size of symbol table
    void** symbols; //Symbol
} Vector;

//symbolTable -- pointer to symbolTable to initialize
//return 1 on result, else failure
Vector *InitVector();

//symbol -- Symbol to add
//symbolTable -- Vector to add symbol to
//size -- size of item to add to table
//return 1 on result, else failure
int AddToVector(Vector *, void *);

//Frees memory alloc'd to symbol table
//Vector -- The vector to free
void DestroyVector(Vector *);

void ItemExistsInVector(Vector *, void *);

void *GetFromVector(Vector *, int);




#endif