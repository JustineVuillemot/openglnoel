#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Application::run()
{
	// Put here code to run before rendering loop

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        // Put here rendering code
		const auto fbSize = m_GLFWHandle.framebufferSize();
		glViewport(0, 0, fbSize.x, fbSize.y);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(m_vao);
        auto indexOffset = 0;
        for(const auto indexCount: m_scData.indexCountPerShape)
        {
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (const GLvoid*) (indexOffset * sizeof(GLuint)));
            indexOffset+= indexCount;
        }
        glBindVertexArray(0);

        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

		glmlv::imguiRenderFrame();

        glfwPollEvents(); // Poll for and process events

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
            // Put here code to handle user interactions
        }

		m_GLFWHandle.swapBuffers(); // Swap front and back buffers
    }

    return 0;
}

Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_viewController { m_GLFWHandle.window() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" },
    m_AssetsRootPath { m_AppPath.parent_path() / "assets" }

{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file

    // Put here initialization code

    //SCENE LOADING
    const auto pathToSceneData = m_AssetsRootPath / m_AppName / "sponza/sponza.obj";
    glmlv::loadObjScene(pathToSceneData, m_scData);

    const auto sceneDiagonalSize = glm::length(m_scData.bboxMax - m_scData.bboxMin);
    m_viewController.setSpeed(sceneDiagonalSize * 0.1f);

    m_projMatrix = glm::perspective(70.f, float(m_nWindowWidth) / m_nWindowHeight, 0.01f * sceneDiagonalSize, sceneDiagonalSize);

    //VBO
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferStorage(GL_ARRAY_BUFFER, m_scData.vertexBuffer.size() * sizeof(m_scData.vertexBuffer[0]), m_scData.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //IBO
    glGenBuffers(1, &m_ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, m_scData.indexBuffer.size() * sizeof(m_scData.indexBuffer[0]), m_scData.indexBuffer.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //VAO
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));
            glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));
            glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}