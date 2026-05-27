#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);
		
	public:
		Parser(string);
		void run();
        void program();
        void declaration();
        void function();
        void program();
		void function();
		void varDeclaration();
		void type();
		void paramTypes();
		void statement();
		void assign();
		void expression();
        void error(string);
};