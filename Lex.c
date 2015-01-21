#include <stdio.h>
#include <ctype.h>

#include <malloc.h>

#include "SymbolTable.h"


//Globals
FILE *m_file; //File pointer
int m_lineNumber = 1; //Current line number
SymbolTable* symbolTable; //Symbol Table

//prototypes
int Lexan();
int OpenFileStream();
int InsertIntoSymbolTable(char*);
int ReadySymbolTable();



int main(int argc, const char* argv[]) {
    if(!ReadySymbolTable()) //This should only fail if malloc fails
    {
        printf("Failed to initialize SymbolTable. Exiting.\n");
    }
    else if(!OpenFileStream()) //If failed to open file
    {
        printf("Failed to open file.");
    }
    else
    {

        //TODO Add implementation
    }
    int i;



    for(i = 0; i < symbolTable->size; i++)
        printf("%s ", (char*)symbolTable->symbols[i]);
    FreeSymbolTable(symbolTable); //We're done! Good times were had by all.


    printf("End of program");


    return 0;
}

int ReadySymbolTable()
{
    symbolTable = InitSymbolTable();
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


int OpenFileStream()
{
    m_file = fopen("./temp.txt", "r");
    if(m_file == NULL)
        printf("File ptr is null \n");
    return 0; //TODO return 1 on success, 0 on failure.
}


int InsertIntoSymbolTable(char* symbol)
{
    return 1;
}

char *GetSymbol(FILE* m_file)
{
    ungetc(1, m_file);
    return 0;
}