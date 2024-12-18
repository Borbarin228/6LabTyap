#include<iostream>
#include<fstream>
#include"translator.h"

int main() {
	auto interpreter = new Translator();
	try {
		interpreter->startParse();
	}
	catch (const char* e) {
		std::cout << e << std::endl;
	}
	delete interpreter;
}