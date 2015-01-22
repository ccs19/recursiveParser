#include <stdio.h>
#include <ctype.h>

#include <malloc.h>

#include "Lex.h"
#include "SymbolTable.h"


//Globals
FILE *m_file;               //File pointer
SymbolTable* symbolTable;   //Symbol Table
int m_lineNumber = 1;       //Current line number





int main(int argc, const char* argv[]) {
    if(!ReadySymbolTable()) //This should only fail if malloc fails
    {
        printf("Failed to initialize SymbolTable. Exiting.\n");
    }
    else if(!OpenFileStream()) //We failed to open file
    {
        printf("Failed to open file.");
    }
    else
    {

        //TODO Add implementation
    }
    int i;

    InsertSymbolInSymbolTable(symbolTable, "Thisisisisis");
    printf("Success = %d ", symbolTable->success);
    for(i = 0; i < symbolTable->size; i++)
        printf("%s ", TableLookupByIndex(symbolTable,i));
    FreeSymbolTable(symbolTable); //We're done! Good times were had by all.


    printf("End of program");


    return 0;
}

int ReadySymbolTable()
{
    NewSymbolTable(symbolTable);
    //symbolTable = InitSymbolTable();
    if(symbolTable == NULL) // Failed to allocate memory if this fails
    {
        return 0;
    }
    else
    {
        AddToSymbolTable(symbolTable, "begin");
        AddToSymbolTable(symbolTable, "end");
        return 1;
    }
}




int Lexan()
{
    int nextChar;

    while( (nextChar = fgetc(m_file)) != EOF )
    {
        if(nextChar == ' ' || nextChar == '\t')
        {
            //do nothing if space or tab
        }
        else if(nextChar == '\n')
        {
            m_lineNumber++; //If newline increase line count.
        }
        else if(isdigit(nextChar)) //If digit
        {

            return NUM;
        }
        else if(isalpha(nextChar)) //If character
        {
            //ungetc(nextChar); //TODO get the symbol
            //char *symbol = GetSymbol(m_file, nextChar);
        }
    }

    fclose(m_file);
    return 0;
}


int OpenFileStream() //TODO add argument from argv
{
    m_file = fopen("./temp.txt", "r");
    return m_file == NULL ? 0 : 1; //If failed to open file, return 0, else 1;
}


char *GetSymbol(FILE* m_file)
{
    ungetc(1, m_file);
    return 0;
}