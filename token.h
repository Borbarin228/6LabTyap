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
		Sep = '?',
		Terminal = '!'

	};
	enum PrecendingType {
	
		Null = ' ',
		Less = '<',
		LessEqual = '&',
		Equal = '=',
		More = '>'

	};
	enum OperationType {
		Minus = '-', 
		Plus = '+',
		Multipl = '*',
		None = ' '
		

	};
private:
	std::string value;
	TokenType tokenType;
	PrecendingType precendingType;
	OperationType opType;
	int triadNum;
public:
	Token(std::string valueS, TokenType tokentypeS, PrecendingType precendingTypeS = PrecendingType::Null,int triadNumS = 0, OperationType op = Token::OperationType::None) :value(valueS), tokenType(tokentypeS), precendingType(precendingTypeS), opType(op), triadNum(triadNumS) {};
	std::string getValue() { return value; };
	TokenType getTokenType() { return tokenType; };
	PrecendingType getPrecendingType() { return precendingType; };
	int getTriadNum() { return triadNum; };
	void setPrecedence(PrecendingType p) { precendingType = p; }
	void setTriadnum(int n) { triadNum = n; }
	void setOperation(OperationType op) { opType = op; };
	OperationType getOperationType() { return opType; };
	char getChar() {
		if (this->tokenType != TokenType::Terminal)
			return this->tokenType;
		else
			return this->value[0];
	}
};
#endif