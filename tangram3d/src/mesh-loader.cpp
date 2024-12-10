////////////////////////////////////////////////////////////////////////////////
//
//  Loading meshes from external files
//
// Copyright (c) 2023-24 by Carlos Martinho
//
// INTRODUCES:
// MODEL DATA, ASSIMP, mglMesh.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../mgl/mgl.hpp"
#include "./tangram.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void cursorCallback(GLFWwindow *win, double xpos, double ypos) override;
  void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) override;
  void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) override;
  void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) override;
  void printMatrix(const glm::mat4 &matrix);

 private:
  const GLuint UBO_BP = 0;
  mgl::ShaderProgram *Shaders = nullptr;
  mgl::OrbitalCamera *Camera1 = nullptr;
  mgl::OrbitalCamera *Camera2 = nullptr;
  mgl::OrbitalCamera *CurrentCamera = nullptr;
  Tangram *Scene = nullptr;
  GLint ModelMatrixId;
  GLint ColorId;
  GLint CameraId;
  bool isMousePressed = false;
  mgl::Mesh *TriangleMesh = nullptr;
  mgl::Mesh *SquareMesh = nullptr;
  mgl::Mesh * ParallelogramMesh = nullptr;
  mgl::KeyManager *KeyManager = nullptr;
  double prevX = 0.0;
  double prevY = 0.0;

  void animate(double elapsed);
  void createSceneGraph();
  void createMeshes();
  void createManagers();
  void createShaderPrograms();
  void createCameras();
  void drawScene();
  void switchCamera();

  TangramPiece * createSmallTriangle(TangramPiece * root);
  TangramPiece * createSmallTriangle2(TangramPiece * root);
  TangramPiece * createMediumTriangle(TangramPiece * root);
  TangramPiece * createLargeTriangle(TangramPiece * root);
  TangramPiece * createLargeTriangle2(TangramPiece * root);
  TangramPiece * createSquare(TangramPiece * root);
  TangramPiece * createParallelogram(TangramPiece * root);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
  std::string mesh_dir = "../assets/models/";

  std::string triangle_file = "triangle.obj";
  std::string triangle_fullname = mesh_dir + triangle_file;

  TriangleMesh = new mgl::Mesh();
  TriangleMesh->joinIdenticalVertices();
  TriangleMesh->create(triangle_fullname);

  std::string square_file = "square.obj";
  std::string square_fullname = mesh_dir + square_file;

  SquareMesh = new mgl::Mesh();
  SquareMesh->joinIdenticalVertices();
  SquareMesh->create(square_fullname);
  
  std::string parallelogram_file = "parallelogram.obj";
  std::string parallelogram_fullname = mesh_dir + parallelogram_file;

  ParallelogramMesh = new mgl::Mesh();
  ParallelogramMesh->joinIdenticalVertices();
  ParallelogramMesh->create(parallelogram_fullname);
}

void MyApp::createManagers() { KeyManager = new mgl::KeyManager(); }

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  if (TriangleMesh->hasNormals() || SquareMesh->hasNormals() || ParallelogramMesh->hasNormals()) {
    Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (TriangleMesh->hasTexcoords() || SquareMesh->hasTexcoords() || ParallelogramMesh->hasTexcoords()) {
    Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }
  if (TriangleMesh->hasTangentsAndBitangents() || SquareMesh->hasTangentsAndBitangents() 
    || ParallelogramMesh->hasTangentsAndBitangents()) {
    Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
  }

  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniform(mgl::COLOR);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
  ColorId = Shaders->Uniforms[mgl::COLOR].index;
  CameraId = Shaders->Ubos[mgl::CAMERA_BLOCK].index;
}

///////////////////////////////////////////////////////////////////////// SCENE GRAPH
void MyApp::createSceneGraph() {
  Scene = new Tangram();
  TangramPiece *Root = new TangramPiece(
    nullptr, nullptr, Shaders, glm::mat4(1.0f), 
    glm::vec3(0.0f), glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)), glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)),
    ModelMatrixId, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), ColorId
  );
  Root->addChild(createSmallTriangle(Root));
  Root->addChild(createSmallTriangle2(Root));
  Root->addChild(createMediumTriangle(Root));
  Root->addChild(createLargeTriangle(Root));
  Root->addChild(createLargeTriangle2(Root));
  Root->addChild(createSquare(Root));
  Root->addChild(createParallelogram(Root));
  Scene->setRoot(Root);
}

const float pieceSize = 5.0f;
const float scaleSize = 0.5f;
const float triangleCathetus = glm::sqrt(pieceSize * pieceSize / 2);
const float smallSizeHorizontalOffset = (pieceSize / 2) * scaleSize;
const float mediumSizeHorizontalOffset = smallSizeHorizontalOffset * glm::sqrt(1.0f/2.0f);
const float downVerticalOffset = smallSizeHorizontalOffset * glm::sqrt(2);

