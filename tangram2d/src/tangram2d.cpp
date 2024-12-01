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
#include <iostream>

#include "../mgl/mgl.hpp"


//////////////////////////////////////////////////////////////////// structs and constants

typedef struct {
  GLfloat XYZW[4];
} Vertex;

typedef struct {
  GLfloat value[4];
} Color;

// Triangle 

const Vertex TriangleVertices[] = {
    {-0.25f, -0.25f, 0.0f, 1.0f},
    {0.25f, -0.25f, 0.0f, 1.0f},
    {-0.25f, 0.25f, 0.0f, 1.0f},
};

const GLubyte TriangleIndices[] = {0, 1, 2};

// Square

const Vertex SquareVertices[] {
    {-0.25f, -0.25f, 0.0f, 1.0f}, 
    {0.25f, -0.250f, 0.0f, 1.0f}, 
    {-0.25f, 0.25f, 0.0f, 1.0f},
    {0.25f, 0.25f, 0.0f, 1.0f}, 
};


const GLubyte SquareIndices[] = {
  0, 1, 2, 
  1, 3, 2
};

// Paralelogram

const Vertex ParalelogramVertices[] = {
  {-0.5f, -0.25f, 0.0f, 1.0f},
  {0.0f, -0.25f, 0.0f, 1.0f},
  {0.0f, 0.25f, 0.0f, 1.0f},
  {0.5f, 0.25f, 0.0f, 1.0f},
};

const GLubyte ParalelogramIndices[] = {
  0, 1, 2,
  1, 3, 2
};

const Color RED = {{1.0f, 0.0f, 0.0f, 1.0f}};
const Color GREEN = {{0.0f, 1.0f, 0.0f, 1.0f}};
const Color BLUE = {{0.0f, 0.0f, 1.0f, 1.0f}};
const Color YELLOW = {{1.0f, 1.0f, 0.0f, 1.0f}};
const Color CYAN = {{0.0f, 1.0f, 1.0f, 1.0f}};
const Color MAGENTA = {{1.0f, 0.0f, 1.0f, 1.0f}};
const Color ORANGE = {{1.0f, 0.5f, 0.0f, 1.0f}};


const GLuint POSITION = 0, COLOR = 1;

//////////////////////////////////////////////////////////////////// TANGRAM OBJECTS
class TangramObject {
  public:
    GLuint VaoId, VboId[2];
    GLsizei NumberOfIndices;
    TangramObject(const Vertex Vertices[], size_t SizeOfVertices, const GLubyte Indices[], size_t SizeOfIndices);
    void destroyBufferObject();
    void draw(mgl::ShaderProgram * Shaders, GLint MatrixId, const glm::mat4 transformation, const Color color, GLint ColorId);
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

void TangramObject::draw(mgl::ShaderProgram * Shaders, GLint MatrixId, const glm::mat4 transformation, const Color color, GLint ColorId) {
  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(transformation));
  glUniform4fv(ColorId, 1, color.value);
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
  GLint ColorId;
  TangramObject * TriangleObject;
  TangramObject * SquareObject;
  TangramObject * ParallelogramObject;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();

  void drawSmallTriangle1();
  void drawSmallTriangle2();
  void drawMediumTriangle();
  void drawLargeTriangle1();
  void drawLargeTriangle2();
  void drawSquare();
  void drawParallelogram();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");
  Shaders->addUniform("Color");
  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
  ColorId = Shaders->Uniforms["Color"].index;
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

const float horizontalOffset = glm::sqrt(0.5) / 2;
const float downVerticalOffset = 0.25f;

void MyApp::drawSmallTriangle1() {
  glm::mat4 rotation = glm::rotate(glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translation = glm::translate(glm::vec3(horizontalOffset, 0.0f, 0.0f));
  glm::mat4 transformation = translation * rotation;
  TriangleObject->draw(Shaders.get(), MatrixId, transformation, RED, ColorId);
}

void MyApp::drawSmallTriangle2() {
  glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translation = glm::translate(glm::vec3(horizontalOffset, 0.0f, 0.0f));
  glm::mat4 transformation = translation * rotation;
  TriangleObject->draw(Shaders.get(), MatrixId, transformation, ORANGE, ColorId);
}

void MyApp::drawMediumTriangle() {
  glm::mat4 rotation = glm::rotate(glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 scale = glm::scale(glm::vec3(glm::sqrt(2), glm::sqrt(2), 1.0));
  glm::mat4 translation = glm::translate(glm::vec3(0.25f, downVerticalOffset - 1.0f, 0.0f));
  glm::mat4 transformation = translation * rotation * scale;
  TriangleObject->draw(Shaders.get(), MatrixId, transformation, MAGENTA, ColorId);
}

void MyApp::drawLargeTriangle1() {
  glm::mat4 rotation = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 1.0f));
  glm::mat4 translation = glm::translate(glm::vec3(0.5, 0.5f, 0.0f));
  glm::mat4 transformation =  translation * rotation * scale;
  TriangleObject->draw(Shaders.get(), MatrixId, transformation, GREEN, ColorId);
}

void MyApp::drawLargeTriangle2() {
  glm::mat4 rotation = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 1.0f));
  glm::mat4 translation = glm::translate(glm::vec3(-0.5, 0.5f, 0.0f));
  glm::mat4 transformation =  translation * rotation * scale;
  TriangleObject->draw(Shaders.get(), MatrixId, transformation, BLUE, ColorId);
}

void MyApp::drawSquare() {
  glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translation = glm::translate(glm::vec3(- horizontalOffset, 0.0f, 0.0f));
  glm::mat4 transformation = translation * rotation;
  SquareObject->draw(Shaders.get(), MatrixId, transformation, YELLOW, ColorId);
}

void MyApp::drawParallelogram() {
  glm::mat4 translation = glm::translate(glm::vec3(-0.25f, downVerticalOffset -0.75f, 0.0f));
  ParallelogramObject->draw(Shaders.get(), MatrixId, translation, CYAN, ColorId);
}

void MyApp::drawScene() {
  // Draw Tangram Pieces in clip space
  drawSmallTriangle1();
  drawSmallTriangle2();
  drawMediumTriangle();
  drawLargeTriangle1();
  drawLargeTriangle2();
  drawSquare();
  drawParallelogram();
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
