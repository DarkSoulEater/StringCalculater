#pragma once
#include "UndergroundType.h"

// Class Int
Int::Int() {

}

Int::~Int() {

}

void Int::SetIntValue(int64_t value) {
    if (int_ == nullptr) {
        int_ = new int64_t;
    }
    *int_ = value;
}

void Int::Int_Claer() {
    //int_ = 
}


// Class Double


// Class Long
// Constructor
Long::Long() {
	size_ = 0;
	long_ = nullptr;
	sign_ = 0;
}
Long::Long(int64_t size) {
	if (size == 0) return;
	size_ = size;
	long_ = new int64_t[size_];
	for (int i = 0; i < size_; ++i) {
		long_[i] = 0;
	}
}
Long::Long(std::string s) {
	size_ = s.size() / 9 + (s.size() % 9? 1 : 0);
	if (s[0] == '-') {
		sign_ = 1;
	}
	long_ = new int64_t[size_];

	for (int i = 0; i < size_; ++i) {
		long_[i] = 0;
	}

	for (int64_t i = 0; i < s.size(); ++i) {
		if (s[i] == '-') continue;
		long_[(s.size() - 1 - i) / 9] *= 10;  
		long_[(s.size() - 1 - i) / 9] += int64_t(s[i] - '0');  
	}
	DeleteForwardZero();
}
Long::Long(const Long& other) {
	size_ = other.size_;
	sign_ = other.sign_;
	long_ = new int64_t[size_];
	std::copy(other.long_, other.long_ + size_, long_);
}
Long::Long(Long&& other) {
	size_ = std::exchange(other.size_, 0LL);
	long_ = std::exchange(other.long_, nullptr);
	sign_ = std::exchange(other.sign_, 0LL);
}
	
// Destructor
Long::~Long() {
	if (long_ != nullptr) delete[] long_;
}

// Set and Get
void Long::setLong(std::string s) {
	size_ = s.size() / 9 + (s.size() % 9 ? 1 : 0);
	if (s[0] == '-') {
		sign_ = 1;
	}
	long_ = new int64_t[size_];

	for (int i = 0; i < size_; ++i) {
		long_[i] = 0;
	}

	for (int64_t i = 0; i < s.size(); ++i) {
		if (s[i] == '-') continue;
		long_[(s.size() - 1 - i) / 9] *= 10;
		long_[(s.size() - 1 - i) / 9] += int64_t(s[i] - '0');
	}
	DeleteForwardZero();
}
void Long::setLong(const Long& other) {
	size_ = other.size_;
	sign_ = other.sign_;
	long_ = new int64_t[size_];
	std::copy(other.long_, other.long_ + size_, long_);
}
void Long::clearLong() {
	size_ = sign_ = 0;
	if (long_ != nullptr) delete[] long_;
	long_ = nullptr;
}

// Service function
void Long::swap(Long& lhs, Long& rhs) {
	std::swap(lhs.size_, rhs.size_);
	std::swap(lhs.long_, rhs.long_);
	std::swap(lhs.sign_, rhs.sign_);
}
void Long::DeleteForwardZero() {
	while (size_ > 1 && !long_[size_ - 1]) {
		--size_;
	}
	if (size_ == 1 && long_[0] == 0) sign_ = 0;
}
void Long::Equalization(Long& a, Long& b) {
	if (a.size_ > b.size_) {
		Long c(a.size_);
		c = b;
		swap(b, c);
	} else if (a.size_ < b.size_) {
		Long c(b.size_);
		c = a;
		swap(a, c);
	}
}
std::string Long::LongToStr() {	
	std::string str = "";
	if (sign_) str += "-";
	for (int64_t i = size_ - 1; i >= 0; --i) {
		std::string x = std::to_string(long_[i]);
		if (i != size_ - 1) {
			while (x.size() < 9) {
				x = "0" + x; 
			}
		}
		str += x;
	}
	return str;
}

// Assignment operator
Long& Long::operator=(Long other) {
	swap(*this, other);
	return *this;
}
//Long& operator+=(Long other);
//Long& operator-=(Long other);

