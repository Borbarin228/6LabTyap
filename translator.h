#ifndef translator_H
#define translator_H
#include<String>
#include <vector>
#include<fstream>
#include<iostream>
#include "lexer.h"
#include <stack>
#include <map>

class Translator {
private:
	std::ofstream outFile;
	Lexer& lexer;

	std::stack<Token> stack;
	std::vector<std::string> initializedVar;
	std::vector<std::tuple<char, std::string, std::string>> triads;

	std::vector<std::pair<char, char>> eqMatrix;
	std::vector<std::pair<char, char>> lessMatrix;
	std::vector<std::pair<char, char>> lessEqMatrix;
	std::vector<std::pair<char, char>> moreMatrix;

	std::map<char, std::vector<std::string>> grammar;
public:
	Translator(Lexer& lexer) : lexer(lexer) {
		outFile = std::ofstream("C:\\Users\\Boris\\Desktop\\tyap\\out.txt");
		initMatrices();
		initGrammar();
	};
	
	~Translator() { outFile.close(); };

	void translate();

private:
	void initMatrices();
	void initGrammar();
	bool shiftReduce(Token);
	void reduceStack();
	void formTriads(Token&, std::deque<Token>, std::string);
	bool haveRelationInMatrix(Token, Token, std::vector<std::pair<char, char>>);
	char findRule(std::string);
	std::string getStackStr();
	std::string getDequeStr(std::deque<Token>);
	void logStackState(std::string);
	void writeTriads();
	void error(std::string);
};


#endif