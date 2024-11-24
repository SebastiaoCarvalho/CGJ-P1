#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

const float THRESHOLD = (float)1.0e-5;

glm::vec3 randomVec();

glm::mat3 randomMat3();

bool vectorEquality(glm::vec3 v1, glm::vec3 v2);

bool matrixEquality(glm::mat3 m1, glm::mat3 m2);

void print_header(const std::string& text);

#endif