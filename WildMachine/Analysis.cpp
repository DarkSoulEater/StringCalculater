#pragma once
#include "Analysis.h"

// Class Converter
Converter::Converter(std::map<std::string, bool>& var_name) : var_name_map_(var_name){

}

void Converter::addData(char x) {
    var_date_ += x;
}
void Converter::addComma() {
	while (!state_stack_.empty()) {
		Conv_state state = state_stack_.top(); state_stack_.pop();

		if (state == Conv_state::open_brack_type_1 ||
			state == Conv_state::open_brack_type_2) {
			if (!state_stack_.empty() && state_stack_.top() != Conv_state::function) {
				throw ErrorType::CommaNotForArguments;
			}
			state_stack_.push(state);
			state_stack_.push(Conv_state::comma);
			return;
		} else if (state == Conv_state::comma) {
			state_stack_.push(state);
			state_stack_.push(state);
			return;
		} else {
			postfix_str += StateToString(state) + " ";
		}
	}

	if (state_stack_.empty()) {
		throw ErrorType::CommaNotForArguments;
	}
}
void Converter::addPoint() {
	++cnt_point_;
}
void Converter::addBracket(char x) {
	if (x == '(') {
		state_stack_.push(Conv_state::open_brack_type_1);
		return;
	} else if (x == '[') {
		state_stack_.push(Conv_state::open_brack_type_2);
		return;
	}

	int cnt_comma = 0;
	while (!state_stack_.empty()) {
		Conv_state state = state_stack_.top(); state_stack_.pop();

		if (state == Conv_state::comma) {
			++cnt_comma;
			continue;
		} else if (state == Conv_state::open_brack_type_1 ||
				   state == Conv_state::open_brack_type_2) {
			if (state == Conv_state::open_brack_type_1 && x == ')' ||
				state == Conv_state::open_brack_type_2 && x == ']') {
				if (!state_stack_.empty() && state_stack_.top() == Conv_state::function) {
					std::string name = function_stack_.top().first;
					int k = function_stack_.top().second;
					function_stack_.pop();

					if (cnt_comma > k - 1) {
						throw ErrorType::TooManyArguments;
					} else if (cnt_comma < k - 1) {
						throw ErrorType::TooFewArguments;
					}

					postfix_str += name + "(" + std::to_string(k) + ") ";
					state_stack_.pop();
					--cnt_nested_funct_;
				}
				return;
			} else {
				if (x == ')') {
					throw ErrorType::FoundUncorrectClosedBrackT1;
				} else {
					throw ErrorType::FoundUncorrectClosedBrackT2;
				}
			}
		} else {
			postfix_str += StateToString(state) + " ";
		}
	}

	if (x == ')') {
		throw ErrorType::NotFoundOpenBrackT1;
	} else {
		throw ErrorType::NotFoundOpenBrackT2;
	}
}
void Converter::addOperator(char x) {
	Conv_state new_state = CharToState(x);
	//std::string tmp = "";
	while (!state_stack_.empty()) {
		Conv_state state = state_stack_.top();
		if (int(state) >= int(new_state)) {
			state_stack_.pop();
			postfix_str += StateToString(state) + " ";
			//tmp += " " + StateToString(state);
		} else {
			break;
		}
	}
	//std::reverse(tmp.begin(), tmp.end());
	//postfix_str += tmp;
	state_stack_.push(new_state);
}
void Converter::addFunction(std::string name, char brack, int k) {
	++cnt_nested_funct_;
	function_stack_.push({ name, k });
	state_stack_.push(Conv_state::function);
	state_stack_.push(CharToState(brack));
}

void Converter::saveData() {
	if (cnt_point_ > 1) {
		throw ErrorType::FoundUnknownCharacter;
	} else if (cnt_point_ == 1) {
		if (var_date_[var_date_.size() - 1] == '.' || var_date_[var_date_.size() - 1] == 'e') {
			throw ErrorType::error;
		}
	}
	cnt_point_ = 0;
	postfix_str += var_date_ + " ";

	if (isalpha(var_date_[0])) {
		var_name_map_[var_date_] = 1;
	}

	var_date_ = "";
}
void Converter::deleteData() {
	var_date_ = "";
	cnt_point_ = 0;
}
std::string Converter::getData() {
	return var_date_;
}

