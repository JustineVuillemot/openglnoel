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

    glmlv::SceneData m_scData;
    glmlv::ViewController m_viewController;
    glmlv::GLProgram m_programGeomatry;
    glmlv::GLProgram m_programShading;

    GLuint m_vbo, m_vboQuad;
    GLuint m_vao, m_vaoQuad;
    GLuint m_ibo, m_iboQuad;
    GLuint m_fbo;

    //FOR VAO
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;
    const GLuint QUAD_VERTEX_ATTR_POS = 0;

    glm::mat4 m_projMatrix, m_MVMatrix, m_normalMatrix, m_MVProjMatrix;
    GLint m_uMVProjMat, m_uMVMat, m_uNormMat, m_usampler2Da, m_usampler2Dd, m_usampler2Ds, m_uKa, m_uKd, m_uKs, m_ushininess; //Geometry pass uniform
    GLint m_uLightDir_vs, m_uLightIn, m_uGPosition, m_uGNormal, m_uGAmbient, m_uGDiffuse, m_uGlossyShininess; //Shading pass uniform

    //Textures
    std::vector<GLuint> m_objTextures;
    GLuint m_samplerObj;

    //GBuffer Textures
    enum GBufferTextureType
    {
        GPosition = 0,
        GNormal,
        GAmbient,
        GDiffuse,
        GGlossyShininess,
        GDepth, // On doit créer une texture de depth mais on écrit pas directement dedans dans le FS. OpenGL le fait pour nous (et l'utilise).
        GBufferTextureCount
    };

    const GLenum m_GBufferTextureFormat[GBufferTextureCount] = { GL_RGB32F, GL_RGB32F, GL_RGB32F, GL_RGB32F, GL_RGBA32F, GL_DEPTH_COMPONENT32F };
    GLuint m_GBufferTextures[GBufferTextureCount];
};