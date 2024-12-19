#include<iostream>
#include<fstream>
#include"translator.h"

int main() {
	Lexer lexer;
	Translator translator(lexer);
	translator.translate();
}