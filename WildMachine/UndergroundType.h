#pragma once
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include "Setting.h"

class Int {
public:
	Int();
	~Int();

	void SetIntValue(int64_t value);
	// int GetValue();

	void Int_Claer();

private:
	int64_t *int_ = nullptr;
};

class Double {
public:
	//Double();
	//~Double();

	//void SetDoubleValue(double value);

private:
	double* double_ = nullptr;
};

class Long {
public:
	// Constructor
	Long();
	Long(int64_t size); // Default size constructor
	Long(std::string s); // Defautlt Constructor
	Long(const Long& other); // Copy Constructor
	Long(Long&& other); // Move Constructor

	// Destructor
	~Long(); 

	// Set and Get
	void setLong(std::string s);
	void setLong(const Long& other);
	void clearLong();
	Long& getLong() {
		return *this;
	}

	// Servise function
	void swap(Long& lhs, Long& rhs);
	void DeleteForwardZero();
	void Equalization(Long &a, Long &b);
	std::string LongToStr();

	// Ñomparison operator
	friend bool operator==(const Long& a, const Long& b);
	friend bool operator!=(const Long& a, const Long& b);
	friend bool operator<(const Long& a, const Long& b);
	friend bool operator<=(const Long& a, const Long& b);
	friend bool operator>(const Long& a, const Long& b);
	friend bool operator>=(const Long& a, const Long& b);

	// Assignment operator
	Long& operator=(Long other);
	//Long& operator+=(Long other);
	//Long& operator-=(Long other);

	// Math operator
	friend Long operator+(const Long& a, const Long& b);
	friend Long operator-(const Long& a, const Long& b);
	friend Long operator*(const Long& a, const Long& b);
	friend Long operator/(const Long& a, const Long& b);
	//friend Long operator%(const Long& a, const Long& b);
	friend Long operator-(const Long& a);

	
	// Temporary
	friend std::ostream& operator<< (std::ostream &out, const Long& date) {
		if (date.sign_) out << '-';
		for (int64_t i = date.size_ - 1; i >= 0; --i) {
			if (i != 0)out << std::setw(9) << std::setfill('0') << date.long_[i];
			else out << date.long_[i];
		}
		return out;
	}
	friend Long TPOW(const Long& a, const Long& b) {
		//int N = std::max(A.size_, B.size_);
		//Long a(N); Long b(N);
		//std::copy(A.long_, A.long_ + A.size_, a.long_);
		//std::copy(B.long_, B.long_ + B.size_, b.long_);

		Long result(a.size_ + b.size_ + 5);
		result.sign_ = a.sign_ ^ b.sign_;

		for (int j = 0; j < a.size_; ++j) {
			for (int i = 0; i < b.size_; ++i) {
				result.long_[j + i] += a.long_[j] * b.long_[i];
			}

			for (int i = 0; i < result.size_; ++i) {
				while (result.long_[i] >= result.base_) {
					result.long_[i] -= result.base_;
					result.long_[i + 1] += 1;
				}
			}
		}

		result.DeleteForwardZero();
		return result;
	}

private: 
	//int64_t base_ = 2147483646;
	//int64_t base_ = 10;
	int64_t base_ = 1e9;
	int64_t *long_ = nullptr;
	int64_t size_ = 0;
	bool sign_ = 0;
};

class LongDouble {
public:
	LongDouble();
	LongDouble(std::string str);
	LongDouble(const LongDouble& other);
	LongDouble(LongDouble&& other);
	~LongDouble();

	// Assignment operator
	LongDouble& operator=(LongDouble other);

	// Comparison operators
	bool friend operator==(const LongDouble& a, const LongDouble& b);
	bool friend operator!=(const LongDouble& a, const LongDouble& b);
	bool friend operator<(const LongDouble& a, const LongDouble& b);
	bool friend operator<=(const LongDouble& a, const LongDouble& b);
	bool friend operator>(const LongDouble& a, const LongDouble& b);
	bool friend operator>=(const LongDouble& a, const LongDouble& b);

	// Math operators
	LongDouble friend operator+(const LongDouble& a, const LongDouble& b);
	LongDouble friend operator-(const LongDouble& a, const LongDouble& b);
	LongDouble friend operator*(const LongDouble& a, const LongDouble& b);
	LongDouble friend operator/(LongDouble a, LongDouble b);
	LongDouble friend operator-(const LongDouble& a);
	LongDouble friend operator^(LongDouble a, LongDouble b);

	// Math function
	LongDouble friend sin(LongDouble& a) {
		double A = a.toDouble();
		//std::cout << A << "\n";
		return std::to_string(sin(A));
	}
	LongDouble friend cos(LongDouble& a) {
		double A = a.toDouble();
		//std::cout << A << "\n";
		return std::to_string(cos(A));
	}
	LongDouble friend log(LongDouble& a, LongDouble& b) {
		double A = a.toDouble(), B = b.toDouble();
		//std::cout << A << "\n";
		return std::to_string(log(A) / log(B));
	}

