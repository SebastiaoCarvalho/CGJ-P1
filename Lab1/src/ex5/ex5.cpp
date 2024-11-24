#include "ex5.hpp"

bool transpose_property(glm::mat3 m1, glm::mat3 m2) {
	glm::mat3 side1 = glm::transpose(m1 * m2);
	std::cout << "left side: " << glm::to_string(side1) << std::endl;
	glm::mat3 side2 = glm::transpose(m2) * glm::transpose(m1);
	std::cout << "right side: " << glm::to_string(side2) << std::endl;
	std::cout << "Equality: " << matrixEquality(side1, side2) << std::endl;
	return matrixEquality(side1, side2);
}

void ex5() {
	print_header("Exercise 5");
	std::cout << "Testing exercise 5 with random matrices." << std::endl;
	assert(transpose_property(randomMat3(), randomMat3()));
	print_header("Exercise 5 completed");
}