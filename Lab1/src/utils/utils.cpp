#include "utils.hpp"

glm::vec3 randomVec() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	return glm::vec3(dist(gen), dist(gen), dist(gen));
}

glm::mat3 randomMat3() {
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

bool vectorEquality(glm::vec3 v1, glm::vec3 v2) {
    return glm::all(glm::epsilonEqual(v1, v2, THRESHOLD));
}

bool matrixEquality(glm::mat4 m1, glm::mat4 m2) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (! glm::epsilonEqual(m1[i][j], m2[i][j], THRESHOLD))
				return false;
	return true;
}

void print_header(const std::string& text) {
    std::cout << '\n' << text << '\n' << '\n';
}