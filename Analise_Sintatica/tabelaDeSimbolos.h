#ifndef TABELADESIMBOLOS_H
#define TABELADESIMBOLOS_H

#include <map>
#include <string>

using namespace std;

class TabelaDeSimbolos 
{
    private:
        map<string, int> st; // Dicionário que mapeia a string para o código do Token

    public:
        TabelaDeSimbolos();
        void insert(string lexeme, int tokenName);
        int find(string lexeme);
};

#endif