const glm::quat axisRotation = glm::angleAxis(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

TangramPiece * MyApp::createSmallTriangle(TangramPiece * root) {
  // Model Matrix
  glm::mat4 scale = glm::scale(glm::vec3(scaleSize, 1.0f, scaleSize));
  glm::mat4 rotation = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 translationVector = glm::vec3(smallSizeHorizontalOffset, 0.0f, scaleSize * pieceSize / 4);
  glm::mat4 translation = glm::translate(translationVector);
  glm::mat4 transformation = translation * rotation * scale;
  // Animation variables
  glm::quat boxRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 boxTranslation = glm::vec3(-smallSizeHorizontalOffset /2, 0.0f, 0.0f) - translationVector;
  TangramPiece *TriangleSmall = new TangramPiece(
    root, TriangleMesh, Shaders, transformation, 
    boxTranslation, boxRotation, axisRotation,
    ModelMatrixId, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), ColorId
  );
  return TriangleSmall;
}

TangramPiece * MyApp::createSmallTriangle2(TangramPiece * root) {
  // Model Matrix
  glm::mat4 scale = glm::scale(glm::vec3(scaleSize, 1.0f, scaleSize));
  glm::mat4 translation = glm::translate(glm::vec3(smallSizeHorizontalOffset, 0.0f, - scaleSize * pieceSize / 4));
  glm::mat4 transformation = translation  * scale;
  // Animation variables
  glm::vec3 boxTranslation = glm::vec3(0.0f, 0.0f, smallSizeHorizontalOffset * 2);
  TangramPiece *TriangleSmall2 = new TangramPiece(
    root, TriangleMesh, Shaders, transformation, 
    boxTranslation, glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)), axisRotation,
    ModelMatrixId, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), ColorId
  );
  return TriangleSmall2;
}

TangramPiece * MyApp::createMediumTriangle(TangramPiece * root) {
  // Model Matrix
  glm::mat4 scale = glm::scale(glm::vec3(glm::sqrt(2) * 0.5, 1.0f, glm::sqrt(2) * 0.5));
  glm::vec3 translationVector = glm::vec3(mediumSizeHorizontalOffset, 0.0f, downVerticalOffset);
  glm::mat4 translation = glm::translate(translationVector);
  glm::mat4 transformation = translation * scale;
  // Animation variables
  glm::quat boxRotation = glm::angleAxis(glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 boxTranslation = glm::vec3(- smallSizeHorizontalOffset * (3.0f/2.0f), 0.0f, smallSizeHorizontalOffset * (3.0f/2.0f)) - translationVector;
  TangramPiece *TriangleMedium = new TangramPiece(
    root, TriangleMesh, Shaders, transformation,
    boxTranslation, boxRotation, axisRotation,
    ModelMatrixId, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ColorId
  );
  return TriangleMedium;
}

TangramPiece * MyApp::createLargeTriangle(TangramPiece * root) {
  // Model Matrix
  glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 translationVector = glm::vec3(triangleCathetus * (1.0f/4.0f), 0.0f, -triangleCathetus * (3.0f/4.0f));
  glm::mat4 translation = glm::translate(translationVector);
  glm::mat4 transformation = translation * rotation;
  // Animation variables
  glm::quat boxRotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 boxTranslation = glm::vec3(pieceSize/4, 0.0f, 0.0f) - translationVector;
  TangramPiece *TriangleLarge = new TangramPiece(
    root, TriangleMesh, Shaders, transformation, 
    boxTranslation,  boxRotation, axisRotation,
    ModelMatrixId, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), ColorId
  );
  return TriangleLarge;
}

TangramPiece * MyApp::createLargeTriangle2(TangramPiece * root) {
  // Model Matrix
  glm::mat4 rotation = glm::rotate(glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 translationVector = glm::vec3(- triangleCathetus * (1.0f/4.0f), 0.0f, -triangleCathetus * (3.0f/4.0f));
  glm::mat4 translation = glm::translate(translationVector);
  glm::mat4 transformation = translation * rotation;
  // Animation variables
  glm::quat boxRotation = glm::angleAxis(glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 boxTranslation = glm::vec3(0.0f, 0.0f, - pieceSize/4) - translationVector;
  TangramPiece *TriangleLarge2 = new TangramPiece(
    root, TriangleMesh, Shaders, transformation,
    boxTranslation, boxRotation, axisRotation,
    ModelMatrixId, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), ColorId
  );
  return TriangleLarge2;
}

TangramPiece * MyApp::createSquare(TangramPiece * root) {
  // Model Matrix
  glm::mat4 scale = glm::scale(glm::vec3(scaleSize, 1.0f, scaleSize));
  glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 translation = glm::translate(glm::vec3(- smallSizeHorizontalOffset, 0.0f, 0.0f));
  glm::mat4 transformation = translation * rotation * scale;
  // Animation variables
  glm::vec3 boxTranslation = glm::vec3(smallSizeHorizontalOffset, 0.0f, smallSizeHorizontalOffset);
  TangramPiece *Square = new TangramPiece(
    root, SquareMesh, Shaders, transformation, 
    boxTranslation, glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)), axisRotation,
    ModelMatrixId, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), ColorId
  );
  return Square;
}

