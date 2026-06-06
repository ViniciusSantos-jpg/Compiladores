#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
	hasErrors = false; 
    lastErrorLine = 0;
    linhaAnterior = 1;
}

void Parser::advance()
{
    linhaAnterior = scanner->getLine(); // Salva a linha antes de avançar
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
    // Verifica se é 'void' ou um 'Type' (int, char)
    if (lToken->name == INT || lToken->name == CHAR) {
        type();
    } else if (lToken->name == VOID) {
        match(VOID); // Consome o 'void'
    } else {
        error("Esperado tipo de retorno ou void");
    }

    match(ID);           // Nome da função
    match(LPARENTHESE);  // (
    paramTypes();        // ParamTypes
    match(RPARENTHESE);  // )
    match(LBRACE);       // {

    // (Type VarDeclaration(, VarDeclaration)*;)*
    while (lToken->name == INT || lToken->name == CHAR) {
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
    if (lToken->name == INT || lToken->name == CHAR) {
        advance();
    } else {
        error("Esperado tipo 'int' ou 'char'");
        synchronize(); 
    }
}

void Parser::error(string str)
{
    // Pega a linha atual do Scanner
    int linhaDoErro = scanner->getLine();
    
    // Se o erro for a falta de um ponto e vírgula, o erro real foi na linha de cima!
    if (str == "esperado ';'") {
        linhaDoErro = linhaAnterior;
    }
    
    // Só imprime se o erro for numa linha diferente do último erro
    if (linhaDoErro != lastErrorLine) {
        
        // Mensagem limpa: "linha X: erro: Y"
        cout << "linha " << linhaDoErro << ": erro: " << str << endl;
        
        lastErrorLine = linhaDoErro; // Atualiza a última linha que deu erro
    }
    
    hasErrors = true;
}


void Parser::paramTypes()
{
    if (lToken->name == VOID) {
        match(VOID);
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
    if (lToken->name == IF) {
        match(IF);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();
        if (lToken->name == ELSE) {
            match(ELSE);
            statement();
        }
    } else if (lToken->name == WHILE) {
        match(WHILE);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();
    } else if (lToken->name == FOR) {
        match(FOR);
        match(LPARENTHESE);
        if (lToken->name == ID) assign(); // Simplificado
        match(SEMICOLON);
        if (lToken->name != SEMICOLON) expression();
        match(SEMICOLON);
        if (lToken->name != RPARENTHESE) assign();
        match(RPARENTHESE);
        statement();
    } else if (lToken->name == RETURN) {
        match(RETURN);
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
        advance();
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
            advance();
            synchronize(); 
        }
}

void Parser::synchronize()
{
    while (lToken->name != END_OF_FILE) {
        if (lToken->name == SEMICOLON || lToken->name == RBRACE || lToken->name == LBRACE) {
            return;
        }
        
        // Verifica se é uma palavra reservada que inicia bloco
        if (lToken->name == INT || lToken->name == CHAR || lToken->name == VOID || 
            lToken->name == IF || lToken->name == WHILE || lToken->name == FOR || lToken->name == RETURN) {
            return;
        }
        advance(); 
    }
}

void Parser::match(int t)
{
    if (lToken->name == t || lToken->attribute == t) {
        advance();
    } else {
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