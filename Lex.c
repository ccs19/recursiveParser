#include <stdio.h>
#include <ctype.h>

#include <malloc.h>
#include <stdlib.h>

#include "Lex.h"
#include "SymbolTable.h"


//Globals
FILE *m_file;               //File pointer
SymbolTable* symbolTable;   //Symbol Table
int m_lineNumber = 1;       //Current line number


//Constants
const int MAX_SYMBOL_SIZE = 256;


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
    for(i = 0; i < symbolTable->size; i++)
        printf("\n%s ", TableLookupByIndex(symbolTable,i));
    FreeSymbolTable(symbolTable); //We're done! Good times were had by all.
    PrintSyntaxError(IllegalIdentifier);

    return 0;
}
/*===========================================================================*/
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
        AddToSymbolTable(symbolTable, "-");
        AddToSymbolTable(symbolTable, "begin");
        AddToSymbolTable(symbolTable, "end");
        return 1;
    }
}
/*
procedure lexan

begin

while(true)? {
?ch = getchar();
?if (ch is a space or ch is a
tab)
????? ;   // do nothing

? else if (ch is a
        newline)
???increment lineno;
?else if (ch is a digit)
???begin

        get the number into number
???  return NUM;
end

else if (ch is a letter)

begin
???  get the identifier into value

        pos = find(value);

if (pos == NOT_FOUND)

else if (pos == 1)

else if(pos == 2)

return ID;

?  end
?else if (ch == EOF)
???return DONE;

? else
???return ch

endwhile
*/
/*===========================================================================*/

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
            int decimalCount = 0;
            while(isdigit(nextChar = fgetc(m_file)) || nextChar == '.') //THIS MIGHT NOT WORK
            {
                if(nextChar == '.') decimalCount++;
                if(decimalCount > 1) PrintSyntaxError(IllegalNumber); //if multiple decimal points found, invalid number
            }
            return NUM;
        }
        else if(isalpha(nextChar)) //If character
        {

        }
    }

    fclose(m_file);
    return 0;
}
/*===========================================================================*/

int OpenFileStream() //TODO add argument from argv
{
    m_file = fopen("./temp.txt", "r");
    return m_file == NULL ? 0 : 1; //If failed to open file, return 0, else 1;
}
/*===========================================================================*/

char *GetSymbol(FILE* m_file)
{
    ungetc(1, m_file);
    return 0;
}



/*===========================================================================*/
void PrintSyntaxError(ErrorMessage errorMessage)
{
    printf("\nLine %d contains error ",m_lineNumber);
    switch(errorMessage)
    {
        case MissingSquareBracket:
            printf("\'Missing square bracket\'");
            break;
        case MissingParend:
            printf("\'Parend Mismatch\'");
            break;
        case IllegalNumber:
            printf("\'Illegal Number\'");
            break;//Too many decimal Places
        case IllegalIdentifier:
            printf("\'Illegal Identifier\'");
            break;
        case UnterminatedComment:
            printf("\'Unterminated Comment\'");
            break;
        case SymbolBufferOverflow:
            printf("\'Symbol size exceeds max of %d\'", MAX_SYMBOL_SIZE);
            break;
        default:
            printf("\'Unknown Error\'");
            break;
    }
    fflush(stdout);
    exit(1);
}


/*===========================================================================*/

//TODO TEST THIS CODE!!!!!
void FindSymbol(char* nextChar)
{
    int underScoreCount = 0; //Number of consecutive underscores
    int symbolSize = 0;      //Size of symbol
    char symbol[MAX_SYMBOL_SIZE]; //Symbol array
    int position = -1;          //Location of symbol in table

    symbol[symbolSize] = nextChar;
    while(isalpha(nextChar = fgetc(m_file)) || nextChar == '_') //
    {
        if(nextChar == '_') underScoreCount++;
        else underScoreCount = 0;
        if(underScoreCount > 1) PrintSyntaxError(IllegalIdentifier); //If consecutive underscores, invalid
        if(symbolSize == MAX_SYMBOL_SIZE-1) PrintSyntaxError(SymbolBufferOverflow); //If no room for null char
        symbol[symbolSize++] = nextChar;
    }
    symbol[symbolSize] = '\0'; //Insert null char

}