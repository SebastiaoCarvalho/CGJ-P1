#include "ex1.hpp"

bool tripleProductProposition(glm::vec3 i, glm::vec3 j, glm::vec3 k) {
	glm::vec3 crossProduct = glm::cross(i, glm::cross(j, k));
	std::cout << "Left Side: " << glm::to_string(crossProduct) << std::endl;
	glm::vec3 dotProduct = j * glm::dot(i, k) - k * glm::dot(i, j);
	std::cout << "Right Side: " << glm::to_string(dotProduct) << std::endl;
	return vectorEquality(crossProduct, dotProduct);
}

void ex1() {
	print_header("Exercise 1");
    std::cout << "Testing exercise 1 with random vectors." << std::endl;
	assert(tripleProductProposition(randomVec(), randomVec(), randomVec()));
   	print_header("Exercise 1 successful");
}