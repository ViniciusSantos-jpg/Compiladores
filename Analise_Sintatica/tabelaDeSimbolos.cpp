#include "tabelaDeSimbolos.h"
#include "token.h"

TabelaDeSimbolos::TabelaDeSimbolos()
{
    // Insere todas as palavras reservadas da linguagem C-- na tabela de símbolos
    insert("int", INT);
    insert("char", CHAR);
    insert("void", VOID);
    insert("if", IF);
    insert("else", ELSE);
    insert("while", WHILE);
    insert("for", FOR);
    insert("return", RETURN);
}

void TabelaDeSimbolos::insert(string lexeme, int tokenName)
{
    st[lexeme] = tokenName;
}

int TabelaDeSimbolos::find(string lexeme)
{
    // Se encontrar a palavra na tabela, retorna o código do Token
    if (st.find(lexeme) != st.end()) {
        return st[lexeme];
    }
    // Se não encontrar, retorna UNDEF (0) para sabermos que é um ID normal
    return UNDEF;
}