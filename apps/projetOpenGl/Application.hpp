#pragma once



#include <glmlv/filesystem.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/Image2DRGBA.hpp>
#include <map>
#include <tiny_gltf.h>



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
    GLuint vbo[2];
    GLuint vao[2];
    GLuint ibo[2];
    const glmlv::SimpleGeometry cube = glmlv::makeCube();
    const glmlv::SimpleGeometry sphere = glmlv::makeSphere(10);

    glmlv::GLProgram program;

    GLuint textureCube;
    GLuint textureSphere;
    GLuint sampler;

    glmlv::ViewController view;

    GLint modelViewProjMatrix;
    GLint modelViewMatrix;
    GLint normalMatrix;

    GLint directionalLightDir;
    GLint directionalLightIntensity;
    GLint pointLightPosition;
    GLint pointLightIntensity;
    GLint uKd;

    //matrix
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrixCube;
    glm::mat4 MVMatrixSphere;
    glm::mat4 NormalMatrixCube;
    glm::mat4 NormalMatrixSphere;
    glm::mat4 ViewMatrix;

    float anglePhi;
    float angleTheta;
    float intensityDir;
    glm::vec3 colorDir;
    float intensityPoint;
    glm::vec3 colorPoint;
    glm::vec3 pointLightPos;
    float uKdCube[3];
    float uKdSphere[3];

	std::map<int, std::vector<double>> meshMatrix;

	std::map<std::string, int> attribIndexOf;
	std::map<int, int> numberOfComponentOf;
	std::map<int, GLenum> attribEnum;

	std::vector<GLuint> vaos;
	std::vector<tinygltf::Primitive> primitives;
	std::vector<std::vector<double>> matrix;

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	//std::string input_gltf; //filename
	
};