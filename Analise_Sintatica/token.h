#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    ID,
    INTEGER_CONSTANT,
    CHAR_CONSTANT,
    STRING_CONSTANT,
    // Operadores
    PLUS, MINUS, MULT, DIV,
    ASSIGN, // '='
    EQ,     // '=='
    NEQ,    // '!='
    LTE,    // '<='
    LT,     // '<'
    GTE,    // '>='
    GT,     // '>'
    AND,    // '&&'
    OR,     // '||'
    NOT,    // '!'
    // Separadores
    LPARENTHESE, RPARENTHESE, // ( )
    LBRACE, RBRACE,           // { }
    LBRACKET, RBRACKET,       // [ ]
    COMMA, SEMICOLON,         // , ;
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};

#endif