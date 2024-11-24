#include "ex5.hpp"

bool transpose_property(glm::mat4 m1, glm::mat4 m2) {
	glm::mat4 side1 = glm::transpose(m1 * m2);
	glm::mat4 side2 = glm::transpose(m2) * glm::transpose(m1);
	return matrixEquality(side1, side2);
}

void ex5() {
	print_header("Exercise 5");
	assert(transpose_property(randomMat3(), randomMat3()));
}