void Converter::finalAnalysis() {
	std::string tmp = "";
	while (!state_stack_.empty()) {
		Conv_state state = state_stack_.top();
		if (state == Conv_state::open_brack_type_1) {
			throw ErrorType::NotFoundClosedBrackT1;
		} else if (state == Conv_state::open_brack_type_2) {
			throw ErrorType::NotFoundClosedBrackT2;
		}
		postfix_str += StateToString(state) + " ";
		//tmp += " " + StateToString(state);
		state_stack_.pop();
	}
	//std::reverse(tmp.begin(), tmp.end());
	//postfix_str += tmp;
}

// Private
Converter::Conv_state Converter::CharToState(char x) {
	if (x == '+') {
		return Conv_state::operator_plus;
	}

	if (x == '-') {
		return Conv_state::operator_minus;
	}

	if (x == '*') {
		return Conv_state::operator_multi;
	}

	if (x == '/') {
		return Conv_state::operator_div;
	}

	if (x == '%') {
		return Conv_state::operator_mod;
	}

	if (x == '^') {
		return Conv_state::power;
	}

	if (x == '(') {
		return Conv_state::open_brack_type_1;
	}

	if (x == '[') {
		return Conv_state::open_brack_type_2;
	}
}
std::string Converter::StateToString(Conv_state x) {
	switch (x) {
	case Converter::Conv_state::function:
		return "f";
		break;
	case Converter::Conv_state::open_brack_type_1:
		return "(";
		break;
	case Converter::Conv_state::open_brack_type_2:
		return "[";
		break;
	case Converter::Conv_state::operator_plus:
		return "+";
		break;
	case Converter::Conv_state::operator_minus:
		return "-";
		break;
	case Converter::Conv_state::operator_multi:
		return "*";
		break;
	case Converter::Conv_state::operator_div:
		return "/";
		break;
	case Converter::Conv_state::operator_mod:
		return "%";
		break;
	case Converter::Conv_state::power:
		return "^";
		break;
	default:
		return "E";
		break;
	}
}

// DFA
bool isbracket(char x) {
	return x == '(' || x == '[' || x == ')' || x == ']';
}
bool isoperator(char x) {
	return x == '+' || x == '-' || x == '*' || x == '/' || x == '%' || x == '^';
}
bool iscomma(char x) { 
	return x == ','; 
}

