#ifndef TANGRAM_HPP
#define TANGRAM_HPP

#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// TANGRAM
class TangramPiece;

class Tangram : public mgl::SceneGraph {
  public:
    void changeToBox(double deltaTime);
    void changeToTangram(double deltaTime);
    TangramPiece *getRoot() { return (TangramPiece *)SceneGraph::getRoot(); }
};

////////////////////////////////////////////////////////////////////////// TANGRAM PIECES
class TangramPiece : public mgl::SceneNode {
  private:
    glm::vec4 Color;
    GLuint ColorId;
    float lerpValue = 0.0f;

    glm::mat4 TangramModelMatrix;

    // animation variables
    glm::vec3 BoxModelTranslation;
    glm::vec3 TangramModelTranslation;

    glm::quat BoxModelRotation;
    glm::quat TangramModelRotation;

    glm::quat BoxModelAxisRotation;
    glm::quat TangramModelAxisRotation;

  public:
    TangramPiece(
      SceneNode * parent, mgl::Mesh * mesh, mgl::ShaderProgram * shaderprogram,
      const glm::mat4 &modelMatrix, const glm::vec3 &translation, const glm::quat &rotation,
      const glm::quat &axisRotation, GLuint modelmatrixid, glm::vec4 color, GLuint colorId
      );
    virtual ~TangramPiece();
    virtual void drawSelf() override;
    void changeToBox(double deltaTime);
    void changeToTangram(double deltaTime);
    virtual void update(double deltaTime) override;
};

#endif // TANGRAM_HPP