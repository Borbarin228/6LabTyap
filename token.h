#ifndef token_H
#define token_H
#include<String>
#include <vector>
#include<fstream>
#include<iostream>

class Token{

public:

	enum TokenType {
		L = 'L',
		S = 'S',
		E = 'E',
		T = 'T',
		I = 'I',
		C = 'C',
		Sep = 'S',
		Terminal = '!'

	};
	enum PrecendingType {
	
		None = ' ',
		Less = '<',
		LessEqual = '&',
		Equal = '=',
		More = '>'

	};
private:
	std::string value;
	TokenType tokenType;
	PrecendingType precendingType;
	int triadNum;
public:
	Token(std::string valueS, TokenType tokentypeS, PrecendingType precendingTypeS = None, int triadNumS = 0) :value(valueS), tokenType(tokentypeS), precendingType(precendingTypeS), triadNum(triadNumS) {};
	std::string getValue() { return value; };
	TokenType getTokenType() { return tokenType; };
	PrecendingType getPrecendingType() { return precendingType; };
	int getTriadNum() { return triadNum; };
	void setPrecedence(PrecendingType p) { precendingType = p; }
	void setTriadnum(int n) { triadNum = n; }
	char getChar() {
		if (this->tokenType != TokenType::Terminal)
			return this->tokenType;
		else
			return this->value[0];
	}
};
#endif