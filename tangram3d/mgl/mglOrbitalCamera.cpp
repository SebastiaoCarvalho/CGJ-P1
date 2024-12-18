#include "./mglOrbitalCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

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
            this->initialRatio = ratio;
            this->ratio = ratio;
            this->isOrthographic = false;
            this->zoomValue = glm::length(eye - center);
            this->zoomLeft = 0.0f;
            this->yaw = 0.0f;
            this->pitch = 0.0f;
            updateViewMatrix();
            updateProjectionMatrix();
    }

    void OrbitalCamera::update(float deltaTime) {
        applyZoomAndRotation(deltaTime);
        updateViewMatrix();
        refresh();
    }

    void OrbitalCamera::rotate(double yaw, double pitch) {
        this->yaw += yaw ;
        this->pitch += pitch;
    }

    void OrbitalCamera::zoom(double yoffset) {
        zoomLeft += yoffset * 0.5;
    }

    void OrbitalCamera::applyZoomAndRotation(double deltaTime) {
        double ammount = deltaTime * 10;
        
        // Use zoom left to smooth zoom animation
        double zoomAmmount = zoomLeft * 0.05;
        zoomLeft -= zoomAmmount;

        glm::vec3 view = glm::normalize(center - eye);
        glm::vec3 side = glm::normalize(glm::cross(up, view));

        // Compute camera rotation
        glm::quat qYaw = glm::angleAxis((float)(yaw * ammount), up);
        glm::quat qPitch = glm::angleAxis((float)(pitch * ammount), side);
        glm::quat q = qPitch * qYaw;
        glm::mat4 rotation = glm::toMat4(q);

        // Update zoom and rotation

        zoomValue = glm::max(float(zoomValue + zoomAmmount), 1e-5f);

        eye = glm::translate(center) * glm::scale(glm::vec3(zoomValue)) * glm::mat4(rotation)  * -(glm::vec4(view, 1.0f));
        up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));

        pitch = 0.0f;
        yaw = 0.0f;
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
            Camera::updateProjectionMatrix(glm::ortho(left * ratio, right * ratio, bottom * initialRatio, top * initialRatio, near, far));
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