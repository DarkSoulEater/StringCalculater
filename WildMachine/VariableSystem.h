#pragma once
#include "UndergroundType.h"

enum class VariableType {
	EmptyType = 0,
	IntType = 1,
	//DoubleType = 2,
	//LongType = 3,
	LongDoubleType = 4
};

class Variable {
public:
	Variable();
	Variable(std::string s);
	Variable(Long& other);
	~Variable();

	std::string GetString();

	void swap(Variable& a, Variable& b);

	// Assignment operator
	//Variable& operator=(Variable other);
	//Variable& operator=(Long other);

	//friend Variable operator+(Variable& a, Variable& b);

	// Tempary
	friend std::ostream& operator<<(std::ostream& out, const Variable& data) {
		//if (data.type_ == VariableType::IntType || data.type_ == VariableType::LongType) {
			//out << Long::getLong();
		//}
	}

private:
	VariableType type_;

	int int_value = 0;
	double d_value = 0;
	LongDouble* long_ = nullptr;

	std::string strval_ = "";
};

/*Variable::Variable(Long& other) {
	setLong(other);
	type_ = VariableType::LongDoubleType; // переписать на int
	strval_ = Long::LongToStr();
}

std::string Variable::GetString() {
	return strval_;
}

void Variable::swap(Variable& a, Variable& b) {
	std::swap(a.type_, b.type_);
	std::swap(a.strval_, b.strval_);
	Long::swap(a.getLong(), b.getLong());
}

// Assignment operator
Variable& Variable::operator=(Variable other) {
	swap(*this, other);
	return *this;
}
Variable& Variable::operator=(Long other) {
	Long::swap(getLong(), other);
	type_ = VariableType::LongType; // For INT
	return *this;
}

Variable operator+(Variable& a, Variable& b) {
	Variable ans;
	if (a.type_ == VariableType::IntType || a.type_ == VariableType::LongType) { // Дописать
		ans = operator+(a.getLong(), b.getLong());
	}
	ans.type_ = VariableType::LongType;
	ans.strval_ = ans.getLong().LongToStr();
	return ans;
}*/