// Comparison operator
bool operator==(const Long& a, const Long& b) {
	if (a.sign_ != b.sign_ || a.size_ != b.size_) return 0;
	for (int i = 0; i < a.size_; ++i) {
		if (a.long_[i] != b.long_[i]) return 0;
	}
	return 1;
}
bool operator!=(const Long& a, const Long& b) {
	return !(a == b);
}
bool operator<(const Long& a, const Long& b) {
	if (a.sign_ != b.sign_) return a.sign_;
	if (a.size_ != b.size_) {
		if (a.sign_) return a.size_ > b.size_;
		return a.size_ < b.size_;
	}
	for (int64_t i = a.size_ - 1; i >= 0; --i) {
		if (a.long_[i] < b.long_[i]) return !a.sign_;
		if (a.long_[i] > b.long_[i]) return a.sign_;
	}
	return 0;
}
bool operator<=(const Long& a, const Long& b) {
	if (a.sign_ != b.sign_) return a.sign_;
	if (a.size_ != b.size_) {
		if (a.sign_) return a.size_ > b.size_;
		return a.size_ < b.size_;
	}
	for (int64_t i = a.size_ - 1; i >= 0; --i) {
		if (a.long_[i] < b.long_[i]) return !a.sign_;
		if (a.long_[i] > b.long_[i]) return a.sign_;
	}
	return 1;
}
bool operator>(const Long& a, const Long& b) {
	if (a.sign_ != b.sign_) return b.sign_;
	if (a.size_ != b.size_) {
		if (a.sign_) return a.size_ < b.size_;
		return a.size_ > b.size_;
	}
	for (int64_t i = a.size_ - 1; i >= 0; --i) {
		if (a.long_[i] < b.long_[i]) return a.sign_;
		if (a.long_[i] > b.long_[i]) return !a.sign_;
	}
	return 0;
}
bool operator>=(const Long& a, const Long& b) {
	if (a.sign_ != b.sign_) return b.sign_;
	if (a.size_ != b.size_) {
		if (a.sign_) return a.size_ < b.size_;
		return a.size_ > b.size_;
	}
	for (int64_t i = a.size_ - 1; i >= 0; --i) {
		if (a.long_[i] < b.long_[i]) return a.sign_;
		if (a.long_[i] > b.long_[i]) return !a.sign_;
	}
	return 1;
}

