//
// Created by Meharjeet Singh on 2023-11-26.
//

#ifndef Q1_ARITHMETICCODE_H
#define Q1_ARITHMETICCODE_H

#include <string>
#include <span>
#include <utility>
#include <cmath>

struct ArithmeticCode {
	long double a, b;
	std::string input;
	std::string output;
};

void encode(ArithmeticCode &code);

#endif //Q1_ARITHMETICCODE_H
