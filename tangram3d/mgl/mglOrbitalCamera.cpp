#include "./mglOrbitalCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

namespace mgl {

    OrbitalCamera::OrbitalCamera(
        GLuint bindingPoint, bool active, glm::vec3 eye, glm::vec3 center, glm::vec3 up, 
        glm::mat4 prespectiveProjectionMatrix, glm::mat4 orthographicProjectionMatrix
        ) : Camera(bindingPoint, active) {
            this->eye = eye;
            this->center = center;
            this->up = up;
            this->prespectiveProjectionMatrix = prespectiveProjectionMatrix;
            this->orthographicProjectionMatrix = orthographicProjectionMatrix;
            this->isOrthographic = false;
            updateViewMatrix(glm::lookAt(eye, center, up));
            updateProjectionMatrix(prespectiveProjectionMatrix);
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
        glm::vec3 view = glm::normalize(center - eye);
        glm::vec3 side = glm::normalize(glm::cross(view, up));
        up = glm::normalize(glm::cross(side, view));
        glm::quat qYaw = glm::angleAxis((float)yaw, up);
        glm::quat qPitch = glm::angleAxis((float)pitch, side);
        std::cout << pitch << std::endl;
        glm::quat q = qPitch * qYaw;
        glm::mat4 rotation = glm::toMat4(q);
        eye = glm::vec3(rotation * glm::vec4(eye, 0.0f));
        std::cout << "eye: " << eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
        up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));
        std::cout <<"up :" << up[0] << " " << up[1] << " " << up[2] << std::endl;
        updateViewMatrix(glm::lookAt(eye, center, up));
        unbind();
        bind();
        setViewMatrix();
        setProjectionMatrix();
    }

    void OrbitalCamera::switchProjection() {
       if (isOrthographic) {
           switchToPerspective();
       } else {
           switchToOrthographic();
       }
    }

    void OrbitalCamera::switchToOrthographic() {
        isOrthographic = true;
        updateProjectionMatrix(orthographicProjectionMatrix);
        unbind();
        bind();
        setViewMatrix();
        setProjectionMatrix();
    }

    void OrbitalCamera::switchToPerspective() {
        isOrthographic = false;
        updateProjectionMatrix(prespectiveProjectionMatrix);
        unbind();
        bind();
        setViewMatrix();
        setProjectionMatrix();
    }
}