// Math operator
Long operator+(const Long& a, const Long& b) {
	Long result(std::max(a.size_, b.size_) + 1LL);
	if (a.sign_ == b.sign_) {
		result.sign_ = a.sign_;
		std::copy(a.long_, a.long_ + a.size_, result.long_);
		for (int i = 0; i < b.size_; ++i) {
			result.long_[i] += b.long_[i];
			while (result.long_[i] >= result.base_) {
				result.long_[i] -= result.base_;
				result.long_[i + 1] += 1;
			}
		}
	} else result = a - (-b);

	result.DeleteForwardZero();
	return result;
}
Long operator-(const Long& a, const Long& b) {
	Long result(std::max(a.size_, b.size_) + 1);
	if (a.sign_ == b.sign_) {
		if (a.sign_) {
			if (a > b) {
				result = b - a;
				result.sign_ = 0;
			} else {
				result.sign_ = 1;
				std::copy(a.long_, a.long_ + a.size_, result.long_);
				for (int i = 0; i < a.size_; ++i) {
					if (i < b.size_) {
						result.long_[i] -= b.long_[i];
					}
					while (result.long_[i] < 0LL) {
						result.long_[i] += result.base_;
						result.long_[i + 1] -= 1LL;
					}
				}
			}
		} else {
			if (a < b) {
				result = b - a;
				result.sign_ = 1;
			} else {
				result.sign_ = 0;
				std::copy(a.long_, a.long_ + a.size_, result.long_);
				for (int i = 0; i < a.size_; ++i) {
					if (i < b.size_) {
						result.long_[i] -= b.long_[i];
					}
					while (result.long_[i] < 0LL) {
						result.long_[i] += result.base_;
						result.long_[i + 1] -= 1LL;
					}
				}
			}
		}
	} else result = a + (-b);

	result.DeleteForwardZero();
	return result;
}
Long operator*(const Long& A, const Long& B) {
	int N = std::max(A.size_, B.size_);
	Long a(N); Long b(N);
	std::copy(A.long_, A.long_ + A.size_, a.long_);
	std::copy(B.long_, B.long_ + B.size_, b.long_);

	Long result(a.size_ + b.size_ + 5);
	result.sign_ = A.sign_ ^ B.sign_;
	
	if (result.size_ < MIN_LENGHT_FOR_KARATSUBA) {
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
	} else {
		Long a_smal_part;
		a_smal_part.long_ = a.long_;
		a_smal_part.size_ = (a.size_ + 1) / 2;

		Long a_big_part;
		a_big_part.long_ = a.long_ + a_smal_part.size_;
		a_big_part.size_ = a.size_ / 2;

		Long b_smal_part;
		b_smal_part.long_ = b.long_;
		b_smal_part.size_ = (b.size_ + 1) / 2;

		Long b_big_part;
		b_big_part.long_ = b.long_ + b_smal_part.size_;
		b_big_part.size_ = b.size_ / 2;

		Long ab_smal = a_smal_part * b_smal_part;
		Long ab_big = a_big_part * b_big_part;
		Long ab = (a_smal_part + a_big_part) * (b_smal_part + b_big_part) - ab_smal - ab_big;

		int64_t n = a_smal_part.size_;
		std::copy(ab_smal.long_, ab_smal.long_ + ab_smal.size_, result.long_);
		std::copy(ab_big.long_, ab_big.long_ + ab_big.size_, result.long_ + 2*n);
		for (int i = 0; i < ab.size_; ++i) {
			result.long_[i + n] += ab.long_[i];
			while (result.long_[i + n] >= result.base_) {
				result.long_[i + n] -= result.base_;
				result.long_[i + n + 1] += 1;
			}
		}

		// Delete
		a_smal_part.long_ = nullptr;
		a_big_part.long_ = nullptr;
		b_smal_part.long_ = nullptr;
		b_big_part.long_ = nullptr;
	}

	result.DeleteForwardZero();
	return result;
}
Long operator/(const Long& a, const Long& b) {
	Long result(a.size_);
	result.sign_ = a.sign_ ^ b.sign_;
	//Long current(value);

	result.DeleteForwardZero();
	return result;
}
//Long operator%(const Long& a, const Long& b);
Long operator-(const Long& a) {
	Long result(a);
	result.sign_ ^= 1;
	return result;
}


// Class LongDouble
// Constructor
LongDouble::LongDouble() {
	point_ = head_ = tail_ = new Node;
	int_part_size = 1;
}
LongDouble::LongDouble(std::string str) {
	for (int64_t i = str.size() - 1; i >= 0; --i) {
		if (str[i] == '-') {
			sign_ = 1;
		} else if (str[i] == '.') {
			push_front(str[--i] - '0');
			point_ = head_;
			fract_part_size = int_part_size - 1;
			int_part_size = 1;
		} else {
			push_front(str[i] - '0');
		}
	}
	delete_minor_zeros();
}
LongDouble::LongDouble(const LongDouble& other) {
	int_part_size = other.int_part_size;
	fract_part_size = other.fract_part_size;
	sign_ = other.sign_;
	
	Node* ptr = other.tail_;
	while (ptr != nullptr) {
		push_front(ptr->val_);
		if (ptr == other.point_) {
			point_ = head_;
			fract_part_size = int_part_size - 1;
			int_part_size = 1;
		}
		ptr = ptr->next;
	}
}
LongDouble::LongDouble(LongDouble&& other) {
	int_part_size = std::exchange(other.int_part_size, 0);
	fract_part_size = std::exchange(other.fract_part_size, 0);
	sign_ = std::exchange(other.sign_, 0);
	head_ = std::exchange(other.head_, nullptr);
	point_ = std::exchange(other.point_, nullptr);
	tail_ = std::exchange(other.tail_, nullptr);
}

// Destructor
LongDouble::~LongDouble() {
	while (head_ != nullptr) {
		Node* ptr = head_->prev;
		delete head_;
		head_ = ptr;
	}
	tail_ = point_ = head_;
}

