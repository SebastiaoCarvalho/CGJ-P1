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
      const glm::quat &axisRotation, GLuint modelmatrixid, glm::vec4 color, GLuint colorId
  ) : SceneNode(parent, mesh, shaderprogram, modelMatrix, modelmatrixid) {
  Color = color;
  ColorId = colorId;
  BoxModelTranslation = translation;
  TangramModelTranslation = glm::vec3(0.0f);
  BoxModelRotation = rotation;
  TangramModelRotation = glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  BoxModelAxisRotation = axisRotation;
  TangramModelAxisRotation = glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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
  for (auto &child : Children) {
    ((TangramPiece * )child)->changeToBox(deltaTime);
  }
  if (lerpValue >= 1.0f) return;
  lerpValue = glm::min(1.0f, lerpValue + (float) deltaTime);
}

void TangramPiece::changeToTangram(double deltaTime) {
  for (auto &child : Children) {
    ((TangramPiece * )child)->changeToTangram(deltaTime);
  }
  if (lerpValue <= 0.0f) return;
  lerpValue = glm::max(0.0f, lerpValue - (float) deltaTime);
}

void TangramPiece::update(double deltaTime) {
  glm::vec3 translationVector = glm::mix(TangramModelTranslation, BoxModelTranslation, lerpValue);
  glm::quat rotationQuaternion = glm::mix(TangramModelRotation, BoxModelRotation, lerpValue);
  glm::quat axisRotationQuaternion = glm::slerp(TangramModelAxisRotation, BoxModelAxisRotation, lerpValue);
  glm::mat4 parentModelMatrix = Parent ? ((TangramPiece *)Parent)->TangramModelMatrix : glm::mat4(1.0f);

  ModelMatrix =  parentModelMatrix * glm::mat4(axisRotationQuaternion) * glm::translate(translationVector)
    * TangramModelMatrix * glm::mat4(rotationQuaternion);
  for (auto &child : Children) {
    child->update(deltaTime);
  }
}
