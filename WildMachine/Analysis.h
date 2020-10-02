#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>

enum class ErrorType {
    CommaNotForArguments,

    TooManyArguments,
    TooFewArguments,

    NotFoundOpenBrackT1,
    NotFoundOpenBrackT2,
    NotFoundClosedBrackT1,
    NotFoundClosedBrackT2,
    FoundUncorrectClosedBrackT1,
    FoundUncorrectClosedBrackT2,

    NotFoundLeftOperand,
    NotFoundRightOperand,
    FoundUnknownCharacter,
    FoundIntInStr,
    FoundEmptyBrack,

    ExpectedArguments,
    ExpectedOperator,
    error
};

class Converter {
public:
    Converter(std::map<std::string, bool>& var_name);
    void addData(char x);
    void addComma();
    void addPoint();
    void addBracket(char x);
    void addOperator(char x);
    void addFunction(std::string name, char breck, int k);

    void saveData();
    void deleteData();
    std::string getData();

    void finalAnalysis(); // Check what stack empty and other...

    std::string getAns() { // Tmp funct
        return postfix_str;
    }
private:
    enum class Conv_state {
        function = 0,
        open_brack_type_1 = 1,
        open_brack_type_2 = 2,
        comma = 3,

        operator_plus = 10,
        operator_minus = 11,

        operator_multi = 20,
        operator_div = 21,
        operator_mod = 22,

        power = 30
    };
    Conv_state CharToState(char x);
    std::string StateToString(Conv_state x);

    int cnt_nested_funct_ = 0;
    std::stack<std::pair<std::string, int>> function_stack_;
    std::string var_date_ = ""; // Input date

    std::string postfix_str = ""; // Answer prefix string
    std::stack<Conv_state> state_stack_; // Stack save states

    std::map<std::string, bool>& var_name_map_;

    int cnt_point_ = 0;
};

enum class State {
    wait_state, // Независимо от предыдущих, ждем что-нить F
    open_bracket, // Ждем ввод чего-то

    const_state, // Вводим число F
    str_state, // Вводим какую-то строку F            

    comma_state, // Ввели запятую
    math_operator, // Ввели оператор
    point_state,
    e_state,

    space_before_operator // Может быть оператор, но не переменная F
};

bool isbracket(char x);
bool isoperator(char x);
bool iscomma(char x);

std::string DFA(std::string str, std::unordered_map<std::string, int>& funct_list, std::map<std::string, bool>& var_name_map);


