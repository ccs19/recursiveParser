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

/*Defines for adding to SymbolTable for ease of reading and
 *automatic typecasting
 *Where applicable, the m_success variable is set after each function call*/
#define NewSymbolTable(TheSymbolTable) TheSymbolTable = InitSymbolTable()
#define InsertSymbolInSymbolTable(TheSymbolTable, sym) TheSymbolTable->success = AddToSymbolTable(TheSymbolTable, (void*)sym)
#define TableLookupByIndex(TheSymbolTable, index) (char*)GetFromSymbolTable(TheSymbolTable,index)
#define EmptyTable(TheSymbolTable) FreeSymbolTable(TheSymbolTable)
#define IsSymbolInTable(TheSymbolTable, sym) TheSymbolTable->success = SymbolExistsInTable(TheSymbolTable, (void*)sym)



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct{
    int success; //If function calls for symbol table are successful
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


int SymbolExistsInTable(SymbolTable* , void* );

void *GetFromSymbolTable(SymbolTable* , int );




#endif