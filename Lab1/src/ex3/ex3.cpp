#include "ex3.hpp"

glm::vec3 rodriguesRotation(glm::vec3 v, glm::vec3 a, float theta) {
    return glm::cos(glm::radians(theta)) * v +
           glm::cross(a,v) * glm::sin(glm::radians(theta)) +
           a * glm::dot(a, v) * (1 - glm::cos(glm::radians(theta)));
}

void ex3() {
    print_header("Exercise 3");
    glm::vec3 v = glm::vec3(1.0f,1.0f,0);
    glm::vec3 a = glm::vec3(0,0,1.0f);
    glm::vec3 expectedV = glm::vec3(0,1.41421f, 0);
    std::cout << "initial v: " << glm::to_string(v) << std::endl;
    std::cout << "a: " << glm::to_string(a) << std::endl;
    for (int i = 0; i < 9; i++) {
        v = rodriguesRotation(v,a,45.0f);
        std::cout << "v after " << i+1 << " rotations of 45 degrees on z: " << glm::to_string(v) << std::endl;
    }
    std::cout << "expected v: " << glm::to_string(expectedV) << std::endl;
    std::cout << "final v == expected v: " << vectorEquality(v,expectedV) << std::endl;
    assert(vectorEquality(v,expectedV));
    print_header("Exercise 3 completed");
}