	std::string ld_to_string() {
		std::string str = "";
		if (sign_) str += "-";
		Node* ptr = head_;
		while (ptr != nullptr) {
			str += std::to_string(ptr->val_);
			if (ptr == point_ && fract_part_size != 0) str += ".";
			ptr = ptr->prev;
		}
		return str;
	}
	std::string part_size_to_string() {
		return "front_size: " + std::to_string(int_part_size) + " back_size: " + std::to_string(fract_part_size);
	}
	void delete_forward_zeros() {
		while (head_ != point_ && head_->val_ == 0) pop_front();
		if (head_ == point_ && head_->val_ == 0) sign_ = 0;
	}
	void delete_backward_zeros() {
		while (tail_ != point_ && tail_->val_ == 0) pop_back();
	}
	void delete_minor_zeros() {
		while (head_ != point_ && head_->val_ == 0) pop_front();
		while (tail_ != point_ && tail_->val_ == 0) pop_back();
		if (head_ == point_ && head_->val_ == 0) sign_ = 0;
	}
	friend void swap(LongDouble &a, LongDouble &b) {
		std::swap(a.int_part_size, b.int_part_size);
		std::swap(a.fract_part_size, b.fract_part_size);
		std::swap(a.sign_, b.sign_);
		std::swap(a.point_, b.point_);
		std::swap(a.head_, b.head_);
		std::swap(a.tail_, b.tail_);
	}
	void normalization() {
		Node* ptr = tail_;
		while (ptr != nullptr) {
			while (ptr->val_ > 9) {
				ptr->val_ -= 10;
				if (ptr->next != nullptr) {
					ptr->next->val_ += 1;
				} else {
					push_front(1);
				}
			}
			while (ptr->val_ < 0) {
				ptr->val_ += 10;
				if (ptr->next != nullptr) {
					ptr->next->val_ -= 1;
				} else {
					std::cout << "Error: not found hight digit";
					throw "Error: not found hight digit";
				}
			}
			ptr = ptr->next;
		}
	}
	void point_shift(int shift) {
		if (shift > 0) {
			while (shift) {
				if (point_->next == nullptr) push_front(0);
				point_ = point_->next;
				++fract_part_size;
				--int_part_size;
				--shift;
			}
		} else {
			while (shift) {
				if (point_->prev == nullptr) push_back(0);
				point_ = point_->prev;
				++int_part_size;
				--fract_part_size;
				++shift;
			}
		}
	}
	bool isInt() {
		return (fract_part_size == 0);
	}
	bool isNull() {
		if (fract_part_size == 0 && int_part_size == 1) {
			if (point_->val_ == 0) {
				return 1;
			}
		}
		return 0;
	}
	bool isNegative() {
		return sign_;
	}
	double toDouble() {
		double a = 0;
		Node* ptr = head_;
		while (ptr != point_) {
			a *= 10;
			a += ptr->val_;
			ptr = ptr->prev;
		}
		a *= 10;
		a += ptr->val_;
		ptr = ptr->prev;

		int k = 0;
		while (ptr != nullptr) {
			a *= 10;
			a += ptr->val_;
			k += 1;
			ptr = ptr->prev;
		}
		if (k) a /= pow(10, k);
		if (sign_) a *= -1;
		return a;
	}
private:
	struct Node {
		int val_ = 0;
		Node* next = nullptr, * prev = nullptr;
	};
	Node* point_ = nullptr, *head_ = nullptr, *tail_ = nullptr;
	int64_t fract_part_size = 0;
	int64_t int_part_size = 0;
	bool sign_ = 0;

	// low_level operation
	void push_front(int val) {
		Node* ptr = new Node;
		ptr->val_ = val;
		ptr->next = nullptr;
		ptr->prev = head_;

		head_ == nullptr ? head_ = tail_ = point_ = ptr : head_->next = ptr, head_ = ptr;
		++int_part_size;
	}
	void push_back(int val) {
		Node* ptr = new Node;
		ptr->val_ = val;
		ptr->prev = nullptr;
		ptr->next = tail_;

		tail_ == nullptr ? head_ = tail_ = point_ = ptr : tail_->prev = ptr, tail_ = ptr;
		++fract_part_size;
	}
	void pop_front() {
		if (head_ != point_) {
			head_ = head_->prev;
			delete head_->next;
			head_->next = nullptr;
			--int_part_size;
		} else {
			std::cout << "Error: delete last digit";
			throw "Error: delete last digit";
		}
	}
	void pop_back() {
		if (tail_ != point_) {
			tail_ = tail_->next;
			delete tail_->prev;
			tail_->prev = nullptr;
			--fract_part_size;
		} else {
			std::cout << "Error: delete last digit";
			throw "Error: delete last digit";
		}
	}
};