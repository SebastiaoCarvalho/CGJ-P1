#ifndef EX4_HPP
#define EX4_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "../utils/utils.hpp"


glm::mat3 rodriguesRotation(glm::mat3 A, float theta);
glm::mat3 dualMatrix(glm::vec3 a);
void ex4();

#endif