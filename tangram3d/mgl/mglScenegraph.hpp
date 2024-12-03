////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

/* #include <vector>
#include <glm/glm.hpp>
#include "./mglMesh.hpp"
#include "./mglShader.hpp" */

namespace mgl {

class IDrawable;
/* class SceneGraph;
class SceneNode; */

////////////////////////////////////////////////////////////////////// IDrawable

class IDrawable {
 public:
  virtual void draw(void) = 0;
};

//////////////////////////////////////////////////////////////////////////////// SceneGraph

/* class SceneGraph : public IDrawable {
 public:
  SceneGraph();
  virtual ~SceneGraph();
  void setRoot(SceneNode *root);
  void draw();
  private:
    SceneNode *Root;
};

class SceneNode : public IDrawable {
  private:
    SceneNode *Parent;
    std::vector<SceneNode *> Children;
    mgl::Mesh *Mesh;
    mgl::ShaderProgram *ShaderProgram;
    glm::mat4 ModelMatrix;

  public:
    SceneNode();
    virtual ~SceneNode();
    void draw();
    void addChild(SceneNode *child);
    void setMesh(mgl::Mesh *mesh);
    void setShaderProgram(mgl::ShaderProgram *shaderprogram);
    void setModelMatrix(const glm::mat4 &modelmatrix);
}; */

} // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
