#include"Translator.h"

Triad::Triad(Operation op, Operand opL, Operand opR, std::string opLV, std::string opRV = "") {
	operation = op;
	leftOp = opL;
	rightOp = opR;
	leftOpValue = opLV;
	rightOpValue = opRV;
}

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
	parseTriads();
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
		std::cout << ++_numberOfTriads << ":\t" << "V(" << buff << ", @)\n";
		triads.push_back(new Triad(Triad::Var, Triad::Varible, Triad::Void, buff));
		int leftOp = _numberOfTriads;
		if (currentChar == '(') {
			int rightOp = procE();
			std::pair<std::string, int> operand = std::make_pair(buff, rightOp);
			if (currentChar == ')') {
				nextChar();
				add(operand);
			}
			else
				breakReading(" Expected ')'");
			std::cout << ++_numberOfTriads << ":\t" << "=(^" << leftOp << ", ^" << operand.second << ")\n";
			triads.push_back(new Triad(Triad::Equal, Triad::Link, Triad::Link, std::to_string(leftOp), std::to_string(operand.second)));
			return _numberOfTriads;
		}
		else
			breakReading(" Expected '('");

	}
	else {
		std::cout << ++_numberOfTriads << ":\t" << "V(" << name << ", @)\n";
		triads.push_back(new Triad(Triad::Var, Triad::Varible, Triad::Void, name));
		int leftOp = _numberOfTriads;
		std::pair<std::string, int> operand = std::make_pair(name, procE());
		if (currentChar == ')') {

			nextChar();
			std::cout << ++_numberOfTriads << ":\t" << "=(^" << leftOp << ", ^" << operand.second << ")\n";
			triads.push_back(new Triad(Triad::Equal, Triad::Link, Triad::Void, std::to_string(leftOp), std::to_string(operand.second)));
			add(operand);
		}
		else
			breakReading(" Expected ')'");
		return _numberOfTriads;
	}

}

long int Translator::procE() {
	nextChar();
	std::string buff;
	if (currentChar == '-') {
		int triad = procE();
		std::cout << ++_numberOfTriads << ":\t" << "-(^" << triad << ", @)\n";
		triads.push_back(new Triad(Triad::Minus, Triad::Link, Triad::Void, std::to_string(triad)));
		return _numberOfTriads;
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
			std::cout << ++_numberOfTriads << ":\t" << "V(" << name << ", @" << ")\n";
			triads.push_back(new Triad(Triad::Var, Triad::Varible, Triad::Void, name));
			return _numberOfTriads;
		}
	}
	breakReading(name, 1);
}

long int Translator::procT() {
	int leftOp, rightOp;
	if (currentChar == '*') {
		nextChar();
		if (currentChar == '(') {
			leftOp = procE();
			while (currentChar == ',') {
				rightOp = procE();
				std::cout << ++_numberOfTriads << ":\t" << "*(^" << leftOp << ", ^" << rightOp << ")\n";
				triads.push_back(new Triad(Triad::Multipl, Triad::Link, Triad::Link, std::to_string(leftOp), std::to_string(rightOp)));
				leftOp = _numberOfTriads;
			}
			if (currentChar == ')') {
				nextChar();
			}
			else
				breakReading(" Expected ')'");
		}
		else
			breakReading(" Expected '('");
	}
	else if (currentChar == '+') {
		nextChar();
		if (currentChar == '(') {
			leftOp = procE();
			while (currentChar == ',') {
				rightOp = procE();
				std::cout << ++_numberOfTriads << ":\t" << "+(^" << leftOp << ", ^" << rightOp << ")\n";
				triads.push_back(new Triad(Triad::Plus, Triad::Link, Triad::Link, std::to_string(leftOp), std::to_string(rightOp)));
				leftOp = _numberOfTriads;
			}
			if (currentChar == ')') {

				nextChar();
			}
			else
				breakReading(" Expected ')'");
		}
		else
			breakReading(" Expected '('");
	}
	return leftOp;
}

long int Translator::procR() {
	nextChar();
	std::string buff;
	while (isDigit()) {
		buff += currentChar;
		nextChar();

	}
	std::cout << ++_numberOfTriads << ":\t" << "C(" << buff << ", @)\n";
	triads.push_back(new Triad(Triad::Const, Triad::Constant, Triad::Void, buff));
	return _numberOfTriads;
}

void Translator::parseTriads() {
	std::cout << "\n|||||||||||||PARSE TRIADS||||||||||||\n\n";
	for (int i = 0;i < triads.size();i++) {
		firstRule(triads[i]);
		secondRule(triads[i]);
		thirdRule(triads[i]);
		fourthRule(triads[i]);
	}
	for (int i = 0; i < triads.size();i++) {
		std::cout << i + 1 << ":\t";
		writeTriad(i);
	}

}

