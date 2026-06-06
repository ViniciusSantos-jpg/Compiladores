#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    pos = 0;
    line = 1;
    st = new TabelaDeSimbolos();
    ifstream inputFile(input, ios::in);
    string fileLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, fileLine))
        {
            this->input.append(fileLine + '\n');
        }
        inputFile.close();
    }
    else 
    {
        cout << "Unable to open file\n"; 
    }
}

int Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    string lexeme = "";

    while (pos < input.length()) {
        char c = input[pos];

        // 1. Ignorar espaços em branco e contar linhas
        if (isspace(c)) {
            if (c == '\n') line++;
            pos++;
            continue;
        }

        // 2. Ignorar comentários (em linha // e em bloco /* */)
        if (c == '/' && pos + 1 < input.length()) {
            if (input[pos + 1] == '/') {
                while (pos < input.length() && input[pos] != '\n') {
                    pos++;
                }
                continue;
            } else if (input[pos + 1] == '*') {
                pos += 2;
                while (pos + 1 < input.length() && !(input[pos] == '*' && input[pos + 1] == '/')) {
                    if (input[pos] == '\n') line++;
                    pos++;
                }
                pos += 2; // Pula o "*/"
                continue;
            }
        }

        // 3. Identificadores (letter (letter | digit | _)*)
        if (isalpha(c)) {
            while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
                lexeme += input[pos++];
            }
            
            int tokenName = st->find(lexeme);
            if (tokenName != UNDEF) {
                // Se for palavra reservada, retorna o token dela (ex: IF, INT)
                return new Token(tokenName, lexeme);
            }
            
            // Se não for reservada, é um ID comum
            return new Token(ID, lexeme);
        }

        // 4. Constantes Numéricas (digit+)
        if (isdigit(c)) {
            while (pos < input.length() && isdigit(input[pos])) {
                lexeme += input[pos++];
            }
            return new Token(INTEGER_CONSTANT, lexeme);
        }

        // 5. Constantes de Caracteres ('a', '\n')
        if (c == '\'') {
            lexeme += input[pos++]; // consome '
            if (input[pos] == '\\') { // Se for caractere de escape
                lexeme += input[pos++];
                lexeme += input[pos++];
            } else {
                lexeme += input[pos++]; // consome o caractere
            }
            if (input[pos] == '\'') {
                lexeme += input[pos++];
                return new Token(CHAR_CONSTANT, lexeme);
            } else {
                lexicalError("Constante de caractere mal formada");
            }
        }

        // 6. Constantes de String ("...")
        if (c == '"') {
            lexeme += input[pos++];
            while (pos < input.length() && input[pos] != '"' && input[pos] != '\n') {
                lexeme += input[pos++];
            }
            if (input[pos] == '"') {
                lexeme += input[pos++];
                return new Token(STRING_CONSTANT, lexeme);
            } else {
                lexicalError("String mal formada");
            }
        }

        // 7. Separadores e Operadores
        switch(c) {
            case '(': pos++; return new Token(LPARENTHESE);
            case ')': pos++; return new Token(RPARENTHESE);
            case '{': pos++; return new Token(LBRACE);
            case '}': pos++; return new Token(RBRACE);
            case '[': pos++; return new Token(LBRACKET);
            case ']': pos++; return new Token(RBRACKET);
            case ',': pos++; return new Token(COMMA);
            case ';': pos++; return new Token(SEMICOLON);
            case '+': pos++; return new Token(PLUS);
            case '-': pos++; return new Token(MINUS);
            case '*': pos++; return new Token(MULT);
            case '/': pos++; return new Token(DIV);
            case '=': 
                if (pos + 1 < input.length() && input[pos+1] == '=') { 
                    pos += 2; return new Token(EQ); // '=='
                } else { 
                    pos++; return new Token(ASSIGN); // '='
                }
                break;
            case '!':
                if (pos + 1 < input.length() && input[pos+1] == '=') { pos+=2; return new Token(NEQ); }
                else { pos++; return new Token(NOT); }
                break;
            case '<':
                if (pos + 1 < input.length() && input[pos+1] == '=') { pos+=2; return new Token(LTE); }
                else { pos++; return new Token(LT); }
                break;
            case '>':
                if (pos + 1 < input.length() && input[pos+1] == '=') { pos+=2; return new Token(GTE); }
                else { pos++; return new Token(GT); }
                break;
            case '&':
                if (pos + 1 < input.length() && input[pos+1] == '&') { pos+=2; return new Token(AND); }
                else { pos++; lexicalError("Operador mal formado '&'"); }
                break;
            case '|':
                if (pos + 1 < input.length() && input[pos+1] == '|') { pos+=2; return new Token(OR); }
                else { pos++; lexicalError("Operador mal formado '|'"); }
                break;
            default:
                lexicalError("Caractere invalido: " + string(1, c));
        }
    }

    return new Token(END_OF_FILE);
}

void Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    exit(EXIT_FAILURE);
}