TangramPiece * MyApp::createParallelogram(TangramPiece * root) {
  // Model Matrix
  glm::mat4 scale = glm::scale(glm::vec3(scaleSize, 1.0f, scaleSize));
  glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 translationVector = glm::vec3(- mediumSizeHorizontalOffset, 0.0f, downVerticalOffset);
  glm::mat4 translation = glm::translate(translationVector);
  glm::mat4 transformation =  translation * rotation * scale;
  // Animation variables
  glm::quat zRotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::quat yRotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat boxRotation = yRotation * zRotation;
  glm::vec3 boxTranslation = glm::vec3(- (smallSizeHorizontalOffset * 3.0f / 2.0f), 0.0f, - smallSizeHorizontalOffset * (1.0f/2.0f)) - translationVector;
  TangramPiece *Parallelogram = new TangramPiece(
    root, ParallelogramMesh, Shaders, transformation,
    boxTranslation, boxRotation,  axisRotation,
    ModelMatrixId, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), ColorId
  );
  return Parallelogram;
}

///////////////////////////////////////////////////////////////////////// CAMERA

// First Camera position and up vector
glm::vec3 eyeFrontal = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 upFrontal = glm::vec3(0.0f, 1.0f, 0.0f);

// Second Camera position and up vector
glm::vec3 eyeDownward = glm::vec3(0.0f, 5.0f, 0.0f);
glm::vec3 upDownward = glm::vec3(0.0f, 0.0f, -1.0f);

// Center of the scene
glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

// Projection settings
float left = -4.0f;
float right = 4.0f;
float bottom = -4.0f;
float top = 4.0f;
float near = 1.0f;
float far = 10.0f;
float fovy = glm::radians(90.0f);
float ratio = 640.0f / 480.0f;

void MyApp::createCameras() {
  Camera1 = new mgl::OrbitalCamera(
    UBO_BP, true, eyeDownward, center, upDownward, left, right, bottom, top, near, far, fovy, ratio
  );
  Camera2 = new mgl::OrbitalCamera(
    UBO_BP, false, eyeFrontal, center, upFrontal, left, right, bottom, top, near, far, fovy, ratio
  );
  CurrentCamera = Camera1;
  CurrentCamera->setViewMatrix();
  CurrentCamera->setProjectionMatrix();
}

void MyApp::switchCamera() {
  CurrentCamera->unbind();
  CurrentCamera = (CurrentCamera == Camera1) ? Camera2 : Camera1;
  CurrentCamera->bind();
  CurrentCamera->setViewMatrix();
  CurrentCamera->setProjectionMatrix();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {
  Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createMeshes();
  createShaderPrograms();  // after mesh;
  createCameras();
  createSceneGraph();
  createManagers();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
  CurrentCamera->updateViewPort(winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { 
  CurrentCamera->update(elapsed);
  animate(elapsed);
  drawScene();
}

void MyApp::animate(double elapsed) {
  if (KeyManager->isPressed(GLFW_KEY_LEFT)) {
    Scene->changeToBox(elapsed);
  }
  if (KeyManager->isPressed(GLFW_KEY_RIGHT)) {
    Scene->changeToTangram(elapsed);
  }
  Scene->getRoot()->update(elapsed);
}

void MyApp::mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      isMousePressed = true;
    } else if (action == GLFW_RELEASE) {
      isMousePressed = false;
    }
  }
}

void MyApp::cursorCallback(GLFWwindow *win, double xpos, double ypos) {
  if (isMousePressed) {
    double dx = xpos - prevX;
    double dy = ypos - prevY;
    double yaw = glm::radians(dx);
    double pitch = glm::radians(dy);
    CurrentCamera->rotate(yaw, pitch);

  }
  prevX = xpos;
  prevY = ypos;
}

void MyApp::scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
  CurrentCamera->zoom(yoffset);
}

void MyApp::keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS)
    KeyManager->press(key);
  else if (action == GLFW_RELEASE)
    KeyManager->release(key);
  switch(key) {
    case GLFW_KEY_C:
      if (KeyManager->isPressed(key)) {
        switchCamera();
      }
      return;
    case GLFW_KEY_P:
      if (KeyManager->isPressed(key)) {
        CurrentCamera->switchProjection();
      }
      return;
  }
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(800, 600, "Mesh Loader", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
