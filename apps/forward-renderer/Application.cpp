#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/Image2DRGBA.hpp>

int Application::run()
{
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "forward.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "forward.fs.glsl";

    glmlv::GLProgram program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    program.use();

    //TEXTURES
    glmlv::Image2DRGBA imageCube = glmlv::readImage(m_AssetsRootPath / m_AppName / "textures" / "cube.jpg");
    glmlv::Image2DRGBA imageSphere = glmlv::readImage(m_AssetsRootPath / m_AppName / "textures" / "sphere.jpg");

    glActiveTexture(GL_TEXTURE0);

    GLuint textures[2];
    glGenTextures( 2, textures);
    glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGB32I, imageSphere.width(), imageSphere.height());

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageCube.width(), imageCube.height(), GL_RGBA, GL_UNSIGNED_BYTE, imageCube.data());
    glBindTexture(GL_TEXTURE_2D, 0);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 1, 0, 0, imageSphere.width(), imageSphere.height(), GL_RGBA, GL_UNSIGNED_BYTE, imageSphere.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    //SAMPLERS
    GLuint sampler;
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



    //Controller
    glmlv::ViewController view = glmlv::ViewController(m_GLFWHandle.window(), 10);

    //getUniform
    //MATRIX
	GLint modelViewProjMatrix = program.getUniformLocation("uModelViewProjMatrix");
	GLint modelViewMatrix = program.getUniformLocation("uModelViewMatrix");
	GLint normalMatrix = program.getUniformLocation("uNormalMatrix");

    //LIGHT
    GLint directionalLightDir = program.getUniformLocation("uDirectionalLightDir");
    GLint directionalLightIntensity = program.getUniformLocation("uDirectionalLightIntensity");
    GLint pointLightPosition = program.getUniformLocation("uPointLightPosition");
    GLint pointLightIntensity = program.getUniformLocation("uPointLightIntensity");
    GLint uKd = program.getUniformLocation("uKd");

	//matrix
	glm::mat4 ProjMatrix;
	glm::mat4 MVMatrixCube;
    glm::mat4 MVMatrixSphere;
	glm::mat4 NormalMatrixCube;
    glm::mat4 NormalMatrixSphere;
    glm::mat4 ViewMatrix;

    //Lightning
    glm::vec3 dirLightDir = glm::vec3(2.0f, 0.0f, 5.0f);
    glm::vec3 dirLightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 pointLightPos = glm::vec3(0.0f, 10.0f, 0.0f);
    glm::vec3 pointLightInt = glm::vec3(1.0f, 2.0f, 1.0f);
    float uKdCube[3] = {1.0f, 0.0f, 0.0f};
    float uKdSphere[3] = {0.0f, 1.0f, 1.0f};

    //sampler
    auto uKdSampler = program.getUniformLocation("uKdSampler");

    ProjMatrix = glm::perspective(glm::radians(70.f), 1.0f*m_nWindowWidth / m_nWindowHeight, 0.1f, 100.0f);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uKdSampler, 0); // Set the uniform to 0 because we use texture unit 0
    glBindSampler(0, sampler);

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        MVMatrixCube = glm::translate(view.getViewMatrix(), glm::vec3(-2.0f, 0.0f, -5.0f));
        MVMatrixSphere = glm::translate(view.getViewMatrix(), glm::vec3(2.0f, 0.0f, -5.0f));
        NormalMatrixCube = glm::transpose(glm::inverse(MVMatrixCube));
        NormalMatrixSphere = glm::transpose(glm::inverse(MVMatrixSphere));

        // Put here rendering code
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto fbSize = m_GLFWHandle.framebufferSize();

        //TEXTURES
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        //Lightning - general
        glUniform3fv( directionalLightDir, 1, glm::value_ptr( dirLightDir ));
        glUniform3fv( directionalLightIntensity, 1, glm::value_ptr( dirLightIntensity ));
        glUniform3fv( pointLightPosition, 1, glm::value_ptr( pointLightPos ));
        glUniform3fv( pointLightIntensity, 1, glm::value_ptr( pointLightInt ));

        //Envoie des matrices
        glUniformMatrix4fv( modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr( ProjMatrix * MVMatrixCube ));
        glUniformMatrix4fv( modelViewMatrix, 1, GL_FALSE, glm::value_ptr( MVMatrixCube ));
        glUniformMatrix4fv( normalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrixCube ));

        //Lightning - Cube
        glUniform3f(uKd, uKdCube[0], uKdCube[1], uKdCube[2]);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, cube.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Envoie des matrices
        glUniformMatrix4fv( modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr( ProjMatrix * MVMatrixSphere ));
        glUniformMatrix4fv( modelViewMatrix, 1, GL_FALSE, glm::value_ptr( MVMatrixSphere ));
        glUniformMatrix4fv( normalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrixSphere ));

        //TEXTURES
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        //Lightning - Sphere
        glUniform3f(uKd, uKdSphere[0], uKdSphere[1], uKdSphere[2]);


        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, sphere.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

		glViewport(0, 0, fbSize.x, fbSize.y);
		//glClear(GL_COLOR_BUFFER_BIT);

        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            if (ImGui::CollapsingHeader("Directional Light"))
            {
                ImGui::DragFloat3("DirLightIntensity", glm::value_ptr(dirLightIntensity));
                ImGui::DragFloat3("DirLightDir", glm::value_ptr(dirLightDir));
            }

            if (ImGui::CollapsingHeader("Point Light"))
            {
                ImGui::DragFloat3("PointLightIntensity", glm::value_ptr(pointLightInt));
                ImGui::DragFloat3("Position", glm::value_ptr(pointLightPos));
            }

            if (ImGui::CollapsingHeader("Materials"))
            {
                ImGui::ColorEdit3("Cube Kd", uKdCube);
                ImGui::ColorEdit3("Sphere Kd", uKdSphere);
            }

            ImGui::End();
            //ImGui::ShowDemoWindow();
        }

		glmlv::imguiRenderFrame();

        glfwPollEvents(); // Poll for and process events

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
            // Put here code to handle user interactions
            view.update(ellapsedTime);
        }

		m_GLFWHandle.swapBuffers(); // Swap front and back buffers
    }

    return 0;
}

Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" },
    m_AssetsRootPath { m_AppPath.parent_path() / "assets" },
    cube {glmlv::makeCube()} ,
    sphere {glmlv::makeSphere(10)}

{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file

    // Put here initialization code
    glEnable(GL_DEPTH_TEST);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    //Construction cube
    
    glGenBuffers(2, vbo);
    glGenBuffers(2, ibo);
    glGenVertexArrays(2, vao);

    //______CUBE_____________________________________
    //VBO cube
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(glmlv::Vertex3f3f2f)*cube.vertexBuffer.size(), cube.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //IBO cube
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, cube.indexBuffer.size() * sizeof(uint32_t), cube.indexBuffer.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //VAO cube
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

    
    //______SPHERE_____________________________________
    //VBO sphere
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(glmlv::Vertex3f3f2f)*sphere.vertexBuffer.size(), sphere.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //IBO sphere
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sphere.indexBuffer.size() * sizeof(uint32_t), sphere.indexBuffer.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //VAO sphere
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

}