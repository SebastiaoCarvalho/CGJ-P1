////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace mgl {

class Camera;

///////////////////////////////////////////////////////////////////////// Camera

class Camera {
 private:
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;

 public:
  GLuint UboId;
  explicit Camera(GLuint bindingpoint);
  virtual ~Camera();
  glm::mat4 getViewMatrix() const;
  void updateViewMatrix(const glm::mat4 &viewmatrix);
  void setViewMatrix();
  glm::mat4 getProjectionMatrix() const;
  void updateProjectionMatrix(const glm::mat4 &projectionmatrix);
  void setProjectionMatrix();
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_CAMERA_HPP */
