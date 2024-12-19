#ifndef lexer_H
#define lexer_H


#include "token.h"

class Lexer {
private:
	char currentChar;
	std::ifstream file;
public:
	Lexer(std::string filepath = "C:\\Users\\Boris\\Desktop\\tyap\\prog.txt") { file.open(filepath); getNextChar(); };
	~Lexer() { file.close(); };
	Token getNextToken();

private:
	void getNextChar();
	std::string procI();
	std::string procC();
	bool isSpace();
	bool isAlph();
	bool isDigit();
	void error(std::string);
};

#endif