#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		bool hasErrors;

		void advance();
		void match(int);
		void synchronize();
		
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