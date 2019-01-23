#pragma once

#include <glmlv/filesystem.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/scene_loading.hpp>

class Application
{
public:
    Application(int argc, char** argv);

    int run();
private:
    const size_t m_nWindowWidth = 1280;
    const size_t m_nWindowHeight = 720;
    glmlv::GLFWHandle m_GLFWHandle{ m_nWindowWidth, m_nWindowHeight, "Template" }; // Note: the handle must be declared before the creation of any object managing OpenGL resource (e.g. GLProgram, GLShader)

    const glmlv::fs::path m_AppPath;
    const std::string m_AppName;
    const std::string m_ImGuiIniFilename;
    const glmlv::fs::path m_ShadersRootPath;
    const glmlv::fs::path m_AssetsRootPath;

    glmlv::SceneData data;
    GLuint vbo, vao, ibo;

    glmlv::ViewController view;

    //float sceneSize;

    glmlv::GLProgram program;

    GLint modelViewProjMatrix;
    GLint modelViewMatrix;
    GLint normalMatrix;

    float anglePhi;
    float angleTheta;
    float intensityDir;
    glm::vec3 colorDir;

    GLint directionalLightDir;
    GLint directionalLightIntensity;
    GLint shininess;
    GLint uKd;
    GLint uKa;
    GLint uKs;

    //matrix
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 ViewMatrix;

    //Texture
    std::vector<GLuint> textures;
    GLuint sampler;
    GLuint KaLocation, KdLocation, KsLocation;
};