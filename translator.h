#ifndef translator_H
#define translator_H
#include<String>
#include <vector>
#include<fstream>
#include<iostream>

class Translator {
private:
	std::ifstream filepath;
	char currentChar;
	std::vector<std::pair<std::string, long int>> varTable;
public:
	Translator() : currentChar('\0') { filepath.open("C:\\Users\\Boris\\Desktop\\tyap\\prog.txt"); };
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

};


#endif