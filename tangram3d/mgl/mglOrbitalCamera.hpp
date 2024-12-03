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
    public:
        OrbitalCamera(GLuint bindingPoint, bool active, glm::vec3 eye, glm::vec3 center, glm::vec3 up);
        void zoom(double yoffset);
};

}

#endif //MGL_ORBITAL_CAMERA_HPP