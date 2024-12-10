#include "./mglScenegraph.hpp"

namespace mgl {

    SceneGraph::SceneGraph() {}

    SceneGraph::~SceneGraph() {}

    void SceneGraph::draw() {
        Root->draw();
    }

    SceneNode *SceneGraph::getRoot() {
        return Root;
    }

    void SceneGraph::setRoot(SceneNode *root) {
        Root = root;
    }

    SceneNode::SceneNode(
        SceneNode * parent, mgl::Mesh *mesh, mgl::ShaderProgram *shaderprogram, 
        const glm::mat4 &modelmatrix, GLuint modelmatrixid
        ) {
        Parent = parent;
        Mesh = mesh;
        Shaders = shaderprogram;
        ModelMatrix = modelmatrix;
        ModelMatrixId = modelmatrixid;
    }

    SceneNode::~SceneNode() {}

    void SceneNode::setMesh(mgl::Mesh *mesh) {
        Mesh = mesh;
    }

    void SceneNode::setShaderProgram(mgl::ShaderProgram *shaderprogram) {
        Shaders = shaderprogram;
    }

    void SceneNode::setModelMatrix(const glm::mat4 &modelmatrix) {
        ModelMatrix = modelmatrix;
    }

    void SceneNode::addChild(SceneNode *child) {
        Children.push_back(child);
    }

    void SceneNode::draw() {
        if (Mesh) drawSelf();
        for (auto child : Children) {
            child->draw();
        }
    }

    void SceneNode::drawSelf() {
        Shaders->bind();
        glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        Mesh->draw();
        Shaders->unbind();
    }

    void SceneNode::update(double deltaTime) {
        for (auto &child : Children) {
            child->update(deltaTime);
        }
    }
}