std::string DFA(std::string str, std::unordered_map<std::string, int>& funct_list, std::map<std::string, bool>& var_name_map) {
	try {
		Converter converter(var_name_map);
		State state = State::wait_state;

		for (int j = 0; j < str.size(); ++j) {
			char current = str[j];

			switch (state) {
			case State::wait_state: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					converter.addBracket(current);
					state = State::open_bracket;
				} else if (isoperator(current)) {
					throw ErrorType::NotFoundLeftOperand;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
			} break;
			case State::const_state: {
				if (current == '.') {
					converter.addData(current);
					converter.addPoint();
				} else if (current == 'e') {
					converter.addData(current);
					converter.addPoint();
				} else if (iscomma(current)) {
					converter.saveData();
					converter.addComma();
					state = State::comma_state;
				} else if (isspace(current)) {
					converter.saveData();
					state = State::space_before_operator;
				} else if (isoperator(current)) {
					converter.saveData();
					converter.addOperator(current);
					state = State::math_operator;
				} else if (isalpha(current)) {
					converter.saveData();
					converter.addOperator('*');
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					converter.saveData();
					if (current == '(' || current == '[') {
						converter.addOperator('*');
						converter.addBracket(current);
						state = State::open_bracket;
					} else {
						converter.addBracket(current);
						state = State::space_before_operator;
					}
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::str_state: {
				if (iscomma(current)) {
					converter.saveData();
					converter.addComma();
					state = State::comma_state;
				} else if (isspace(current)) {
					converter.saveData();
					state = State::space_before_operator;
				} else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					throw ErrorType::FoundIntInStr;
				} else if (isoperator(current)) {
					converter.saveData();
					converter.addOperator(current);
					state = State::math_operator;
				} else if (isbracket(current)) {
					if (current == '(' || current == '[') {
						if (funct_list.count(converter.getData())) {
							converter.addFunction(converter.getData(), current, funct_list[converter.getData()]);
							converter.deleteData();
							state = State::open_bracket;
						} else {
							converter.saveData();
							converter.addOperator('*');
							converter.addBracket(current);
							state = State::open_bracket;
						}
					} else {
						converter.saveData();
						converter.addBracket(current);
						state = State::space_before_operator;
					}
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::math_operator: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::NotFoundRightOperand;
					}
					converter.addBracket(current);
					state = State::open_bracket;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::open_bracket: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::FoundEmptyBrack;
					}
					converter.addBracket(current);
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::comma_state: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::ExpectedArguments;
					}
					converter.addBracket(current);
					state = State::open_bracket;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::space_before_operator: {
				if (isspace(current)) continue;
				else if (isoperator(current)) {
					converter.addOperator(current);
					state = State::math_operator;
				} else if (isdigit(current) || isalpha(current) || isbracket(current)) {
					throw ErrorType::ExpectedOperator;
				} else if (iscomma(current)) {
					converter.addComma();
					state = State::comma_state;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			default: {
				break;
			}
			}
		}
	
		switch (state) { // The ultimate test
		case State::wait_state: {
			break;
		}
		case State::const_state: {
			converter.saveData();
			break;
		}
		case State::str_state: {
			converter.saveData();
			break;
		}
		case State::math_operator: {
			throw ErrorType::NotFoundRightOperand;
			break;
		}
		case State::open_bracket: {
			// Checked in final analysis
			break;
		}
		case State::comma_state: {
			throw ErrorType::ExpectedArguments;
			break;
		}
		case State::space_before_operator: {
			break;
		}
		default: {
			break;
		}
		}
	
		converter.finalAnalysis();
		return converter.getAns();
		//std::cout << converter.getAns() << "\n";
	} catch (int a) { ; }
	catch (const ErrorType& error) {
		switch (error) {
		case ErrorType::CommaNotForArguments:
			//std::cout << "CommaNotForArguments\n";
			throw std::string("CommaNotForArguments");
			break;
		case ErrorType::TooManyArguments:
			throw std::string("TooManyArguments");
			break;
		case ErrorType::TooFewArguments:
			throw std::string("TooFewArguments");
			break;
		case ErrorType::NotFoundOpenBrackT1:
			throw std::string("NotFoundOpenBrackT1");
			break;
		case ErrorType::NotFoundOpenBrackT2:
			throw std::string("NotFoundOpenBrackT2");
			break;
		case ErrorType::NotFoundClosedBrackT1:
			throw std::string("NotFoundClosedBrackT1");
			break;
		case ErrorType::NotFoundClosedBrackT2:
			throw std::string("NotFoundClosedBrackT2");
			break;
		case ErrorType::FoundUncorrectClosedBrackT1:
			throw std::string("FoundUncorrectClosedBrackT1");
			break;
		case ErrorType::FoundUncorrectClosedBrackT2:
			throw std::string("FoundUncorrectClosedBrackT2");
			break;
		case ErrorType::NotFoundLeftOperand:
			throw std::string("NotFoundLeftOperand");
			break;
		case ErrorType::NotFoundRightOperand:
			throw std::string("NotFoundRightOperand");
			break;
		case ErrorType::FoundUnknownCharacter:
			throw std::string("FoundUnknownCharacter");
			break;
		case ErrorType::FoundIntInStr:
			throw std::string("FoundIntInStr");
			break;
		case ErrorType::FoundEmptyBrack:
			throw std::string("FoundEmptyBrack");
			break;
		case ErrorType::ExpectedArguments:
			throw std::string("ExpectedArguments");
			break;
		case ErrorType::ExpectedOperator:
			throw std::string("ExpectedOperator");
			break;
		case ErrorType::error:
			throw std::string("Expected digit after 0. or 0e");
			break;
		default:
			break;
		}
	}
	return "";
}


/*
bool DFA(std::string str, std::unordered_map<std::string, int>& funct_list) {
	try {
		Converter converter;
		State state = State::wait_state;

		for (int j = 0; j < str.size(); ++j) {
			char current = str[j];

			switch (state) {
			case State::wait_state: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					converter.addBracket(current);
					state = State::open_bracket;
				} else if (isoperator(current)) {
					throw ErrorType::NotFoundLeftOperand;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::const_state: {
				if (iscomma(current)) {
					converter.saveData();
					converter.addComma();
					state = State::comma_state;
				} else if (isspace(current)) {
					converter.saveData();
					state = State::space_before_operator;
				} else if (isoperator(current)) {
					converter.saveData();
					converter.addOperator(current);
					state = State::math_operator;
				} else if (isalpha(current)) {
					converter.saveData();
					converter.addOperator('*');
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					converter.saveData();
					if (current == '(' || current == '[') {
						converter.addOperator('*');
						converter.addBracket(current);
						state = State::open_bracket;
					} else {
						converter.addBracket(current);
						state = State::space_before_operator;
					}
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::str_state: {
				if (iscomma(current)) {
					converter.saveData();
					converter.addComma();
					state = State::comma_state;
				} else if (isspace(current)) {
					converter.saveData();
					state = State::space_before_operator;
				} else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					throw ErrorType::FoundIntInStr;
				} else if (isbracket(current)) {
					if (current == '(' || current == '[') {
						if (funct_list.count(converter.getData())) {
							converter.addFunction(converter.getData(), current, funct_list[converter.getData()]);
							converter.deleteData();
							state = State::open_bracket;
						} else {
							converter.saveData();
							converter.addOperator('*');
							converter.addBracket(current);
							state = State::open_bracket;
						}
					} else {
						converter.saveData();
						converter.addBracket(current);
						state = State::space_before_operator;
					}
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::math_operator: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::NotFoundRightOperand;
					}
					converter.addBracket(current);
					state = State::open_bracket;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::open_bracket: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::FoundEmptyBrack;
					}
					converter.addBracket(current);
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::comma_state: {
				if (isspace(current)) continue;
				else if (isalpha(current)) {
					converter.addData(current);
					state = State::str_state;
				} else if (isdigit(current)) {
					converter.addData(current);
					state = State::const_state;
				} else if (isbracket(current)) {
					if (current == ')' || current == ']') {
						throw ErrorType::ExpectedArguments;
					}
					converter.addBracket(current);
					state = State::open_bracket;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			case State::space_before_operator: {
				if (isspace(current)) continue;
				else if (isoperator(current)) {
					converter.addOperator(current);
					state = State::math_operator;
				} else if (isdigit(current) || isalpha(current) || isbracket(current)) {
					throw ErrorType::ExpectedOperator;
				} else if (iscomma(current)) {
					converter.addComma();
					state = State::comma_state;
				} else {
					throw ErrorType::FoundUnknownCharacter;
				}
				break;
			}
			default: {
				break;
			}
			}
		}

		switch (state) { // The ultimate test
		case State::wait_state: {
			break;
		}
		case State::const_state: {
			converter.saveData();
			break;
		}
		case State::str_state: {
			converter.saveData();
			break;
		}
		case State::math_operator: {
			throw ErrorType::NotFoundRightOperand;
			break;
		}
		case State::open_bracket: {
			// Checked in final analysis
			break;
		}
		case State::comma_state: {
			throw ErrorType::ExpectedArguments;
			break;
		}
		case State::space_before_operator: {
			break;
		}
		default: {
			break;
		}
		}

		converter.finalAnalysis();
		std::cout << converter.getAns() << "\n";
		return 1;
	} catch (const ErrorType & error) {
		std::cout << int(error);
		std::cout << "ErrorExpection\n";
		return 0;
	}
}
*/