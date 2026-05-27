#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
    while (lToken->name != END_OF_FILE)
        function();

    match(END_OF_FILE);
}

void Parser::function()
{
    // Regra 2: Type ID (ParamTypes) { ... } ou void ID (ParamTypes) { ... }
    
    // Verifica se é 'void' ou um 'Type' (int, char)
    if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "char")) {
        type();
    } else if (lToken->name == ID && lToken->lexeme == "void") {
        match(ID); // Consome o 'void'
    } else {
        error("Esperado tipo de retorno ou void");
    }

    match(ID);           // Nome da função
    match(LPARENTHESE);  // (
    paramTypes();        // ParamTypes
    match(RPARENTHESE);  // )
    match(LBRACE);       // {

    // (Type VarDeclaration(, VarDeclaration)*;)*
    while (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "char")) {
        type();
        varDeclaration();
        while (lToken->name == COMMA) {
            match(COMMA);
            varDeclaration();
        }
        match(SEMICOLON);
    }

    // (Statement)*
    while (lToken->name != RBRACE && lToken->name != END_OF_FILE) {
        statement();
    }

    match(RBRACE);       // }
}

void Parser::type()
{
    // Regra 4: Type -> int | char
    if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "char")) {
        match(ID);
    } else {
        error("Esperado tipo 'int' ou 'char'");
    }
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
