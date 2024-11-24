#ifndef EX5_HPP
#define EX5_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "../utils/utils.hpp"

bool transpose_property(glm::mat3 m1, glm::mat3 m2);

void ex5();

#endif