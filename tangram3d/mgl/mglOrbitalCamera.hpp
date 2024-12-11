#ifndef MGL_ORBITAL_CAMERA_HPP
#define MGL_ORBITAL_CAMERA_HPP

#include "./mglCamera.hpp"

namespace mgl {

class OrbitalCamera;

class OrbitalCamera : public Camera {
    private:
        glm::vec3 eye;
        glm::vec3 center;
        glm::vec3 up;
        float left;
        float right;
        float bottom;
        float top;
        float near;
        float far;
        float fovy;
        float ratio;
        bool isOrthographic = false;
        float zoomValue;
        float yaw;
        float pitch;
    public:
        OrbitalCamera(
            GLuint bindingPoint, 
            bool active, 
            glm::vec3 eye, 
            glm::vec3 center, 
            glm::vec3 up, 
            float left,
            float right,
            float bottom,
            float top,
            float near,
            float far,
            float fovy,
            float ratio
        );
        void update(float deltaTime);
        void updateViewMatrix();
        void updateProjectionMatrix();
        void rotate(double yaw, double pitch);
        void zoom(double yoffset);
        //void applyZoom(double deltaTime);
        void applyZoomAndRotation(double deltaTime);
        void applyRotate(double deltaTime);
        void switchProjection();
        void switchToOrthographic();
        void switchToPerspective();
        void updateViewPort(int width, int height);
};

}

#endif //MGL_ORBITAL_CAMERA_HPP