#ifndef EX2_HPP
#define EX2_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "../utils/utils.hpp"

void testCoordinateFrames(glm::vec3 up, glm::vec3 view, glm::vec3 expectedV, glm::vec3 expectedW, glm::vec3 expectedU);

void ex2();


#endif