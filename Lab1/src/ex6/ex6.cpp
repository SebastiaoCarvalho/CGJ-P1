#include "ex6.hpp"

bool inverseProperty(glm::mat3 m1, glm::mat3 m2) {
	glm::mat3 side1 = glm::inverse(m1 * m2);
	std::cout << "(A * B)^-1: " << glm::to_string(side1) << std::endl;
	glm::mat3 side2 = glm::inverse(m2) * glm::inverse(m1);
	std::cout << "A^-1 * B^-1: " << glm::to_string(side2) << std::endl;
	std::cout << "(A * B)^-1 == A^-1 * B^-1: " << matrixEquality(side1, side2) << std::endl;
	return matrixEquality(side1, side2);
}

void ex6() {
	print_header("Exercise 6");
	std::cout << "Testing exercise 6 with random matrices." << std::endl;
	assert(inverseProperty(randomMat3(), randomMat3()));
	print_header("Exercise 6 completed");
}