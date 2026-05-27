#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		bool hasErrors; // NOVO: Flag para saber se houve erros

		void advance();
		void match(int);
		void synchronize(); // NOVO: Função do Modo Pânico
		
	public:
		Parser(string);
		void run();
		void program();
		void function();
		void varDeclaration();
		void type();
		void paramTypes();
		void statement();
		void assign();
		void expression();
		void term();
		void error(string);
};