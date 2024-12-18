#ifndef translator_H
#define translator_H
#include<String>
#include <vector>
#include<fstream>
#include<iostream>

class Triad {
public:
	enum Operation { Var, Const, Equal, Minus, Plus, Multipl, None };
	enum Operand { Varible, Constant, Void, Link };
	Operand leftOp, rightOp;
	Operation operation;
	std::string leftOpValue, rightOpValue;
	Triad(Operation, Operand, Operand, std::string, std::string);

};

class Translator {
private:
	long _numberOfTriads;
	std::ifstream filepath;
	char currentChar;
	std::vector<std::pair<std::string, long int>> varTable;
	std::vector<Triad*> triads;
public:
	Translator() : _numberOfTriads(0), currentChar('\0') { filepath.open("C:\\Users\\Boris\\Desktop\\tyap\\prog.txt"); };
	~Translator() { filepath.close(); };

	void startParse();
private:
	void nextChar();
	bool isSpace();
	bool isAlph();
	bool isDigit();
	long int procS(bool = false, std::string = "");
	long int procE();
	long int procR();
	void breakReading(std::string, int);
	void add(const std::pair<std::string, long int>&);
	long int procT();
	long int procI(std::string);
	long int procX();
	void parseTriads();
	void firstRule(Triad*);
	void secondRule(Triad*);
	void thirdRule(Triad*);
	void fourthRule(Triad*);
	void writeTriad(int);
};


#endif 