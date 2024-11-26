////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// Copyright (c) 2013-24 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>

#include "../mgl/mgl.hpp"


//////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;


// Triangle 

const Vertex TriangleVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte TriangleIndices[] = {0, 1, 2};

// Square

const Vertex SquareVertices[] {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
};


const GLubyte SquareIndices[] = {
  0, 1, 2, 
  1, 3, 2
};

// Paralelogram

const Vertex ParalelogramVertices[] = {
  {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
  {{0.5f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
  {{0.5f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
  {{1.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
};

const GLubyte ParalelogramIndices[] = {
  0, 1, 2,
  1, 3, 2
};

const GLuint POSITION = 0, COLOR = 1;

//////////////////////////////////////////////////////////////////// TANGRAM OBJECTS
class TangramObject {
  public:
    GLuint VaoId, VboId[2];
    GLsizei NumberOfIndices;
    TangramObject(const Vertex Vertices[], size_t SizeOfVertices, const GLubyte Indices[], size_t SizeOfIndices);
    void destroyBufferObject();
    void draw(mgl::ShaderProgram * Shaders, GLint MatrixId, const glm::mat4 transformation);
};

TangramObject::TangramObject(const Vertex Vertices[], size_t SizeOfVertices, const GLubyte Indices[], size_t SizeOfIndices) {
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(2, VboId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    {
      glBufferData(GL_ARRAY_BUFFER,  SizeOfVertices , Vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(0));
      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(
          COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
          reinterpret_cast<GLvoid *>(sizeof(Vertices[0].XYZW)));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeOfIndices, Indices,
                   GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(2, VboId);

  NumberOfIndices = SizeOfIndices / sizeof(GLubyte);
}

void TangramObject::destroyBufferObject() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
}

void TangramObject::draw(mgl::ShaderProgram * Shaders, GLint MatrixId, const glm::mat4 transformation) {

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(transformation));
  glDrawElements(GL_TRIANGLES, NumberOfIndices, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));
  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

 private:
  std::unique_ptr<mgl::ShaderProgram> Shaders;
  GLint MatrixId;
  TangramObject * TriangleObject;
  TangramObject * SquareObject;
  TangramObject * ParallelogramObject;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

void MyApp::createBufferObjects() {
  TriangleObject = new TangramObject(TriangleVertices, sizeof(TriangleVertices), TriangleIndices, sizeof(TriangleIndices));
  SquareObject = new TangramObject(SquareVertices, sizeof(SquareVertices), SquareIndices, sizeof(SquareIndices));
  ParallelogramObject = new TangramObject(ParalelogramVertices, sizeof(ParalelogramVertices), ParalelogramIndices, sizeof(ParalelogramIndices));
}

void MyApp::destroyBufferObjects() {
  TriangleObject->destroyBufferObject();
  delete TriangleObject;
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
const glm::mat4 M = glm::translate(glm::vec3(-0.5f, -0.5f, 0.0f));

void MyApp::drawScene() {
  // Drawing directly in clip space
  TriangleObject->draw(Shaders.get(), MatrixId, I);
  SquareObject->draw(Shaders.get(), MatrixId, M);
  ParallelogramObject->draw(Shaders.get(), MatrixId, I);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Tangram 2D", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
