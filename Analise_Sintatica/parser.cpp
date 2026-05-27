#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
	hasErrors = false; // Inicializa sem erros
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void Parser::run()
{
	advance();	
	program();
	
	// Só imprime sucesso se a flag de erros continuar falsa
	if (!hasErrors) {
		cout << "Compilação encerrada com sucesso!\n";
	} else {
		cout << "\nCompilação encerrada com erros estruturais. Verifique as mensagens acima.\n";
	}
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
    if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "char")) {
        match(ID);
    } else {
        error("Esperado tipo 'int' ou 'char'");
        synchronize(); // RECUPERA O ERRO AQUI!
    }
}

void Parser::error(string str)
{
    string tokenStr = lToken->lexeme;
    
    // Se o token for um operador ou símbolo que não salva lexema na Etapa 1, definimos o texto manualmente
    if (tokenStr.empty()) {
        switch(lToken->name) {
            case EQ: tokenStr = "=="; break;
            case NEQ: tokenStr = "!="; break;
            case ASSIGN: tokenStr = "="; break;
            case PLUS: tokenStr = "+"; break;
            case MINUS: tokenStr = "-"; break;
            case MULT: tokenStr = "*"; break;
            case DIV: tokenStr = "/"; break;
            case SEMICOLON: tokenStr = ";"; break;
            case COMMA: tokenStr = ","; break;
            case LPARENTHESE: tokenStr = "("; break;
            case RPARENTHESE: tokenStr = ")"; break;
            case LBRACE: tokenStr = "{"; break;
            case RBRACE: tokenStr = "}"; break;
            case LBRACKET: tokenStr = "["; break;
            case RBRACKET: tokenStr = "]"; break;
            case END_OF_FILE: tokenStr = "fim de arquivo"; break;
            default: tokenStr = "token desconhecido"; break;
        }
    }
    
    // Padrão de saída do g++
    cout << "linha " << scanner->getLine() << ": erro: " << str 
         << " antes de '" << tokenStr << "'" << endl;
    
    hasErrors = true;
}

void Parser::paramTypes()
{
    // Regra 5: ParamTypes -> void | Type ID ([])? (, Type ID ([])?)*
    if (lToken->name == ID && lToken->lexeme == "void") {
        match(ID);
    } else {
        type();
        match(ID);
        if (lToken->name == LBRACKET) {
            match(LBRACKET);
            match(RBRACKET);
        }
        while (lToken->name == COMMA) {
            match(COMMA);
            type();
            match(ID);
            if (lToken->name == LBRACKET) {
                match(LBRACKET);
                match(RBRACKET);
            }
        }
    }
}

void Parser::statement()
{
    // Regra 6: Statement
    if (lToken->name == ID && lToken->lexeme == "if") {
        match(ID);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();
        if (lToken->name == ID && lToken->lexeme == "else") {
            match(ID);
            statement();
        }
    } else if (lToken->name == ID && lToken->lexeme == "while") {
        match(ID);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();
    } else if (lToken->name == ID && lToken->lexeme == "for") {
        match(ID);
        match(LPARENTHESE);
        if (lToken->name == ID && lToken->lexeme != "int" && lToken->lexeme != "char") assign();
        match(SEMICOLON);
        if (lToken->name != SEMICOLON) expression();
        match(SEMICOLON);
        if (lToken->name != RPARENTHESE) assign();
        match(RPARENTHESE);
        statement();
    } else if (lToken->name == ID && lToken->lexeme == "return") {
        match(ID);
        if (lToken->name != SEMICOLON) {
            expression();
        }
        match(SEMICOLON);
    } else if (lToken->name == LBRACE) {
        match(LBRACE);
        while (lToken->name != RBRACE && lToken->name != END_OF_FILE) {
            statement();
        }
        match(RBRACE);
    } else if (lToken->name == SEMICOLON) {
        match(SEMICOLON);
    } else if (lToken->name == ID) {
        assign(); 
        match(SEMICOLON);
    } else {
        error("Statement invalido");
    }
}

void Parser::assign()
{
    // Regra 7: Assign -> ID ([Expression])? = Expression
    match(ID);
    
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        expression();
        match(RBRACKET);
    }
    
    match(ASSIGN); // Consome o '=', se falhar, o match resolve o erro automaticamente!
    expression();
}

void Parser::expression()
{
    // Regra 8 (Fatorada para evitar loop infinito): Expression -> Term (Op Term)*
    term();
    while (lToken->name == PLUS || lToken->name == MINUS || lToken->name == MULT || lToken->name == DIV ||
           lToken->name == EQ || lToken->name == NEQ || lToken->name == LTE || lToken->name == LT || 
           lToken->name == GTE || lToken->name == GT || lToken->name == AND || lToken->name == OR) {
        advance(); // Consome o operador binário, relacional ou lógico
        term();
    }
}

void Parser::term()
{
    if (lToken->name == NOT) {
        match(NOT);
        expression();
    } else if (lToken->name == LPARENTHESE) {
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
    } else if (lToken->name == INTEGER_CONSTANT || lToken->name == CHAR_CONSTANT || lToken->name == STRING_CONSTANT) {
        advance();
    } else if (lToken->name == ID) {
        match(ID);
        if (lToken->name == LPARENTHESE) { // Chamada de função
            match(LPARENTHESE);
            if (lToken->name != RPARENTHESE) {
                expression();
                while (lToken->name == COMMA) {
                    match(COMMA);
                    expression();
                }
            }
            match(RPARENTHESE);
        } else if (lToken->name == LBRACKET) { // Acesso a array
            match(LBRACKET);
            expression();
            match(RBRACKET);
        }
    } else {
        error("expressao invalida");
        synchronize(); // Adicionado para recuperar erros dentro de expressões
    }
}

void Parser::synchronize()
{
    while (lToken->name != END_OF_FILE) {
        if (lToken->name == SEMICOLON || lToken->name == RBRACE || lToken->name == LBRACE) {
            return;
        }
        
        if (lToken->name == ID) {
            string lex = lToken->lexeme;
            if (lex == "int" || lex == "char" || lex == "void" || 
                lex == "if" || lex == "while" || lex == "for" || lex == "return") {
                return;
            }
        }
        advance(); // Descarta o token problemático e avança para o próximo
    }
}

void Parser::match(int t)
{
    if (lToken->name == t || lToken->attribute == t) {
        advance();
    } else {
        // O match agora sabe exatamente o que faltou e manda a mensagem pronta
        if (t == SEMICOLON) {
            error("esperado ';'");
        } else if (t == RPARENTHESE) {
            error("esperado ')'");
        } else if (t == LBRACE) {
            error("esperado '{'");
        } else if (t == RBRACE) {
            error("esperado '}'");
        } else if (t == ASSIGN) {
            error("esperado '='");
        } else if (t == ID) {
            error("esperado identificador");
        } else {
            error("sintaxe invalida");
        }
        synchronize();
    }
}

void Parser::varDeclaration()
{
    // Regra 3: VarDeclaration -> ID ([integerconstant])?
    match(ID);
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        match(INTEGER_CONSTANT);
        match(RBRACKET);
    }
}