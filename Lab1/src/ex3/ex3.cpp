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
    glm::vec3 out = glm::vec3(0,1.41421f, 0);
    for (int i = 0; i < 9; i++)
        v = rodriguesRotation(v,a,45.0f);
    assert(glm::all(glm::epsilonEqual(v,out,THRESHOLD)));
}