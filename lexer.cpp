#include "lexer.h"

void Lexer::getNextChar() {
	if (currentChar != EOF) {
		currentChar = file.get();

		while (isSpace()) {
			currentChar = file.get();
		}
	}
}

bool Lexer::isAlph() {
	return (currentChar >= 'a' && currentChar <= 'z') || currentChar == '_';
}

bool Lexer::isDigit() {
	return currentChar >= '0' && currentChar <= '9';
}

bool Lexer::isSpace() {
	return currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == '\r';
}

void Lexer::error(std::string message) {
	file.close();
	throw "\nAbort Parsing.";
}

std::string Lexer::procI() {
	std::string id = "";

	while (isalpha(currentChar)) {
		id += currentChar;
		getNextChar();
	}

	return id;
}

std::string Lexer::procC() {
	std::string value = "";
	if (currentChar != '#') {
		error("expected #");
	}
	while (isdigit(currentChar)) {
		value += currentChar;
		getNextChar();
	}

	return value;
}

Token Lexer::getNextToken() {
	if (isalpha(currentChar)) {
		std::string id = procI();
		return Token(id, Token::TokenType::I);
	}
	else if (isdigit(currentChar)) {
		std::string value = procC();
		return Token(value, Token::TokenType::C);
	}
	else if (currentChar == '(' || currentChar == ')' || currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == ',' || currentChar == '[' || currentChar == ']') {
		Token token = Token(std::string(1, currentChar), Token::TokenType::Terminal);
		getNextChar();
		return token;
	}
	else if (currentChar == EOF)
		return Token("?", Token::TokenType::Sep);
	else
		error("invalid character");
}