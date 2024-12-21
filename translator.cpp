#include "translator.h"

Triad::Triad(Operation op, Operand opL, Operand opR, std::string opLV, std::string opRV = "") {
	operation = op;
	leftOp = opL;
	rightOp = opR;
	leftOpValue = opLV;
	rightOpValue = opRV;
}


void Translator::translate() {
	stack.push(Token("?", Token::TokenType::Sep));
	Token currentToken = lexer.getNextToken();

	while (true) {
		if (stack.top().getTokenType() == Token::TokenType::Sep && currentToken.getTokenType() == Token::TokenType::Sep)
			break;


		while (shiftReduce(currentToken))
			continue;


		currentToken = lexer.getNextToken();

		if (stack.top().getOperationType() != Token::OperationType::None) {
			currentToken.setOperation(stack.top().getOperationType());
		}

		if (currentToken.getChar() == '+') {
			currentToken.setOperation(Token::OperationType::Plus);
		}
		else if (currentToken.getChar() == '-') {
			currentToken.setOperation(Token::OperationType::Minus);
		}
		else if (currentToken.getChar() == '*') {
			currentToken.setOperation(Token::OperationType::Multipl);
		}

		if (stack.top().getTokenType() == Token::TokenType::I && currentToken.getValue() != "[") {
			bool found = false;
			for (auto var : initializedVar) {
				if (stack.top().getValue() == var) {
					found = true;
					break;
				}
			}
		}
		
		
	}
	std::cout << "OK." << std::endl;
	writeAllTriads();
}

void Translator::initMatrices(){
	eqMatrix = {
		{'L','?'},{'L','S'},
		{'I','['},{'[','E'},{'E',']'},
		{'-','('},{'+','('},{'*','('},{'T',')'},
		{'T',','},{',','E'},
	};
	lessMatrix = {
		{'?','S'},{'?','I'}, {'L','I'},
		{'[','-'},{'[','+'},{'[','*'},{'[','S'},{'[','C'},{'[','I'},
		{'(','E'},{'(','-'},{'(','+'},{'(','*'},{'(','S'},{'(','I'},{'(','C'},
		{',','-'},{',','+'},{',','*'},{',','S'},{',','I'},{',','C'},
	};
	moreMatrix = {
		{'S','?'},{']','?'},{'S','I'},{']','I'},{')',']'},
		{']',')'},{']',']'},{'E',')'},{')',')'},{'S',')'},{'I',')'},{'C',')'},{'C',']'},
		{']',','},{'S',']'},{'I',']'},{'E',','},{')',','},{'S',','},{'I',','},{'C',','},
	};
	lessEqMatrix = {
		{'?','L'},{'(','T'},
	};
}

void Translator::initGrammar(){
	grammar = {
	{'L', {"LS", "S"}},
	{'S', {"I[E]"}},
	{'E', {"+(T)", "-(T)", "*(T)", "S", "I", "C"}},
	{'T', {"T,E", "E"}},
	};
}

bool Translator::shiftReduce(Token token){
	if (haveRelationInMatrix(stack.top(), token, lessEqMatrix)) {
		token.setPrecedence(Token::PrecendingType::LessEqual);
		stack.push(token);
		logStackState("shift(less or Equal)");
	}
	else if (haveRelationInMatrix(stack.top(), token, eqMatrix)) {
		token.setPrecedence(Token::PrecendingType::Equal);
		stack.push(token);
		logStackState("shift(Equal)");
	}
	else if (haveRelationInMatrix(stack.top(), token, lessMatrix)) {
		token.setPrecedence(Token::PrecendingType::Less);
		stack.push(token);
		logStackState("shift(less)");
	}
	else if (haveRelationInMatrix(stack.top(), token, moreMatrix)) {
		reduceStack();
		logStackState("reduce");
		return true;
	}
	else {
		error("invalid syntax - no such sequence of characters");
	}
	return false;
}

void Translator::reduceStack(){
	std::deque<Token> rulesDeq = stack._Get_container();
	bool reduced = false;
	char rule = NULL;
	std::string stringRule = "";
	while (!rulesDeq.empty()) {
		while (!rulesDeq.empty() && rulesDeq.front().getPrecendingType() != Token::PrecendingType::Less && rulesDeq.front().getPrecendingType() != Token::PrecendingType::LessEqual) {
			rulesDeq.pop_front();
		}
		stringRule = getDequeStr(rulesDeq);


		rule = findRule(stringRule);
		if (rule != NULL) {
			reduced = true;
			break;
		}
		
		rulesDeq.pop_front();
	}

	if (!reduced)
		error("invalid syntax - no such rule");

	Token tmp = stack.top();
	stack.pop();

	if (rule == 'L' && (stack.top().getValue() == "[" || stack.top().getValue() == "," || stack.top().getValue() == "(")) {
		rule = 'E';
	}
	else {
		stack.push(tmp);
		for (int i = 0; i < rulesDeq.size();i++) {
			if (rule == 'S' && stack.top().getTokenType() == Token::TokenType::I)
				initializedVar.push_back(stack.top().getValue());
			stack.pop();
		}
	}

	Token reducedToken = Token("", static_cast<Token::TokenType>(rule));
	formTriads(reducedToken, rulesDeq, stringRule);
	if (haveRelationInMatrix(stack.top(), reducedToken, lessEqMatrix))
		reducedToken.setPrecedence(Token::PrecendingType::LessEqual);
	else if (haveRelationInMatrix(stack.top(), reducedToken, eqMatrix))
		reducedToken.setPrecedence(Token::PrecendingType::Equal);
	else if (haveRelationInMatrix(stack.top(), reducedToken, lessMatrix))
		reducedToken.setPrecedence(Token::PrecendingType::Less);

	else
		error("invalid syntax - no such sequence of characters to reduce");

	stack.push(reducedToken);

}