void Translator::writeTriad(int i) {
	std::string result = "";
	switch (triads[i]->operation)
	{
	case Triad::Var:
		result = "V(";
		break;
	case Triad::Const:
		result = "C(";
		break;
	case Triad::Minus:
		result = "-(";
		break;
	case Triad::Multipl:
		result = "*(";
		break;
	case Triad::Plus:
		result = "+(";
		break;
	case Triad::Equal:
		result = "=(";
		break;
	case Triad::None:
		std::cout << "DELETE" << std::endl;
		return;
	}
	if (triads[i]->leftOp == Triad::Constant) result += triads[i]->leftOpValue;
	else if (triads[i]->leftOp == Triad::Varible) result += triads[i]->leftOpValue;
	else if (triads[i]->leftOp == Triad::Link) result += "^" + triads[i]->leftOpValue;
	else if (triads[i]->leftOp == Triad::Void) result += "@";

	result += ", ";

	if (triads[i]->rightOp == Triad::Constant) result += triads[i]->rightOpValue;
	else if (triads[i]->rightOp == Triad::Varible) result += triads[i]->rightOpValue;
	else if (triads[i]->rightOp == Triad::Link) result += "^" + triads[i]->rightOpValue;
	else if (triads[i]->rightOp == Triad::Void) result += "@";

	std::cout << result << ")" << std::endl;
}

void Translator::firstRule(Triad* currentTriad) {
	if (currentTriad->operation == Triad::Minus) {
		if (currentTriad->leftOp == Triad::Link) {
			auto leftTriad = triads[std::stoi(currentTriad->leftOpValue) - 1];
			if (leftTriad->operation == Triad::Const) {
				currentTriad->leftOpValue = leftTriad->leftOpValue;
				currentTriad->leftOp = Triad::Constant;
				leftTriad->leftOp = Triad::Void;
				leftTriad->operation = Triad::None;
			}
		}
	}
	else if (currentTriad->operation != Triad::Equal && currentTriad->rightOp != Triad::Void) {
		if (currentTriad->leftOp == Triad::Link) {
			auto leftTriad = triads[std::stoi(currentTriad->leftOpValue) - 1];
			if (leftTriad->operation == Triad::Const) {
				currentTriad->leftOpValue = leftTriad->leftOpValue;
				currentTriad->leftOp = Triad::Constant;
				leftTriad->leftOp = Triad::Void;
				leftTriad->operation = Triad::None;
			}
		}

	}


}

void Translator::secondRule(Triad* currentTriad) {
	if (currentTriad->operation == Triad::Minus && currentTriad->leftOp == Triad::Constant) {
		currentTriad->operation = Triad::Const;
		currentTriad->leftOpValue = std::to_string(-1 * std::stoi(currentTriad->leftOpValue));
	}
}

void Translator::thirdRule(Triad* currentTriad) {
	if (currentTriad->operation != Triad::Equal) {
		if (currentTriad->rightOp == Triad::Link) {
			auto rightTriad = triads[std::stoi(currentTriad->rightOpValue) - 1];
			if (rightTriad->operation == Triad::Const) {
				currentTriad->rightOpValue = rightTriad->leftOpValue;
				currentTriad->rightOp = Triad::Constant;
				rightTriad->leftOp = Triad::Void;
				rightTriad->operation = Triad::None;
			}
		}
		if (currentTriad->leftOp == Triad::Constant && currentTriad->rightOp == Triad::Constant) {
			if (currentTriad->operation == Triad::Multipl) {
				currentTriad->operation = Triad::Const;
				currentTriad->leftOpValue = std::to_string(std::stoi(currentTriad->leftOpValue) * std::stoi(currentTriad->rightOpValue));
				currentTriad->rightOp = Triad::Void;
			}
			else if (currentTriad->operation == Triad::Plus) {
				currentTriad->operation = Triad::Const;
				currentTriad->leftOpValue = std::to_string(std::stoi(currentTriad->leftOpValue) + std::stoi(currentTriad->rightOpValue));
				currentTriad->rightOp = Triad::Void;
			}
		}
	}
}

void Translator::fourthRule(Triad* currentTriad) {
	if (currentTriad->operation == Triad::Equal) {
		if (currentTriad->leftOp == Triad::Link) {
			auto leftTriad = triads[std::stoi(currentTriad->leftOpValue) - 1];
			if (leftTriad->leftOp == Triad::Varible) {
				currentTriad->leftOp = Triad::Varible;
				currentTriad->leftOpValue = leftTriad->leftOpValue;
				leftTriad->leftOp = Triad::Void;
				leftTriad->operation = Triad::None;
			}
		}
	}
}