#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

const float THRESHOLD = (float)1.0e-5;

static void printMatrix(glm::mat4& matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << matrix[j][i] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

static void vectorOperations() {
	glm::vec3 v1(1.0f, 2.0f, 3.0f);
	glm::vec3 v2(4.0f, 5.0f, 6.0f);

	//Example of using glm experimental string cast functions
	std::cout << "v1: " << glm::to_string(v1) << "" << std::endl;
	std::cout << "v2: " << glm::to_string(v2) << "" << std::endl;

	//Calculate and print the result of adding vector v2 to vector v1
	std::cout << "v1 + v2: " << glm::to_string(v1 + v2) << "" << std::endl;
	
	//Calculate and print the result of subtracting vector v2 to vector v1
	std::cout << "v1 - v2: " << glm::to_string(v1 - v2) << "" << std::endl;
	
	//Calculate and print the result of the dot product between v1 and v2
	std::cout << "v1 . v2: " << glm::dot(v1, v2) << "" << std::endl;
	
	//Calculate and print the result of the cross product between v1 and v2
	std::cout << "v1 x v2: " << glm::to_string(glm::cross(v1, v2)) << "" << std::endl;

}

static void matrixOperations() {
	glm::mat4 model = glm::mat4(1.0f);
	printMatrix(model);
	//First, apply a translation to the model by (3, 2, -5)
	glm::mat4 translationMatrix = glm::translate(model, glm::vec3(3.0f, 2.0f, -5.0f));
	printMatrix(translationMatrix);
	//Secondly, scale the model by (2, 2, 2)
	glm::mat4 scaleMatrix = glm::scale(translationMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	printMatrix(scaleMatrix);
	//Then, rotate the model 45 degrees around the axis (0, 0, 1)
	glm::mat4 rotationMatrix = glm::rotate(scaleMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	printMatrix(rotationMatrix);
	//Finally, print the resulting model matrix
}

static bool tripleProductProposition(glm::vec3 i, glm::vec3 j, glm::vec3 k) {
	glm::vec3 crossProduct = glm::cross(i, glm::cross(j, k));
	glm::vec3 dotProduct = j * glm::dot(i, k) - k * glm::dot(i, j);
	return glm::all(glm::epsilonEqual(crossProduct, dotProduct, THRESHOLD));
}

static glm::vec3 randomVec() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	return glm::vec3(dist(gen), dist(gen), dist(gen));
}

static glm::mat3 randomMat3() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	glm::mat3 randomMat;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			randomMat[i][j] = dist(gen);
		}
	}
	return randomMat;
}

static void ex1() {
	assert(tripleProductProposition(randomVec(), randomVec(), randomVec()));
}

static bool matrixEquality(glm::mat4 m1, glm::mat4 m2) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (! glm::epsilonEqual(m1[i][j], m2[i][j], THRESHOLD))
				return false;
	return true;
}

static bool transpose_property(glm::mat4 m1, glm::mat4 m2) {
	glm::mat4 side1 = glm::transpose(m1 * m2);
	glm::mat4 side2 = glm::transpose(m2) * glm::transpose(m1);
	return matrixEquality(side1, side2);
}

static void ex2() {
	glm::vec3 up = glm::vec3(0.7, 0.7, 0);
	glm::vec3 view = glm::vec3(1, 0, 0);
	glm::vec3 v = glm::normalize(view);
	glm::vec3 w = glm::normalize(glm::cross(up, v));
	glm::vec3 u = glm::cross(v, w);
	assert(glm::length(v) == 1);
	assert(glm::length(w) == 1);
	assert(glm::length(u) == 1);
	assert(glm::dot(u, v) == 0);
	assert(glm::dot(u, w) == 0);
	assert(glm::dot(v, w) == 0);
	assert(glm::all(glm::epsilonEqual(glm::cross(u, v), w, THRESHOLD)));
	
	// assert values
	assert(glm::all(glm::epsilonEqual(v, view, THRESHOLD)));
	assert(glm::all(glm::epsilonEqual(w, glm::vec3(0, 0, -1), THRESHOLD)));
	assert(glm::all(glm::epsilonEqual(u, glm::vec3(0, 1, 0), THRESHOLD)));
}

inline glm::vec3 rodriguesRotation(glm::vec3 v, glm::vec3 a, float theta) {
    return glm::cos(glm::radians(theta)) * v +
           glm::cross(a,v) * glm::sin(glm::radians(theta)) +
           a * glm::dot(a, v) * (1 - glm::cos(glm::radians(theta)));
}


inline glm::mat3 rodriguesRotation(glm::mat3 A, float theta) {
    return glm::mat3(1.0f) + glm::sin(glm::radians(theta)) * A +
           (1 - glm::cos(glm::radians(theta))) * A * A;
}

inline glm::mat3 dualMatrix(glm::vec3 a) {
    return glm::mat3(0, a.z, -a.y, -a.z, 0, a.x, a.y, -a.x, 0);
}

static void ex3() {
    glm::vec3 v = glm::vec3(1.0f,1.0f,0);
    glm::vec3 a = glm::vec3(0,0,1.0f);
    glm::vec3 out = glm::vec3(0,1.41421f, 0);
    for (int i = 0; i < 9; i++)
        v = rodriguesRotation(v,a,45.0f);
    assert(glm::all(glm::epsilonEqual(v,out,THRESHOLD)));
}


static void ex4() {
    glm::vec3 v = glm::vec3(1.0f,1.0f,1.0f); // input
    glm::vec3 out = glm::vec3(1.0f,-1.0f,1.0f); // input

    glm::vec3 ax = glm::vec3(1.0f,0,0);
    glm::vec3 ay = glm::vec3(0,1.0f,0);
    glm::vec3 az = glm::vec3(0,0,1.0f);
    glm::mat3 Rx = rodriguesRotation(dualMatrix(ax), 90.0f);
    glm::mat3 Ry = rodriguesRotation(dualMatrix(ay), 90.0f);
    glm::mat3 Rz = rodriguesRotation(dualMatrix(az), 90.0f);
    for (int i = 0; i < 3; i++)
        v = Rx * v;
    for (int i = 0; i < 3; i++)
        v = Ry * v;
    for (int i = 0; i < 3; i++)
        v = Rz * v;

    assert(glm::all(glm::epsilonEqual(v,out,THRESHOLD)));
}

static void ex5() {
	assert(transpose_property(randomMat3(), randomMat3()));
}

int main() {
	ex1();
	ex2();
	ex3();
	ex4();
	ex5();
	std::cout << "Run successful";
	return 0;
}