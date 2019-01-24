#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Application::run()
{
	// Put here code to run before rendering loop
    glClearColor(1,0,0,1);

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        //Matrix
        m_MVMatrix = glm::translate(m_viewController.getViewMatrix(), glm::vec3(0, 0, -5));
        m_normalMatrix = glm::transpose(glm::inverse(m_MVMatrix));

        // Put here rendering code
		const auto fbSize = m_GLFWHandle.framebufferSize();
		glViewport(0, 0, fbSize.x, fbSize.y);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //Projection and Normal Matrix
        glUniformMatrix4fv(m_uMVProjMat, 1, false, glm::value_ptr(m_projMatrix * m_MVMatrix));
        glUniformMatrix4fv(m_uMVMat, 1, false, glm::value_ptr(m_MVMatrix));
        glUniformMatrix4fv(m_uNormMat, 1, false, glm::value_ptr(m_normalMatrix));

        //Lights
        glUniform3fv(m_uLightDir_vs, 1, glm::value_ptr(m_viewController.getViewMatrix() * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)));
        glUniform3fv(m_uLightIn, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

        //Samplers
        glBindSampler(0, m_samplerObj);
        glUniform1i(m_usampler2Da, 0);

        glBindSampler(1, m_samplerObj);
        glUniform1i(m_usampler2Dd, 1);

        glBindSampler(2, m_samplerObj);
        glUniform1i(m_usampler2Ds, 2);

        //Draw
        glBindVertexArray(m_vao);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

        auto indexOffset = 0;
        auto indexShape = 0;
        for(const auto indexCount: m_scData.indexCountPerShape)
        {
            //Textures
            auto materialId = m_scData.materialIDPerShape[indexShape];
            int32_t textureId = -1;

            if(materialId != -1){
                //ambiant
                glUniform3fv(m_uKa, 1, glm::value_ptr(m_scData.materials[materialId].Ka));
                textureId = m_scData.materials[materialId].KaTextureId;
                if(textureId != -1){
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, m_objTextures[textureId]);
                }

                //diffuse
                glUniform3fv(m_uKd, 1, glm::value_ptr(m_scData.materials[materialId].Kd));
                textureId = m_scData.materials[materialId].KdTextureId;
                if(textureId != -1){
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, m_objTextures[textureId]);
                }

                //shininess
                glUniform1f(m_ushininess, m_scData.materials[materialId].shininess);
                glUniform3fv(m_uKs, 1, glm::value_ptr(m_scData.materials[materialId].Ks));
                textureId = m_scData.materials[materialId].KsTextureId;
                if(textureId != -1){
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, m_objTextures[textureId]);
                }
            }

            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (const GLvoid*) (indexOffset * sizeof(GLuint)));
            indexOffset += indexCount;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);

            ++indexShape;
        }

        glBindVertexArray(0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

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
            m_viewController.update(ellapsedTime);
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

    glBindBuffer(GL_ARRAY_BUFFER, m_ibo);
        glBufferStorage(GL_ARRAY_BUFFER, m_scData.indexBuffer.size() * sizeof(m_scData.indexBuffer[0]), m_scData.indexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    //PROGRAM CREATION
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "geometryPass.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "geometryPass.fs.glsl";

    m_program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    m_program.use();

    //VS UNIFORM
    m_uMVProjMat = m_program.getUniformLocation("uModelViewProjMatrix");
    m_uMVMat = m_program.getUniformLocation("uModelViewMatrix");
    m_uNormMat = m_program.getUniformLocation("uNormalMatrix");

    m_usampler2Da = m_program.getUniformLocation("uKaSampler");
    m_usampler2Dd = m_program.getUniformLocation("uKdSampler");
    m_usampler2Ds = m_program.getUniformLocation("uKsSampler");

    m_uKa = m_program.getUniformLocation("uKa");
    m_uKd = m_program.getUniformLocation("uKd");
    m_uKs = m_program.getUniformLocation("uKs");

    m_uLightDir_vs = m_program.getUniformLocation("uLightDir_vs");
    m_uLightIn = m_program.getUniformLocation("uLightIntensity");
    m_ushininess = m_program.getUniformLocation("shininess");

    //Textures
    m_objTextures.resize(m_scData.textures.size());
    glGenTextures(m_scData.textures.size(), m_objTextures.data());

    for(int i =0; i< m_scData.textures.size(); ++i){
        glBindTexture(GL_TEXTURE_2D, m_objTextures[i]);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_scData.textures[i].width(), m_scData.textures[i].height());
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_scData.textures[i].width(), m_scData.textures[i].height(), GL_RGBA, GL_UNSIGNED_BYTE, m_scData.textures[i].data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //GBuffer Textures
    glGenTextures(GBufferTextureCount, m_GBufferTextures);
    for(int i =0; i< GBufferTextureCount; ++i){
        glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[i]);
            glTexStorage2D(GL_TEXTURE_2D, 1, m_GBufferTextureFormat[i], m_nWindowWidth, m_nWindowHeight);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //FBO
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
        for(int i = 0; i < GBufferTextureCount - 1; ++i){
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBufferTextures[i], 0);
        }
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureCount - 1], 0);
        glDrawBuffers(5, drawBuffers);

        auto checkFrameBuffer = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

        if(checkFrameBuffer != GL_FRAMEBUFFER_COMPLETE){
            std::cerr << "Error with FrameBuffer Construction" << std::endl;
        }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glGenSamplers(1, &m_samplerObj);
    glSamplerParameteri(m_samplerObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_samplerObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
}