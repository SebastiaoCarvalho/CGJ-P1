#include "ex2.hpp"

void testCoordinateFrames(glm::vec3 up, glm::vec3 view, glm::vec3 expectedV, glm::vec3 expectedW, glm::vec3 expectedU) {
	glm::vec3 v = glm::normalize(view);
	glm::vec3 w = glm::normalize(glm::cross(up, v));
	glm::vec3 u = glm::cross(v, w);

    // assert properties
    std::cout << "|v| == 1? " << (glm::length(v) == 1) << std::endl;
	assert(glm::length(v) == 1);
    std::cout << "|w| == 1? " << (glm::length(w) == 1) << std::endl;
	assert(glm::length(w) == 1);
    std::cout << "|u| == 1? " << (glm::length(u) == 1) << std::endl;
	assert(glm::length(u) == 1);
    std::cout << "u.v == 0? " << (glm::dot(u, v) == 0) << std::endl;
	assert(glm::dot(u, v) == 0);
    std::cout << "u.w == 0? " << (glm::dot(u, w) == 0) << std::endl;
	assert(glm::dot(u, w) == 0);
    std::cout << "v.w == 0? " << (glm::dot(v, w) == 0) << std::endl;
	assert(glm::dot(v, w) == 0);
    std::cout << "u x v == w? " << vectorEquality(glm::cross(u, v), w) << std::endl;
	assert(vectorEquality(glm::cross(u, v), w));
	
	// assert values

    std::cout << "v == expected v?" << vectorEquality(v, expectedV) << std::endl;
	assert(vectorEquality(v, expectedV));
    std::cout << "w == expected w?" << vectorEquality(w, expectedW) << std::endl;
	assert(vectorEquality(w, expectedW));
    std::cout << "u == expected u?" << vectorEquality(u, expectedU) << std::endl;
	assert(vectorEquality(u, expectedU));
}

void ex2() {
    print_header("Exercise 2");
    std::cout << "Testing exercise 2 with up: " << glm::to_string(glm::vec3(0.7, 0.7, 0)) << ", view: " 
        << glm::to_string(glm::vec3(1, 0, 0)) << std::endl;
    testCoordinateFrames(glm::vec3(0.7, 0.7, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    std::cout << "Testing exercise 2 with up: " << glm::to_string(glm::vec3(0.5, 0.5, 0)) << ", view: " 
        << glm::to_string(glm::vec3(2, 0, 0)) << std::endl;
    testCoordinateFrames(glm::vec3(0.5, 0.5, 0), glm::vec3(2, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    print_header("Exercise 2 successful");
}