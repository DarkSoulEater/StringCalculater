#pragma once
#include <string>
#include <stack>
#include <unordered_map>
#include "UndergroundType.h"
#include "Analysis.h"

#define check   if (isalpha(var.top()[0])) {\
					val = variable_list[var.top()];\
					if (val == "") return "Variable not set";\
				} else {\
					val = var.top();\
				}

std::string Calculate(const std::string& expr, std::map<std::string, std::string>& variable_list) {
	std::string data = "";
	std::stack<std::string> var;
	// return expr;
	for (int i = 0; i < expr.size(); ++i) {
		if (expr[i] != ' ') {
			data += expr[i];
		} else {
			switch (data[data.size() - 1]) {
			case '+': {
				std::string val;
				check;
				LongDouble a(val); var.pop();
				check;
				LongDouble b(val); var.pop();
				LongDouble ans = a + b;
				var.push(ans.ld_to_string());
				break;
			}
			case '-': {
				std::string val;
				check;
				LongDouble a(val); var.pop();
				check;
				LongDouble b(val); var.pop();
				LongDouble ans = b - a;
				var.push(ans.ld_to_string());
				break;
			}
			case '*': {
				std::string val;
				check;
				LongDouble a(val); var.pop();
				check;
				LongDouble b(val); var.pop();
				LongDouble ans = a * b;
				var.push(ans.ld_to_string());
				break;
			}
			case '/': {
				std::string val;
				check;
				LongDouble a(val); var.pop();
				check;
				LongDouble b(val); var.pop();
				if (a.isNull()) {
					return "Error: division by zero";
				}
				LongDouble ans = b / a;
				var.push(ans.ld_to_string());
				break;
			}
			case '^': {
				std::string val;
				check;
				LongDouble a(val); var.pop();
				check;
				LongDouble b(val); var.pop();
				if (!a.isInt()) {
					return "Error: not correct power";
				}
				if (b.isNull() && a.isNegative()) {
					return "Error: division by zero";
				}
				if (b.isNull() && a.isNull()) {
					return "Error: 0 ^ 0";
				}
				LongDouble ans = b ^ a;
				var.push(ans.ld_to_string());
				break;
			}
			case ')': {
				std::string name_ = data.substr(0, 3);
				int k = data[data.size() - 2] - '0';
				if (k == 1) {
					std::string val;
					check;
					LongDouble a(val); var.pop();
					if (name_ == "sin") {
						LongDouble ans = sin(a);
						var.push(ans.ld_to_string());
					} else {
						LongDouble ans = cos(a);
						var.push(ans.ld_to_string());
					}
				} else {
					std::string val;
					check;
					LongDouble a(val); var.pop();
					check;
					LongDouble b(val); var.pop();
					if (a <= LongDouble("0") || a == LongDouble("1")) {
						return "Error: not correct log base";
					}
					if (b <= LongDouble("0")) {
						return "Error: argument log < 0";
					}

					LongDouble ans = log(b, a);
					var.push(ans.ld_to_string());
				}
				break;
			}
			default:
				std::cout << data << "\n";
				var.push(data);
				break;
			}

			data = "";
		}
	}
	if (var.empty()) return "";
	std::string val;
	check;
	return val;
}