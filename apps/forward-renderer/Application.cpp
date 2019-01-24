#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glmlv/Image2DRGBA.hpp>

int Application::run()
{
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "forward.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "forward.fs.glsl";

    glmlv::GLProgram program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    program.use();

    glmlv::ViewController vc(m_GLFWHandle.window(), 10.f);

    //VS uniform
    GLint ProjMatLoc = program.getUniformLocation("uModelViewProjMatrix");
    GLint MVMatLoc = program.getUniformLocation("uModelViewMatrix");
    GLint normMatLoc = program.getUniformLocation("uNormalMatrix");

    //FS uniform
    GLint directLightDirLoc = program.getUniformLocation("uDirectionalLightDir");
    GLint directLightIntLoc = program.getUniformLocation("uDirectionalLightIntensity");
    GLint ptLightPosLoc = program.getUniformLocation("uPointLightPosition");
    GLint ptLightIntLoc = program.getUniformLocation("uPointLightIntensity");
    GLint uKdLoc = program.getUniformLocation("uKd");

    glm::mat4 ProjMatrix, MVMatrixCube, MVMatrixSphere, normalMatrixCube, normalMatrixSphere, ProjMVMatrix;
    ProjMatrix = glm::perspective(glm::radians(70.f), (float)m_nWindowWidth/(float)m_nWindowHeight, 0.1f, 100.f); // 2 derniers => near / far

    /*******
    Textures
    *******/
    //charger
    const auto pathToCubeTex = m_AssetsRootPath / "textures/cube.jpg";
    const auto pathToSphereTex = m_AssetsRootPath / "textures/sphere.JPG";

    glmlv::Image2DRGBA cubeTex = glmlv::readImage(pathToCubeTex);
    glmlv::Image2DRGBA sphereTex = glmlv::readImage(pathToSphereTex);

    glActiveTexture(GL_TEXTURE0);

    //texture objects
    GLuint cubeTexObj;
    glGenTextures(1, &cubeTexObj);
    glBindTexture(GL_TEXTURE_2D, cubeTexObj);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, cubeTex.width(), cubeTex.height());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cubeTex.width(), cubeTex.height(), GL_RGBA, GL_UNSIGNED_BYTE, cubeTex.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint sphereTexObj;
    glGenTextures(1, &sphereTexObj);
    glBindTexture(GL_TEXTURE_2D, sphereTexObj);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, sphereTex.width(), sphereTex.height());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sphereTex.width(), sphereTex.height(), GL_RGBA, GL_UNSIGNED_BYTE, sphereTex.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    //sampler object
    GLuint samplerObj;
    glGenSamplers(1, &samplerObj);
    glSamplerParameteri(samplerObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(samplerObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //location uniform de texture
    GLint sampler2DLoc = program.getUniformLocation("uKdSampler");

    glBindSampler(0, samplerObj);
    glUniform1i(sampler2DLoc, 0);


    /*****
    For UI
    *****/
    float directLightDir[3] = { 1.0f, 1.0f, 1.0f };
    float directLightInt[3] = { 1.0f, 1.0f, 1.0f };
    float ptLightPos[3] = { 1.0f, 1.0f, -4.0f };
    float ptLightInt[3] = { 1.0f, 1.0f, 1.0f };
    float uKdCube[3] = { 1.0f,0.0f,0.2f };
    float uKdSphere[3] = { 1.0f,1.0f,1.0f };

	// Put here code to run before rendering loop
    //glClearColor(1, 0, 0, 1);
    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        MVMatrixCube = glm::translate(vc.getViewMatrix(), glm::vec3(2, 0, -5));
        MVMatrixSphere = glm::translate(vc.getViewMatrix(), glm::vec3(-2, 0, -5));
        
        normalMatrixCube = glm::transpose(glm::inverse(MVMatrixCube));
        normalMatrixSphere = glm::transpose(glm::inverse(MVMatrixSphere));

        glUniform3fv(directLightDirLoc, 1, glm::value_ptr(vc.getViewMatrix() * glm::vec4(directLightDir[0], directLightDir[1], directLightDir[2], 0)));
        glUniform3fv(directLightIntLoc, 1, directLightInt);
        glUniform3fv(ptLightPosLoc, 1, glm::value_ptr(vc.getViewMatrix() * glm::vec4(ptLightPos[0], ptLightPos[1], ptLightPos[2], 1)));
        glUniform3fv(ptLightIntLoc, 1, ptLightInt);

        // Put here rendering code
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //CUBE
        glUniformMatrix4fv(ProjMatLoc, 1, false, glm::value_ptr(ProjMatrix * MVMatrixCube));
        glUniformMatrix4fv(MVMatLoc, 1, false, glm::value_ptr(MVMatrixCube));
        glUniformMatrix4fv(normMatLoc, 1, false, glm::value_ptr(normalMatrixCube));

        glUniform3f(uKdLoc, uKdCube[0], uKdCube[1], uKdCube[2]);

        glBindTexture(GL_TEXTURE_2D, cubeTexObj);

        glBindVertexArray(vao[0]);
            glDrawElements(GL_TRIANGLES, cube.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        //SPHERE
        glUniformMatrix4fv(ProjMatLoc, 1, false, glm::value_ptr(ProjMatrix * MVMatrixSphere));
        glUniformMatrix4fv(MVMatLoc, 1, false, glm::value_ptr(MVMatrixSphere));
        glUniformMatrix4fv(normMatLoc, 1, false, glm::value_ptr(normalMatrixSphere));

        glUniform3f(uKdLoc, uKdSphere[0], uKdSphere[1], uKdSphere[2]);

        glBindTexture(GL_TEXTURE_2D, sphereTexObj);

        glBindVertexArray(vao[1]);
            glDrawElements(GL_TRIANGLES, sphere.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

		const auto fbSize = m_GLFWHandle.framebufferSize();
		glViewport(0, 0, fbSize.x, fbSize.y);

        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("Light Control");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::InputFloat3("Directional Light Direction", directLightDir);
            ImGui::InputFloat3("Directional Light Intensity", directLightInt);
            ImGui::InputFloat3("Point Light Position", ptLightPos);
            ImGui::InputFloat3("Point Light Intensity", ptLightInt);
            ImGui::ColorEdit3("uKd Cube", uKdCube);
            ImGui::ColorEdit3("uKd Sphere", uKdSphere);
            ImGui::End();
            //ImGui::ShowDemoWindow();
        }

		glmlv::imguiRenderFrame();

        glfwPollEvents(); // Poll for and process events

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
            // Put here code to handle user interactions
            vc.update(ellapsedTime);
        }

		m_GLFWHandle.swapBuffers(); // Swap front and back buffers
    }

    glDeleteTextures(1, &cubeTexObj);
    glDeleteTextures(1, &sphereTexObj);

    return 0;
}

Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" },
    m_AssetsRootPath { glmlv::fs::path{ argv[1] } }

{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file

    glGenBuffers(2, vbo);
    glGenBuffers(2, ibo);
    glGenVertexArrays(2, vao);

    /****
    CUBE
    ****/

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferStorage(GL_ARRAY_BUFFER, cube.vertexBuffer.size() * sizeof(cube.vertexBuffer[0]), cube.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, cube.indexBuffer.size() * sizeof(cube.indexBuffer[0]), cube.indexBuffer.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // VAO
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));
            glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));
            glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /******
    SPHERE
    ******/

    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferStorage(GL_ARRAY_BUFFER, sphere.vertexBuffer.size() * sizeof(sphere.vertexBuffer[0]), sphere.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sphere.indexBuffer.size() * sizeof(sphere.indexBuffer[0]), sphere.indexBuffer.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //VAO
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));
            glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));
            glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    glEnable(GL_DEPTH_TEST);
}