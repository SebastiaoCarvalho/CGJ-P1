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
  /* mgl::SceneGraph *Scene = nullptr; */
  GLint ModelMatrixId;
  GLint CameraId;
  bool isMousePressed = false;
  mgl::Mesh *Mesh = nullptr;
  mgl::KeyManager *KeyManager = nullptr;
  double prevX = 0.0;
  double prevY = 0.0;

  void createSceneGraph();
  void createMeshes();
  void createManagers();
  void createShaderPrograms();
  void createCamera();
  void drawScene();
  void switchCamera();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
  std::string mesh_dir = "../../assets/models/";
  // std::string mesh_file = "cube-v.obj";
  std::string mesh_file = "cube-vn-flat.obj";
  // std::string mesh_file = "cube-vn-smooth.obj";
  // std::string mesh_file = "cube-vt.obj";
  // std::string mesh_file = "cube-vt2.obj";
  // std::string mesh_file = "torus-vtn-flat.obj";
  // std::string mesh_file = "torus-vtn-smooth.obj";
  // std::string mesh_file = "suzanne-vtn-flat.obj";
  // std::string mesh_file = "suzanne-vtn-smooth.obj";
  // std::string mesh_file = "teapot-vn-flat.obj";
  // std::string mesh_file = "teapot-vn-smooth.obj";
  // std::string mesh_file = "bunny-vn-flat.obj";
  // std::string mesh_file = "bunny-vn-smooth.obj";
  // std::string mesh_file = "monkey-torus-vtn-flat.obj";
  std::string mesh_fullname = mesh_dir + mesh_file;

  Mesh = new mgl::Mesh();
  Mesh->joinIdenticalVertices();
  Mesh->create(mesh_fullname);
}

void MyApp::createManagers() { KeyManager = new mgl::KeyManager(); }

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  if (Mesh->hasNormals()) {
    Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (Mesh->hasTexcoords()) {
    Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }
  if (Mesh->hasTangentsAndBitangents()) {
    Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
  }

  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
  CameraId = Shaders->Ubos[mgl::CAMERA_BLOCK].index;
}

///////////////////////////////////////////////////////////////////////// SCENE GRAPH
/* void MyApp::createSceneGraph() {
  Scene = new mgl::SceneGraph();
  mgl::SceneNode *Root = new mgl::SceneNode();
  
}
 */
///////////////////////////////////////////////////////////////////////// CAMERA

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
    glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
    glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 ProjectionMatrix1 =
    glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const glm::mat4 ProjectionMatrix2 =
    glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 1.0f, 10.0f);

void MyApp::createCamera() {
  Camera1 = new mgl::OrbitalCamera(UBO_BP, true, glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  Camera1->updateProjectionMatrix(ProjectionMatrix1);
  Camera2 = new mgl::OrbitalCamera(UBO_BP, false, glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  Camera2->updateProjectionMatrix(ProjectionMatrix2);
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
  printMatrix(CurrentCamera->getProjectionMatrix());
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix(1.0f);

void MyApp::drawScene() {
  Shaders->bind();
  glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  Mesh->draw();
  Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createMeshes();
  createShaderPrograms();  // after mesh;
  createCamera();
  createManagers();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
  // change projection matrices to maintain aspect ratio
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

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
    std::cout << "Cursor: " << xpos << " " << ypos << std::endl;
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
  std::cout << "Scroll: " << yoffset << std::endl;
  CurrentCamera->zoom(yoffset);
  printMatrix(CurrentCamera->getViewMatrix());
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
      // TODO: change projection 
      return;
  }
}

void MyApp::printMatrix(const glm::mat4 &matrix) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
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
