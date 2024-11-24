#ifndef EX3_HPP
#define EX3_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "../utils/utils.hpp"

glm::vec3 rodriguesRotation(glm::vec3 v, glm::vec3 a, float theta);

void ex3();

#endif