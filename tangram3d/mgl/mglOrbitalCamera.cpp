#include "./mglOrbitalCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <iostream>

namespace mgl {

    OrbitalCamera::OrbitalCamera(GLuint bindingPoint, bool active, glm::vec3 eye, glm::vec3 center, glm::vec3 up) 
        : Camera(bindingPoint, active) {
            this->eye = eye;
            this->center = center;
            this->up = up;
            updateViewMatrix(glm::lookAt(eye, center, up));
    }

    void OrbitalCamera::zoom(double yoffset) {
        std::cout << eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
        glm::vec3 direction = glm::normalize(center - eye);
        direction = glm::abs(direction);
        glm::vec3 translation = direction * (float)yoffset;
        std::cout << "translation : " << translation[0] << " " << translation[1] << " " << translation[2] << std::endl;
        eye += translation;
        updateViewMatrix(glm::lookAt(eye, center, up));
        unbind();
        bind();
        setViewMatrix();
        setProjectionMatrix();
    }

    void OrbitalCamera::rotate(double yaw, double pitch) {
        
    }
}