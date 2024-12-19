#include"Translator.h"


void Translator::breakReading(std::string message, int num = 0) {
	filepath.close();
	if (num == 0) {
		std::cout << "Was encountered '" << currentChar << message;
	}
	if (num == 1) {
		std::cout << "no such operand " << message;
	}
	throw "\nAbort Parsing.";
}



bool Translator::isAlph() {
	return (currentChar >= 'a' && currentChar <= 'z') || currentChar == '_';
}

bool Translator::isDigit() {
	return currentChar >= '0' && currentChar <= '9';
}

bool Translator::isSpace() {
	return currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == '\r';
}

void Translator::nextChar() {
	if (currentChar != EOF) {
		currentChar = filepath.get();
		while (isSpace()) {
			currentChar = filepath.get();
		}
	}
}

void Translator::startParse() {

	nextChar();
	while (currentChar != EOF) {
		procS();
	}

}

void Translator::add(const std::pair<std::string, long int>& value) {
	for (int i = 0; varTable.size() > i;i++) {
		if (varTable[i].first == value.first) {
			varTable[i].second = value.second;
			return;
		}
	}
	varTable.push_back(value);
}

long int Translator::procS(bool in, std::string name) {

	if (!in) {

		if (!isAlph()) {
			breakReading(" Expected identifire");
		}
		std::string buff = "";
		while (isAlph()) {
			buff += currentChar;
			nextChar();
		}

		if (currentChar == '(') {

			std::pair<std::string, int> operand = std::make_pair(buff, procE());
			if (currentChar == ')') {
				nextChar();
				add(operand);
			}
			else
				breakReading(" Expected ')'");
			return operand.second;
		}
		else
			breakReading(" Expected '('");

	}
	else {

		std::pair<std::string, int> operand = std::make_pair(name, procE());
		if (currentChar == ')') {
			nextChar();

			add(operand);
		}
		else
			breakReading(" Expected ')'");
		return operand.second;
	}

}

long int Translator::procE() {
	nextChar();
	std::string buff;
	if (currentChar == '-') {
		return;
	}
	else if (currentChar == '*' || currentChar == '+')
		return procT();
	else if (currentChar == '#')
		return procR();
	else if (isAlph())
		return procX();
	else
		breakReading(" Expected operation or digit/operator");
}

long int Translator::procX() {
	std::string buff;
	while (isAlph()) {
		buff += currentChar;
		nextChar();
	}
	if (currentChar == '(') {
		return procS(true, buff);
	}
	else {
		return procI(buff);
	}
}

long int Translator::procI(std::string name) {
	for (int i = 0; i < varTable.size();i++) {
		if (varTable[i].first == name) {
			return varTable[i].second;
		}
	}
	breakReading(name, 1);
}

long int Translator::procT() {
	long int result;
	if (currentChar == '*') {
		result = 1;
		nextChar();
		if (currentChar == '(') {
			result *= procE();
			while (currentChar == ',') {
				result *= procE();
			}
			if (currentChar == ')') {
				nextChar();
				return result;
			}
			else
				breakReading(" Expected ')'");
		}
		else
			breakReading(" Expected '('");
	}
	else if (currentChar == '+') {
		result = 0;
		nextChar();
		if (currentChar == '(') {
			result += procE();
			while (currentChar == ',') {
				result += procE();
			}
			if (currentChar == ')') {
				nextChar();
				return result;
			}
			else
				breakReading(" Expected ')'");
		}
		else
			breakReading(" Expected '('");
	}

}

long int Translator::procR() {
	nextChar();
	std::string buff;
	while (isDigit()) {
		buff += currentChar;
		nextChar();

	}
	return stol(buff);
}