void Translator::formTriads(Token& reducedToken, std::deque<Token> tokensSequence, std::string rule){
	if (rule == "C") {
		triads.push_back(new Triad(Triad::Operation::Const, Triad::Operand::Constant, Triad::Operand::Void, tokensSequence[0].getValue()));
		reducedToken.setTriadnum(triads.size());
		reducedToken.setOperation(stack.top().getOperationType());
	}
	else if (rule == "I") {
		triads.push_back(new Triad(Triad::Operation::Var, Triad::Operand::Varible, Triad::Operand::Void, tokensSequence[0].getValue()));
		reducedToken.setTriadnum(triads.size());
		reducedToken.setOperation(tokensSequence[0].getOperationType());
	}
	else if (rule == "E") {
		reducedToken.setTriadnum(tokensSequence[0].getTriadNum());
		reducedToken.setOperation(tokensSequence[0].getOperationType());
	}

	else if (rule[0] == '+' || rule[0] == '-' || rule[0] == '*') {
		reducedToken.setTriadnum(triads.size());
		reducedToken.setOperation(stack.top().getOperationType());
	}

	else if (rule == "I[E]") {
		reducedToken.setOperation(stack.top().getOperationType());
		reducedToken.setTriadnum(triads.size() + 2);
		triads.push_back(new Triad(Triad::Var, Triad::Varible, Triad::Void, tokensSequence[0].getValue()));
		triads.push_back(new Triad(Triad::Equal, Triad::Link, Triad::Link, std::to_string(triads.size()), std::to_string(tokensSequence[2].getTriadNum())));
	}
	else if (rule == "T,E") {
		reducedToken.setTriadnum(tokensSequence[2].getTriadNum() + 1);
		reducedToken.setOperation(tokensSequence[2].getOperationType());
		if(reducedToken.getOperationType() == Token::OperationType::Plus){
			triads.push_back(new Triad(Triad::Plus, Triad::Link, Triad::Link, std::to_string(tokensSequence[0].getTriadNum()), std::to_string(tokensSequence[2].getTriadNum())));
		}
		if (reducedToken.getOperationType() == Token::OperationType::Multipl) {
			triads.push_back(new Triad(Triad::Multipl, Triad::Link, Triad::Link, std::to_string(tokensSequence[0].getTriadNum()), std::to_string(tokensSequence[2].getTriadNum())));
		}
		if (reducedToken.getOperationType() == Token::OperationType::Minus) {
			triads.push_back(new Triad(Triad::Minus, Triad::Link, Triad::Link, std::to_string(tokensSequence[0].getTriadNum()), std::to_string(tokensSequence[2].getTriadNum())));
		}
	}
	else if (rule == "S") {
		reducedToken.setOperation(stack.top().getOperationType());
		reducedToken.setTriadnum(tokensSequence[0].getTriadNum());
	}
}

bool Translator::haveRelationInMatrix(Token left, Token right, std::vector<std::pair<char, char>> matrix){
	for (auto key : matrix) {
		if (key.first == left.getChar() && key.second == right.getChar()) {
			return true;
		}
	}
	return false;
}

char Translator::findRule(std::string _rule){
	for (auto& rules : grammar) {
		for (auto& rule : rules.second) {
			if (rule == _rule) {
				return rules.first;
			}
		}
	}
	return NULL;
}

std::string Translator::getStackStr(){
	std::stack<Token> tmp = stack;
	std::vector<char> stackChars;
	while (!tmp.empty()) {
		stackChars.push_back(tmp.top().getChar());
		stackChars.push_back(tmp.top().getPrecendingType());
		tmp.pop();
	}
	std::reverse(stackChars.begin(), stackChars.end());
	return std::string(stackChars.begin(), stackChars.end());
}

std::string Translator::getDequeStr(std::deque<Token> deque){
	std::string result = "";
	while (!deque.empty()) {
		result += deque.front().getChar();
		deque.pop_front();
	}
	return result;
}

void Translator::logStackState(std::string message){
	std::cout << "Action:\t" << message << std::endl;
	std::cout << "Stack:\t\b" << getStackStr() << std::endl;

	std::cout << std::endl;
}

void Translator::writeAllTriads(){
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

void Translator::error(std::string message){
	outFile.close();
	std::cout << "\nInterpreter error: " << message << std::endl;
	throw "oxoxox";
}