// Assignment operator
LongDouble& LongDouble::operator=(LongDouble other) {
	swap(*this, other);
	return *this;
}
//Long& operator+=(Long other);
//Long& operator-=(Long other);

// Comparison operators
bool operator==(const LongDouble& a, const LongDouble& b) {
	if (a.sign_ != b.sign_) return 0;
	if (a.int_part_size != b.int_part_size || a.fract_part_size != b.fract_part_size) return 0;
	LongDouble::Node* ptr1 = a.head_, *ptr2 = b.head_;
	while (ptr1 != nullptr && ptr2 != nullptr) {
		if (ptr1->val_ != ptr2->val_) return 0;
		ptr1 = ptr1->prev;
		ptr2 = ptr2->prev;
	}
	return 1;
}
bool operator!=(const LongDouble& a, const LongDouble& b) {
	return !(a == b);
}
bool operator<(const LongDouble& a, const LongDouble& b) {
	if (a.sign_ != b.sign_) return a.sign_;
	if (a.int_part_size != b.int_part_size) {
		if (a.sign_) return a.int_part_size > b.int_part_size;
		return a.int_part_size < b.int_part_size;
	}
	LongDouble::Node* ptra = a.head_, *ptrb = b.head_;
	while (ptra != nullptr && ptrb != nullptr) {
		if (ptra->val_ < ptrb->val_) return !a.sign_;
		if (ptra->val_ > ptrb->val_) return a.sign_;
		ptra = ptra->prev, ptrb = ptrb->prev;
	}

	if (ptra == nullptr && ptrb == nullptr) return 0;
	else if (ptra == nullptr) return !a.sign_;
	else return a.sign_;
}
bool operator<=(const LongDouble& a, const LongDouble& b) {
	return !(a > b);
}
bool operator>(const LongDouble& a, const LongDouble& b) {
	if (a.sign_ != b.sign_) return b.sign_;
	if (a.int_part_size != b.int_part_size) {
		if (a.sign_) return a.int_part_size < b.int_part_size;
		return a.int_part_size > b.int_part_size;
	}
	LongDouble::Node* ptra = a.head_, * ptrb = b.head_;
	while (ptra != nullptr && ptrb != nullptr) {
		if (ptra->val_ < ptrb->val_) return a.sign_;
		if (ptra->val_ > ptrb->val_) return !a.sign_;
		ptra = ptra->prev, ptrb = ptrb->prev;
	}

	if (ptra == nullptr && ptrb == nullptr) return 0;
	else if (ptra == nullptr) return a.sign_;
	else return !a.sign_;
}
bool operator>=(const LongDouble& a, const LongDouble& b) {
	return !(a < b);
}