/*#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>
#include <string>

enum class ErrorType {
    error
};

class Converter {
public:
    // Converter();
    //~Converter();

    void addDate(char x);
    std::string getData() { return var_date_; }
    void addBracket(char x) {
        int cnt_comma = 0;
        if (x == '(') {
            state_stack_.push(Conv_state::open_brack_type_1);
        } else if (x == '[') {
            state_stack_.push(Conv_state::open_brack_type_2);
        }
        
        else if (x == ')') {
            while (!state_stack_.empty()) {
                Conv_state cur_state = state_stack_.top(); state_stack_.pop();
                if (cur_state == Conv_state::open_brack_type_1) {                    
                    if (state_stack_.top() == Conv_state::function) {
                        std::string name = function_stack_.top().first;
                        int k = function_stack_.top().second;
                        function_stack_.top();

                        // Save funct
                        postfix_str += name + "(" + std::to_string(k) + ") ";
                        std::cout << "Save function: " + name + "(" + std::to_string(k) +")\n";

                        if (cnt_comma != k - 1) {
                            // Error Много или мало аргументов
                            std::cout << "Error: Many or too few argumetns\n";
                            throw ErrorType::error;
                        }

                        cnt_nested_funct_--;
                        state_stack_.pop();
                    }
                    return;
                } else if (cur_state == Conv_state::open_brack_type_2) {
                    // Error не соответсвие типов скобок
                    std::cout << "[...)\n";
                } else if (cur_state == Conv_state::comma) {
                    cnt_comma++;
                } else {
                    postfix_str += StateToString(cur_state) + " ";
                }
            }
        } else if (x == ']') {
            while (!state_stack_.empty()) {
                Conv_state cur_state = state_stack_.top(); state_stack_.pop();
                if (cur_state == Conv_state::open_brack_type_2) {
                    if (state_stack_.top() == Conv_state::function) {
                        std::string name = function_stack_.top().first;
                        int k = function_stack_.top().second;
                        function_stack_.top();

                        // Save funct
                        postfix_str += name + "[" + std::to_string(k) + "] ";
                        std::cout << "Save function: " + name + "[" + std::to_string(k) + "]\n";

                        if (cnt_comma != k - 1) {
                            // Error Много или мало аргументов
                            std::cout << "Error: Many or too few argumetns\n";
                            throw ErrorType::error;
                        }

                        cnt_nested_funct_--;
                        state_stack_.pop();
                    }
                    return;
                } else if (cur_state == Conv_state::open_brack_type_1) {
                    // Error не соответсвие типов скобок
                    std::cout << "(...]\n";
                } else if (cur_state == Conv_state::comma) {
                    cnt_comma++;
                } else {
                    postfix_str += StateToString(cur_state) + " ";
                }
            }
        }
        if (state_stack_.empty()) {
            // Error
            return;
        }

        return;
    }
    void addComma() {
        while (!state_stack_.empty()) {
            Conv_state cur_state = state_stack_.top();
            state_stack_.pop();

            if (cur_state == Conv_state::open_brack_type_1 || cur_state == Conv_state::open_brack_type_2) {
                if (state_stack_.top() != Conv_state::function) {
                    // Error common out of arguments function
                    std::cout << "Error (,): comma not in function\n";
                }
                state_stack_.push(cur_state);
                state_stack_.push(Conv_state::comma);
                return;
            } else if (cur_state == Conv_state::comma) {
                state_stack_.push(cur_state);
                state_stack_.push(cur_state);
                return;
            } else {
                postfix_str += StateToString(cur_state) + " ";
            }
        }

        if (!state_stack_.empty()) {
            // Error
            std::cout << "Error: , : not found function for ,\n";
        }
    }

    void saveDate();
    void finishAnalysis();
    void deleteData();
    //void reboot();

    void addOperator(char x);

    void addFunction(std::string name, char breck, int k) {
        cnt_nested_funct_++;
        function_stack_.push({ name, k });

        state_stack_.push(Conv_state::function);
        breck == '(' ? state_stack_.push(Conv_state::open_brack_type_1) : state_stack_.push(Conv_state::open_brack_type_2);
    }

    std::string getAns() {
        return postfix_str;
    }
private:


    enum class Conv_state {
        function = 0,
        open_brack_type_1 = 1,
        open_brack_type_2 = 2,
        comma = 3,

        operator_plus = 10,
        operator_minus = 11,

        operator_multi = 20,
        operator_div = 21,
        operator_mod = 22,

        power = 30
    };

    Conv_state CharToState(char x) {
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
    }
    
    char StateToChar(Conv_state x) {
        switch (x) {
        case Converter::Conv_state::function:
            return 'f';
            break;
        case Converter::Conv_state::open_brack_type_1:
            return '(';
            break;
        case Converter::Conv_state::open_brack_type_2:
            return '[';
            break;
        case Converter::Conv_state::operator_plus:
            return '+';
            break;
        case Converter::Conv_state::operator_minus:
            return '-';
            break;
        case Converter::Conv_state::operator_multi:
            return '*';
            break;
        case Converter::Conv_state::operator_div:
            return '/';
            break;
        case Converter::Conv_state::operator_mod:
            return '%';
            break;
        case Converter::Conv_state::power:
            return '^';
            break;
        default:
            return 'E';
            break;
        }
    }
    std::string StateToString(Conv_state x) {
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

    int cnt_nested_funct_ = 0;
    std::stack<std::pair<std::string, int>> function_stack_;
    std::string var_date_ = ""; // Input date

    std::string postfix_str = ""; // Answer prefix string
    std::stack<Conv_state> state_stack_; // Stack save states
};

void Converter::saveDate() {
    postfix_str += var_date_ + " ";
    var_date_ = "";
}

void Converter::addDate(char x) {
    var_date_ += x;
}

void Converter::deleteData() {
    var_date_ = "";
}

void Converter::addOperator(char x) {
    Conv_state state = CharToState(x);

    while (!state_stack_.empty()) {
        Conv_state upper = state_stack_.top();
        if (int(upper) > int(state) + 5) {
            postfix_str += StateToString(upper) + " ";
            state_stack_.pop();
        } else {
            break;
        }
    }

    state_stack_.push(state);
}

void Converter::finishAnalysis() {
    while (!state_stack_.empty()) {
        Conv_state upper = state_stack_.top();
        if (upper == Conv_state::open_brack_type_1 ||
            upper == Conv_state::open_brack_type_2) {
            // Error
            std::cout << "(... OR [...\n";
        }
        postfix_str += StateToString(upper) + " ";
        state_stack_.pop();
    }
}



enum class State {
    wait_state, // Независимо от предыдущих, ждем что-нить
    open_bracket,

    const_state, // Вводим число
    str_state, // Вводим какую-то строку
        // variable_state, // Это точно переменная
        function_state, // Это точно функция
        // math_const, //  Это точно мат. конст

    bracket, // Это скобка
    comma_state,
    math_operator,
    need_math_operator, // Далее необходим знак
    unneed_math_operator, // Далее возможен знак или ничего 

    space_before_operator,
    space_before_var
};

bool isbracket(char x) {
    return x == '(' || x == '[' || x == ')' || x == ']';
}
bool is_math_operator(char x) { return 0; }
bool isoperator(char x) {
    return x == '+' || x == '-' || x == '*' || x == '/' || x == '%' || x == '^';
}
bool iscomma(char x) { return x == ','; }

bool DFA(std::string str, std::unordered_map<std::string, int>& funct) {
    try {
        Converter converter;

        State state = State::wait_state;

        for (int j = 0; j < str.size(); ++j) {
            char current = str[j];

            switch (state) {
            case State::wait_state: {
                if (isspace(current)) continue;
                if (isalpha(current)) {
                    converter.addDate(current);
                    state = State::str_state;
                }
                if (isdigit(current)) {
                    converter.addDate(current);
                    state = State::const_state;
                }
                if (isbracket(current)) {
                    converter.addBracket(current);
                    state = State::open_bracket;
                }
                break;
            }

            case State::const_state: {
                if (iscomma(current)) {
                    converter.saveDate();
                    converter.addComma();
                    state = State::comma_state;
                }
                if (isspace(current)) {
                    converter.saveDate();
                    state = State::space_before_operator;
                } else if (isoperator(current)) {
                    converter.saveDate();
                    converter.addOperator(current);
                    state = State::math_operator;
                } else if (isalpha(current)) {
                    // Error Число + буква - 343а
                } else if (isdigit(current)) {
                    converter.addDate(current);
                    state = State::const_state;
                } else if (isbracket(current)) {
                    converter.saveDate();
                    if (current == '(' || current == '[') {
                        converter.addOperator('*');
                        state = State::open_bracket;
                    }
                    converter.addBracket(current);
                    state = State::space_before_operator;
                }
                break;
            }
            case State::str_state: {
                if (iscomma(current)) {
                    converter.saveDate();
                    converter.addComma();
                    state = State::comma_state;
                }
                if (isspace(current)) {
                    converter.saveDate();
                    state = State::space_before_operator;
                }
                if (isalpha(current)) { // Добавить переход к Var_state
                    converter.addDate(current);
                    state = State::str_state;
                }
                if (isdigit(current)) {
                    // Error dsds7
                    std::cout << "var_name4\n";
                }
                if (isbracket(current)) {
                    if (current == '(' || current == '[') {
                        if (funct.count(converter.getData())) {
                            // std::cout << "Find_function " + converter.getData() + "\n";
                            converter.addFunction(converter.getData(), current, funct[converter.getData()]);
                            converter.deleteData();
                            state = State::open_bracket;
                        } else {
                            converter.addBracket(current);
                            state = State::open_bracket;
                        }
                    } else {
                        converter.saveDate();
                        if (current == '(' || current == '[') {
                            converter.addOperator('*');
                            state = State::open_bracket;
                        }
                        converter.addBracket(current);
                        state = State::space_before_operator; // Добавить нужный переход при (a + b) <-
                    }
                }
                break;
            }

            case State::math_operator: {
                if (isspace(current)) continue;
                if (isalpha(current)) {
                    converter.addDate(current);
                    state = State::str_state;
                }
                if (isdigit(current)) {
                    converter.addDate(current);
                    state = State::const_state;
                }
                if (isbracket(current)) {
                    if (current == ')' || current == ']') {
                        // Error
                        std::cout << "Error: a+)\n";
                    }
                    converter.addBracket(current);
                    state = State::open_bracket;
                }
                break;
            }
            case State::open_bracket: {
                if (isspace(current)) continue;
                if (isalpha(current)) {
                    converter.addDate(current);
                    state = State::str_state;
                }
                if (isdigit(current)) {
                    converter.addDate(current);
                    state = State::const_state;
                }
                if (isbracket(current)) {
                    if (current == ')' || current == ']') {
                        // Error
                        std::cout << "Error: Empty bracket: ()\n";
                    }
                    converter.addBracket(current);
                }
                break;
            }
            case State::comma_state: {
                if (isspace(current)) continue;
                else if (isalpha(current)) {
                    converter.addDate(current);
                    state = State::str_state;
                } else if (isdigit(current)) {
                    converter.addDate(current);
                    state = State::const_state;
                } else if (isbracket(current)) {
                    if (current == ')' || current == ']') {
                        // Error
                        std::cout << "Error: (a,): not fount arguments\n";
                    }
                    converter.addBracket(current);
                }
                break;
            }
                                   //case State::variable_state: {
                                      // break;
                                   //}
            case State::bracket: {
                break;
            }
                               //case State::math_const: {
                                   //break;
                               //}

                               // Space
            case State::space_before_operator: {
                if (isspace(current)) continue;
                if (isoperator(current)) {
                    converter.addOperator(current);
                    state = State::math_operator;
                }
                // Add isComma
                // Error ожидался знак
                break;
            }
            default:
                break;
            }

            /*  Case
                if (isspace(current));
                if (isalpha(current));
                if (isdigit(current));
                if (isbracket(current));
            * /
        }

        // Ending case
        switch (state) {
        case State::wait_state:
            // Проверить кол-во вложенных функций на == 0
            break;
        case State::const_state:
            converter.saveDate();
            break;
        case State::str_state:
            converter.saveDate();
            break;
        case State::math_operator:
            // Error ожидался второй аргумент
            std::cout << "Need two arguments\n";
            break;

        case State::space_before_operator:
            break;
        case State::space_before_var:
            break;
        default:
            break;
        }

        converter.finishAnalysis();
        std::cout << converter.getAns() << "\n";
        return 1;
    } catch (const ErrorType & error) {
        std::cout << "ErrorExpection\n";
        return 0;
    }
}


*/