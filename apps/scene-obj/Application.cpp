#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Application::run()
{
	// Put here code to run before rendering loop
    //glClearColor(1,0,0,1);

    //Change unit texture
    glUniform1i(KaLocation, 0); // Set the uniform to 0 because we use texture unit 0
    glUniform1i(KdLocation, 1); // Set the uniform to 1 because we use texture unit 1
    glUniform1i(KsLocation, 2); // Set the uniform to 2 because we use texture unit 2

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        // Put here rendering code
        const auto fbSize = m_GLFWHandle.framebufferSize();
        glViewport(0, 0, fbSize.x, fbSize.y);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Matrix
        MVMatrix = glm::translate(view.getViewMatrix(), glm::vec3(0.0f, 0.0f, -5.0f));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        //Envoie des matrices
        glUniformMatrix4fv( modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr( ProjMatrix * MVMatrix ));
        glUniformMatrix4fv( modelViewMatrix, 1, GL_FALSE, glm::value_ptr( MVMatrix ));
        glUniformMatrix4fv( normalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrix ));

        //samplers
        //glActiveTexture(GL_TEXTURE0);
        glBindSampler(0, sampler);
        //glActiveTexture(GL_TEXTURE1);
        glBindSampler(1, sampler);
        //glActiveTexture(GL_TEXTURE2);
        glBindSampler(2, sampler);

        glBindVertexArray(vao);
        auto indexOffset = 0;
        int indexShape = 0;


        //Lightning - General
        glUniform3fv( directionalLightDir, 1, glm::value_ptr( view.getViewMatrix() * glm::vec4(sin(anglePhi)*cos(angleTheta), sin(anglePhi)*sin(angleTheta), cos(anglePhi), 0)));
        glUniform3fv( directionalLightIntensity, 1, glm::value_ptr( colorDir*intensityDir ));


        for (const auto indexCount: data.indexCountPerShape)
        {
        
            auto materialID = data.materialIDPerShape[indexShape];
            if(materialID != -1){
                glUniform1f( shininess, data.materials[materialID].shininess);
                if(data.materials[materialID].KaTextureId != -1){
                    glActiveTexture(GL_TEXTURE0);
                    glUniform3fv(uKa, 1, glm::value_ptr(data.materials[materialID].Ka));
                    glBindTexture(GL_TEXTURE_2D, textures[data.materials[materialID].KaTextureId]);
                }
                if(data.materials[materialID].KdTextureId != -1){
                    glActiveTexture(GL_TEXTURE1);
                    glUniform3fv(uKd, 1, glm::value_ptr(data.materials[materialID].Kd));
                    glBindTexture(GL_TEXTURE_2D, textures[data.materials[materialID].KdTextureId]);
                }
                if(data.materials[materialID].KsTextureId != -1){
                    glActiveTexture(GL_TEXTURE2);
                    glUniform3fv(uKs, 1, glm::value_ptr(data.materials[materialID].Ks));
                    glBindTexture(GL_TEXTURE_2D, textures[data.materials[materialID].KsTextureId]);
                }
            }
            

            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (const GLvoid*) (indexOffset * sizeof(GLuint)));
            indexOffset += indexCount;
            ++indexShape;
            

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (ImGui::CollapsingHeader("Directional Light"))
            {
                ImGui::DragFloat("Dir angle Phi", &anglePhi);
                ImGui::DragFloat("Dir angle Theta", &angleTheta);
                ImGui::DragFloat("DirLightIntensity", &intensityDir);
                ImGui::ColorEdit3("DirLightColor", glm::value_ptr(colorDir));
            }
            ImGui::End();
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
    view { glmlv::ViewController(m_GLFWHandle.window(), 10) },
    anglePhi(10),
    angleTheta(10),
    intensityDir(10),
    colorDir {50,50,50}
{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file

    // Put here initialization code
    //PROGRAM
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "scene.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "scene.fs.glsl";

    program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    program.use();


    glmlv::loadObjScene(m_AssetsRootPath / m_AppName / "sponza/sponza.obj", data);

    const auto sceneDiagonalSize = glm::length(data.bboxMax - data.bboxMin);
    view.setSpeed(sceneDiagonalSize * 0.2f); // 10% de la scene parcouru par seconde

    //sceneSize = glm::length(data.bboxMax - data.bboxMin);
    ProjMatrix = glm::perspective(70.f, float(m_nWindowWidth) / m_nWindowHeight, 0.01f * sceneDiagonalSize, sceneDiagonalSize); // near = 1% de la taille de la scene, far = 100%


    glEnable(GL_DEPTH_TEST);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    //Construction cube
    
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &vao);

    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(glmlv::Vertex3f3f2f)*data.vertexBuffer.size(), data.vertexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //IBO
    glBindBuffer(GL_ARRAY_BUFFER, ibo);
    glBufferStorage(GL_ARRAY_BUFFER, data.indexBuffer.size() * sizeof(uint32_t), data.indexBuffer.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //VAO
    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    //getUniform
    //MATRIX
    modelViewProjMatrix = program.getUniformLocation("uModelViewProjMatrix");
    modelViewMatrix = program.getUniformLocation("uModelViewMatrix");
    normalMatrix = program.getUniformLocation("uNormalMatrix");

    //Texture
    //textures = new GLuint[data.textures.size()];
    uKd = program.getUniformLocation("uKd");
    uKa = program.getUniformLocation("uKa");
    uKs = program.getUniformLocation("uKs");

    glActiveTexture(GL_TEXTURE0);
    //glGenTextures(data.textures.size(), textures);

    for(int i = 0; i < data.textures.size(); ++i){
        GLuint textureTemp;
        glGenTextures(1, &textureTemp);
        glBindTexture(GL_TEXTURE_2D, textureTemp);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, data.textures[i].width(), data.textures[i].height());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data.textures[i].width(), data.textures[i].height(), GL_RGBA, GL_UNSIGNED_BYTE, data.textures[i].data());
        glBindTexture(GL_TEXTURE_2D, 0);
        textures.push_back(textureTemp);
    }

    //SAMPLERS
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //samplerLocation
    KaLocation = program.getUniformLocation("uKaSampler");
    KdLocation = program.getUniformLocation("uKdSampler");
    KsLocation = program.getUniformLocation("uKsSampler");

    //LIGHT
    directionalLightDir = program.getUniformLocation("uDirectionalLightDir_vs");
    directionalLightIntensity = program.getUniformLocation("uDirectionalLightIntensity");
    shininess = program.getUniformLocation("uShininess");
}