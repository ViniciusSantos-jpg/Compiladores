#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include "token.h"
#include "tabelaDeSimbolos.h" 

class Scanner 
{
    private: 
        string input; 
        size_t pos;  
        int line;     
        TabelaDeSimbolos* st; 
    
    public:
        Scanner(string);
        int getLine();
        Token* nextToken();        
        void lexicalError(string);
};

#endif