//
// Created by Meharjeet Singh on 2023-11-26.
//

#include "ArithmeticCode.h"


using std::string;

void encode(ArithmeticCode &code) {
	// GIVEN:
	// 1) input is a span of characters
	// 2) Only has 2 Distinct Characters A and B
	// 3) Probability of A is 0.5 and B is 0.5
	// 4) input length <= 16

	code.output = "";
	code.a = 0;
	code.b = 1;
	for (char c: code.input) {
		const auto width = code.b - code.a;
		c = std::toupper(c);
		if (c == 'A') {
			code.b = code.a + width * 0.5;
			code.output.push_back('0');
		} else if (c == 'B') {
			code.a = code.a + width * 0.5;
			code.output.push_back('1');
		} else {
			throw std::invalid_argument("Invalid Character");
		}
	}
}