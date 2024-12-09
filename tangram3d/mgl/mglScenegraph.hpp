////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./mglMesh.hpp" 
#include "./mglShader.hpp"
#include "./mglDrawable.hpp"

namespace mgl {


class SceneGraph;
class SceneNode;

//////////////////////////////////////////////////////////////////////////////// SceneGraph

class SceneGraph : public IDrawable {
 public:
  SceneGraph();
  virtual ~SceneGraph();
  SceneNode * getRoot();
  void setRoot(SceneNode *root);
  void draw();
  private:
    SceneNode *Root;
};

class SceneNode : public IDrawable {
  protected:
    SceneNode *Parent;
    std::vector<SceneNode *> Children;
    mgl::Mesh *Mesh;
    mgl::ShaderProgram *Shaders;
    GLuint ModelMatrixId;
    glm::mat4 ModelMatrix;

  public:
    SceneNode(SceneNode * parent, mgl::Mesh * mesh, mgl::ShaderProgram * shaderprogram, const glm::mat4 &modelmatrix, GLuint modelmatrixid);
    virtual ~SceneNode();
    void draw();
    //virtual void update(double deltaTime);
    virtual void drawSelf();
    void addChild(SceneNode *child);
    void setMesh(mgl::Mesh *mesh);
    void setShaderProgram(mgl::ShaderProgram *shaderprogram);
    void setModelMatrix(const glm::mat4 &modelmatrix);
};

} // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
