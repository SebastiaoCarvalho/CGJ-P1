#include "tangram.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

////////////////////////////////////////////////////////////////////////// TANGRAM

void Tangram::changeToBox(double deltaTime) {
  getRoot()->changeToBox(deltaTime);
}

void Tangram::changeToTangram(double deltaTime) {
  getRoot()->changeToTangram(deltaTime);
}

////////////////////////////////////////////////////////////////////////// TANGRAM PIECES

TangramPiece::TangramPiece(
  SceneNode * parent, mgl::Mesh * mesh, mgl::ShaderProgram * shaderprogram,
      const glm::mat4 &modelMatrix, const glm::vec3 &translation, const glm::quat &rotation, 
      GLuint modelmatrixid, glm::vec4 color, GLuint colorId
  ) : SceneNode(parent, mesh, shaderprogram, modelMatrix, modelmatrixid) {
  Color = color;
  ColorId = colorId;
  BoxModelTranslation = translation;
  TangramModelTranslation = translation;
  BoxModelRotation = rotation;
  TangramModelRotation = rotation;
  TangramModelMatrix = modelMatrix;
}

TangramPiece::~TangramPiece() {}

void TangramPiece::drawSelf() {
  Shaders->bind();
  glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  glUniform4fv(ColorId, 1, glm::value_ptr(Color));
  Mesh->draw();
  Shaders->unbind();
}

void TangramPiece::changeToBox(double deltaTime) {
  if (lerpValue >= 1.0f) return;
  lerpValue += deltaTime;
}

void TangramPiece::changeToTangram(double deltaTime) {
  if (lerpValue <= 0.0f) return;
  lerpValue = glm::max(0.0f, lerpValue - (float) deltaTime);
}

void TangramPiece::update(double deltaTime) {
  glm::vec3 translationVector = glm::mix(TangramModelTranslation, BoxModelTranslation, lerpValue);
  glm::quat rotationQuaternion = glm::mix(TangramModelRotation, BoxModelRotation, lerpValue);
  ModelMatrix = glm::translate(translationVector) * glm::mat4(rotationQuaternion) * TangramModelMatrix;
}
