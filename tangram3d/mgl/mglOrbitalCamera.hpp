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
        glm::mat4 prespectiveProjectionMatrix;
        glm::mat4 orthographicProjectionMatrix;
        bool isOrthographic = false;
    public:
        OrbitalCamera(
            GLuint bindingPoint, 
            bool active, 
            glm::vec3 eye, 
            glm::vec3 center, 
            glm::vec3 up, 
            glm::mat4 prespectiveProjectionMatrix, 
            glm::mat4 orthographicProjectionMatrix
        );
        void zoom(double yoffset);
        void rotate(double yaw, double pitch);
        void switchProjection();
        void switchToOrthographic();
        void switchToPerspective();
};

}

#endif //MGL_ORBITAL_CAMERA_HPP