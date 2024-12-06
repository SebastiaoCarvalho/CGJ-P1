#include "./mglOrbitalCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

namespace mgl {

    OrbitalCamera::OrbitalCamera(
        GLuint bindingPoint, bool active, glm::vec3 eye, glm::vec3 center, glm::vec3 up, 
        float left, float right, float bottom, float top, float near, float far, float fovy, float ratio
        ) : Camera(bindingPoint, active) {
            this->eye = eye;
            this->center = center;
            this->up = up;
            this->left = left;
            this->right = right;
            this->bottom = bottom;
            this->top = top;
            this->near = near;
            this->far = far;
            this->fovy = fovy;
            this->ratio = ratio;
            this->isOrthographic = false;
            this->zoomValue = 0.0f;
            this->yaw = 0.0f;
            this->pitch = 0.0f;
            updateViewMatrix();
            updateProjectionMatrix();
    }

    void OrbitalCamera::update(float deltaTime) {
        applyZoom(deltaTime);
        applyRotate(deltaTime);
    }

    void OrbitalCamera::rotate(double yaw, double pitch) {
        this->yaw += yaw;
        this->pitch += pitch;
    }

    void OrbitalCamera::zoom(double yoffset) {
        zoomValue = yoffset;
    }

    void OrbitalCamera::applyZoom(double deltaTime) {
        if (!zoomValue) return;
        float ammount = float(deltaTime) * 5;
        std::cout << "eye : " <<  eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
        glm::vec3 view = glm::normalize(center - eye);
        view = glm::abs(view);
        glm::vec3 translation = view * (float)zoomValue * ammount;
        std::cout << "translation : " << translation[0] << " " << translation[1] << " " << translation[2] << std::endl;
        eye += translation;
        if (std::abs(zoomValue) < deltaTime) {
            zoomValue = 0.0f;
        }
        else {
            zoomValue += (zoomValue > 0) ? -ammount : ammount;
        }
        updateViewMatrix();
        refresh();
    }

    void OrbitalCamera::applyRotate(double deltaTime) {
        if (yaw == 0.0f && pitch == 0.0f) return;
        double ammount = deltaTime * 10;
        glm::vec3 view = glm::normalize(center - eye);
        glm::vec3 side = glm::normalize(glm::cross(view, up));
        up = glm::normalize(glm::cross(side, view));
        glm::quat qYaw = glm::angleAxis((float)(yaw * ammount), up);
        glm::quat qPitch = glm::angleAxis((float)(pitch * ammount), side);
        //std::cout << pitch << std::endl;
        glm::quat q = qPitch * qYaw;
        glm::mat4 rotation = glm::toMat4(q);
        eye = glm::vec3(rotation * glm::vec4(eye, 1.0f));
        //std::cout << "eye: " << eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
        up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));
        //std::cout <<"up :" << up[0] << " " << up[1] << " " << up[2] << std::endl;
        yaw = 0.0f;
        pitch = 0.0f;
        updateViewMatrix();
        refresh();
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
        updateProjectionMatrix();
        refresh();
    }

    void OrbitalCamera::switchToPerspective() {
        isOrthographic = false;
        updateProjectionMatrix();
        refresh();
    }

    void OrbitalCamera::updateViewMatrix() {
        Camera::updateViewMatrix(glm::lookAt(eye, center, up));
    }

    void OrbitalCamera::updateProjectionMatrix() {
        if (isOrthographic) {
            Camera::updateProjectionMatrix(glm::ortho(left * ratio, right * ratio, bottom * ratio , top * ratio, near, far));
        } else {
            Camera::updateProjectionMatrix(glm::perspective(fovy, ratio, near, far));
        }
    }

    void OrbitalCamera::updateViewPort(int width, int height) {
        ratio = (float)width / (float)height;
        updateProjectionMatrix();
        refresh();
    }
}