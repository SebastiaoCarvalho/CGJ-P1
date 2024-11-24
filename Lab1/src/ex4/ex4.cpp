#include "ex4.hpp"

glm::mat3 rodriguesRotation(glm::mat3 A, float theta) {
    return glm::mat3(1.0f) + glm::sin(glm::radians(theta)) * A +
           (1 - glm::cos(glm::radians(theta))) * A * A;
}

glm::mat3 dualMatrix(glm::vec3 a) {
    return glm::mat3(0, a.z, -a.y, -a.z, 0, a.x, a.y, -a.x, 0);
}

void ex4() {
    print_header("Exercise 4");
    glm::vec3 v = glm::vec3(1.0f,1.0f,1.0f); // input
    glm::vec3 expectedV = glm::vec3(1.0f,-1.0f,1.0f); // input
    std::cout << "initial v: " << glm::to_string(v) << std::endl;

    glm::vec3 ax = glm::vec3(1.0f,0,0);
    glm::vec3 ay = glm::vec3(0,1.0f,0);
    glm::vec3 az = glm::vec3(0,0,1.0f);
    glm::mat3 Rx = rodriguesRotation(dualMatrix(ax), 90.0f);
    glm::mat3 Ry = rodriguesRotation(dualMatrix(ay), 90.0f);
    glm::mat3 Rz = rodriguesRotation(dualMatrix(az), 90.0f);
    for (int i = 0; i < 3; i++) {
        v = Rx * v;
        std::cout << "v after " << i+1 << " rotations of 90 degrees on x: " << glm::to_string(v) << std::endl;
    }
    for (int i = 0; i < 3; i++) {
        v = Ry * v;
        std::cout << "v after " << i+1 << " rotations of 90 degrees on y: " << glm::to_string(v) << std::endl;
    }
    for (int i = 0; i < 3; i++) {
        v = Rz * v;
        std::cout << "v after " << i+1 << " rotations of 90 degrees on z: " << glm::to_string(v) << std::endl;
    }
    std::cout << "expected v: " << glm::to_string(expectedV) << std::endl;
    std::cout << "final v == expected v: " << vectorEquality(v, expectedV) << std::endl;
    assert(vectorEquality(v, expectedV));
    print_header("Exercise 4 completed");
}