// Math operators
LongDouble operator+(const LongDouble& a, const LongDouble& b) {
	LongDouble ans;
	if (a.sign_ == b.sign_) {
		ans.sign_ = a.sign_;
		ans.point_->val_ = a.point_->val_ + b.point_->val_;

		LongDouble::Node* ptra = a.point_->prev, * ptrb = b.point_->prev;
		while (ptra != nullptr || ptrb != nullptr) {
			ans.push_back(0);
			if (ptra != nullptr) {
				ans.tail_->val_ += ptra->val_;
				ptra = ptra->prev;
			}
			if (ptrb != nullptr) {
				ans.tail_->val_ += ptrb->val_;
				ptrb = ptrb->prev;
			}
		}

		ptra = a.point_->next, ptrb = b.point_->next;
		while (ptra != nullptr || ptrb != nullptr) {
			ans.push_front(0);
			if (ptra != nullptr) {
				ans.head_->val_ += ptra->val_;
				ptra = ptra->next;
			}
			if (ptrb != nullptr) {
				ans.head_->val_ += ptrb->val_;
				ptrb = ptrb->next;
			}
		}

		ans.normalization();
		ans.delete_minor_zeros();
	} else {
		ans = a - (-b);
	}

	return ans;
}
LongDouble operator-(const LongDouble& a, const LongDouble& b) {
	LongDouble ans;
	if (a.sign_ == b.sign_) {
		if (a != b && a.sign_ ^ (a < b)) {
			ans = b - a;
			ans.sign_ = !a.sign_;
		} else {
			ans.sign_ = a.sign_;
			ans.point_->val_ = a.point_->val_ - b.point_->val_;

			LongDouble::Node* ptra = a.point_->prev, * ptrb = b.point_->prev;
			while (ptra != nullptr || ptrb != nullptr) {
				ans.push_back(0);
				if (ptra != nullptr) {
					ans.tail_->val_ += ptra->val_;
					ptra = ptra->prev;
				}
				if (ptrb != nullptr) {
					ans.tail_->val_ -= ptrb->val_;
					ptrb = ptrb->prev;
				}
			}

			ptra = a.point_->next, ptrb = b.point_->next;
			while (ptra != nullptr || ptrb != nullptr) {
				ans.push_front(0);
				if (ptra != nullptr) {
					ans.head_->val_ += ptra->val_;
					ptra = ptra->next;
				}
				if (ptrb != nullptr) {
					ans.head_->val_ -= ptrb->val_;
					ptrb = ptrb->next;
				}
			}

			ans.normalization();
			ans.delete_minor_zeros();
		}
	} else {
		ans = a + (-b);
	}

	return ans;
}
LongDouble operator*(const LongDouble& a, const LongDouble& b) {
	LongDouble ans;
	LongDouble::Node* ptr_ans = ans.tail_;
	LongDouble::Node* ptrb = b.tail_;
	while (ptrb != nullptr) {
		LongDouble::Node* ptra = a.tail_, *ptr_ins = ptr_ans;
		while (ptra != nullptr) {
			ptr_ins->val_ += ptra->val_ * ptrb->val_;
			ptra = ptra->next;

			if (ptr_ins->next == nullptr) ans.push_front(0);
			ptr_ins = ptr_ins->next;
		}

		ptrb = ptrb->next;
		if (ptr_ans->next == nullptr) ans.push_front(0);
		ptr_ans = ptr_ans->next;
	}

	ans.point_shift(a.fract_part_size + b.fract_part_size);
	ans.sign_ = a.sign_ ^ b.sign_;
	ans.normalization();
	ans.delete_minor_zeros();
	return ans;
}
LongDouble operator/(LongDouble a, LongDouble b) {
	LongDouble up, ans, zero;
	LongDouble::Node* now_a = a.head_;
	int64_t accuracy = 0;
	bool shift = 1;
	while (b.point_ != b.tail_) {
		//std::cout << "failed\n";
		b.point_shift(-1);
		a.point_shift(-1);
	}
	while (now_a != nullptr || up != zero) { // 
		if (!shift && accuracy++ > 30) break; // Accuracy limit

		up.push_back(now_a != nullptr ? now_a->val_ : 0);
		up.point_shift(-1);
		up.delete_minor_zeros();

		int k = 0;
		while (up >= b) {
			up = up - b; // up -= b;
			++k;
		}
		ans.push_back(k);
		if (shift) ans.point_shift(-1);

		if (now_a == a.point_) shift = 0;
		if (now_a != nullptr) now_a = now_a->prev;
		else shift = 0;
	}

	ans.normalization(); // 
	ans.delete_minor_zeros();
	return ans;
}
LongDouble operator-(const LongDouble& a) {
	LongDouble result (a);
	result.sign_ ^= 1;
	return result;
}
LongDouble operator^(LongDouble a, LongDouble b) {
	a.point_shift(-1);
	LongDouble null("0"), ans("1");
	bool f = 0;
	if (b.sign_) {
		b.sign_ = 0;
		f = 1;
	}
	while (b != null) {
		//std::cout << ans.ld_to_string() << " " << a.ld_to_string() << "\n";
		//std::cout << (ans * a).ld_to_string() << "\n";
		//std::cout << (LongDouble("2") * LongDouble("2")).ld_to_string() << "\n";
		ans = a * ans;

		b = b - LongDouble("1");
	}
	if (f) ans = LongDouble("1") / ans;
	return ans;
}