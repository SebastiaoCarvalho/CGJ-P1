////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglCamera.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mgl {

///////////////////////////////////////////////////////////////////////// Camera

Camera::Camera(GLuint bindingpoint, bool active)
    : ViewMatrix(glm::mat4(1.0f)), ProjectionMatrix(glm::mat4(1.0f)) {
  isActive = active;
  BindingPoint = bindingpoint;
  if (isActive) bind();
}

Camera::~Camera() {
  unbind();
}

void Camera::bind() {
  glGenBuffers(1, &UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::unbind() {
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glDeleteBuffers(1, &UboId);
}

glm::mat4 Camera::getViewMatrix() const { return ViewMatrix; }

void Camera::updateViewMatrix(const glm::mat4 &viewmatrix) {
  ViewMatrix = viewmatrix;
}

void Camera::setViewMatrix() {
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(ViewMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glm::mat4 Camera::getProjectionMatrix() const { return ProjectionMatrix; }

void Camera::updateProjectionMatrix(const glm::mat4 &projectionmatrix) {
  ProjectionMatrix = projectionmatrix;
}

void Camera::setProjectionMatrix() {
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(ProjectionMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::refresh() {
  unbind();
  bind();
  setViewMatrix();
  setProjectionMatrix();
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl
