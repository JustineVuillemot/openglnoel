#define  TINYGLTF_IMPLEMENTATION 

#include "Application.hpp"

#include <iostream>
#include <vector>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/ViewController.hpp>


int Application::run()
{
    //sampler
    auto uKdSampler = program.getUniformLocation("uKdSampler");

    ProjMatrix = glm::perspective(glm::radians(70.f), 1.0f*m_nWindowWidth / m_nWindowHeight, 0.1f, 100.0f);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uKdSampler, 0); // Set the uniform to 0 because we use texture unit 0
    glBindSampler(0, sampler);


    //glClearColor(1,0,0,1);

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
        glBindTexture(GL_TEXTURE_2D, textureCube);

        //Lightning - General
        glUniform3fv( directionalLightDir, 1, glm::value_ptr( view.getViewMatrix() * glm::vec4(sin(anglePhi)*cos(angleTheta), sin(anglePhi)*sin(angleTheta), cos(anglePhi), 0)));
        glUniform3fv( directionalLightIntensity, 1, glm::value_ptr( colorDir*intensityDir ));
        glUniform3fv( pointLightPosition, 1, glm::value_ptr( view.getViewMatrix() * glm::vec4(pointLightPos.x, pointLightPos.y, pointLightPos.z, 1)));
        glUniform3fv( pointLightIntensity, 1, glm::value_ptr( colorPoint*intensityPoint ));

        //Envoie des matrices
        glUniformMatrix4fv( modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr( ProjMatrix * MVMatrixCube ));
        glUniformMatrix4fv( modelViewMatrix, 1, GL_FALSE, glm::value_ptr( MVMatrixCube ));
        glUniformMatrix4fv( normalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrixCube ));

        //Lightning - Cube
        glUniform3f(uKd, uKdCube[0], uKdCube[1], uKdCube[2]);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, cube.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        //Envoie des matrices
        glUniformMatrix4fv( modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr( ProjMatrix * MVMatrixSphere ));
        glUniformMatrix4fv( modelViewMatrix, 1, GL_FALSE, glm::value_ptr( MVMatrixSphere ));
        glUniformMatrix4fv( normalMatrix, 1, GL_FALSE, glm::value_ptr( NormalMatrixSphere ));

        //TEXTURES
        glBindTexture(GL_TEXTURE_2D, textureSphere);

        //Lightning - Sphere
        glUniform3f(uKd, uKdSphere[0], uKdSphere[1], uKdSphere[2]);


        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, sphere.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

		glViewport(0, 0, fbSize.x, fbSize.y);

        glBindTexture(GL_TEXTURE_2D, 0);
		//glClear(GL_COLOR_BUFFER_BIT);

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

            if (ImGui::CollapsingHeader("Point Light"))
            {
                ImGui::DragFloat3("Position", glm::value_ptr(pointLightPos));
                ImGui::DragFloat("PointLightIntensity", &intensityPoint);
                ImGui::ColorEdit3("PointLightColor", glm::value_ptr(colorPoint));
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
    view { glmlv::ViewController(m_GLFWHandle.window(), 10) },
    pointLightPos {glm::vec3(0.0f, 10.0f, 0.0f)},
    uKdCube {1.0f, 0.0f, 0.0f},
    uKdSphere {0.0f, 1.0f, 1.0f},
    anglePhi(10),
    angleTheta(10),
    intensityDir(10),
    colorDir {50,50,50},
    intensityPoint(20),
    colorPoint {50,50,50}
{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file


	// Put here initialization code
	glEnable(GL_DEPTH_TEST);


	//Initialize map
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXCOORDS = 2;


	//std::map<int, std::string> attribIndexOf
	attribIndexOf.insert({"POSITION", VERTEX_ATTR_POSITION});
	attribIndexOf.insert({"NORMAL", VERTEX_ATTR_NORMAL});
	//std::map<int, int> numberOfComponentOf
	numberOfComponentOf.insert({TINYGLTF_TYPE_VEC2, 2});
	numberOfComponentOf.insert({TINYGLTF_TYPE_VEC3, 3});

	


	//TINY GLTF
	/*
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	const auto ret = loader.LoadASCIIFromFile(&model, &err, &warn, input_gltf);

	vector<GLuint> buffers(model.buffers.size()); // un par tinygltf::Buffer

	glGenBuffers(buffers.size(), buffers.data())
	Pour chaque model.buffers
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferStorage(GL_ARRAY_BUFFER, model.buffers[i].data.size(), model.buffers[i].data.data(), 0);
	}*/



	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	std::string input_gltf; //filename

	const auto ret = loader.LoadASCIIFromFile(&model, &err, &warn, input_gltf);

	if (!warn.empty()) {
		printf("Warn: %s\n", warn.c_str());
	}

	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}

	if (!ret) {
		printf("Failed to parse glTF\n");
	}

	std::vector<GLuint> buffers(model.buffers.size()); // un par tinygltf::Buffer / c'est les vbos

	glGenBuffers(buffers.size(), buffers.data());
	for (int i = 0; i < model.buffers.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferStorage(GL_ARRAY_BUFFER, model.buffers[i].data.size(), model.buffers[i].data.data(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	std::vector<GLuint> vaos;
	std::vector<tinygltf::Primitive> primitives; /// Pour chaque VAO on va aussi stocker les données de la primitive associé car on doit l'utiliser lors du rendu

	for (int i = 0; i < model.meshes.size(); ++i) {
		for (int j = 0; j < model.meshes[i].primitives.size(); ++j) {
			GLuint vaoId;

			glGenVertexArrays(1, &vaoId);
			glBindVertexArray(vaoId);

			tinygltf::Accessor indexAccessor = model.accessors[model.meshes[i].primitives[j].indices];
			tinygltf::BufferView bufferView = model.bufferViews[indexAccessor.bufferView];
			int bufferIndex = bufferView.buffer;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[bufferIndex]); // Ici on bind le buffer OpenGL qui a été rempli dans la premiere boucle

			for (std::map<std::string, int>::iterator it = model.meshes[i].primitives[j].attributes.begin(); it != model.meshes[i].primitives[j].attributes.end(); ++it) {
				tinygltf::Accessor accesor = model.accessors[model.meshes[i].primitives[j].attributes[it->first]];
				bufferView = model.bufferViews[accesor.bufferView];
				bufferIndex = bufferView.buffer;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferIndex]);
				glEnableVertexAttribArray(attribIndexOf[it->first]); // Ici je suppose qu'on a prérempli une map attribIndexOf qui associe aux strings genre "POSITION" un index d'attribut du vertex shader (les location = XXX du vertex shader); dans les TPs on utilisait 0 pour position, 1 pour normal et 2 pour tex coords
				glVertexAttribPointer(attribIndexOf[it->first], numberOfComponentOf[accesor.type], accesor.componentType, GL_FALSE, bufferView.byteStride, (const GLvoid*)(bufferView.byteOffset + accesor.byteOffset)); // Ici encore il faut avoir remplit une map numberOfComponentOf qui associe un type gltf (comme "VEC2") au nombre de composantes (2 pour "VEC2", 3 pour "VEC3")
			}
			vaos.push_back(vaoId);
			primitives.push_back(model.meshes[i].primitives[j]);
		}
	}

	/*
	vector<GLuint> vaos;
	vector<tinygltf::Primitive> primitives; // Pour chaque VAO on va aussi stocker les données de la primitive associé car on doit l'utiliser lors du rendu
	
	Pour chaque mesh
	{
		Pour chaque primitive
		{
			GLuint vaoId;
			glGenVertexArray(..)
			glBindVertexArray(..)
			indexAccessor = models.accessors[model.mesh[i].primitive[j].indices];
			bufferView = model.bufferViews[indexAccessor.bufferView]
			bufferIndex = bufferView.buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[bufferIndex]) // Ici on bind le buffer OpenGL qui a été rempli dans la premiere boucle
			pour chaque primitive.attributes:
			{
				accesor = models.accessors[model.mesh[i].primitive[j].attributes[key]]; // key est "POSITION", ou "NORMAL", ou autre (voir l'image de spec du format)
				bufferView = model.bufferViews[ accesor   .bufferView]
				bufferIndex = bufferView.buffer
				glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferIndex])
				glEnableVertexAttribArray(attribIndexOf[key]) // Ici je suppose qu'on a prérempli une map attribIndexOf qui associe aux strings genre "POSITION" un index d'attribut du vertex shader (les location = XXX du vertex shader); dans les TPs on utilisait 0 pour position, 1 pour normal et 2 pour tex coords
				glVertexAttribPointer(attribIndexOf[key], numberOfComponentOf[accessor.type], accessor.componentType, bufferView.byteStride, (const void*) (bufferView.byteOffset + accessor.byteOffset) // Ici encore il faut avoir remplit une map numberOfComponentOf qui associe un type gltf (comme "VEC2") au nombre de composantes (2 pour "VEC2", 3 pour "VEC3")
			}
			vaos.push_back(vaoId);
			primitives.push_back(primitive)
		}
	}
	*/

    

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

    //PROGRAM
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "forward.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "forward.fs.glsl";

    program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    program.use();

    //TEXTURES
    glmlv::Image2DRGBA imageCube = glmlv::readImage(m_AssetsRootPath / m_AppName / "textures" / "cube.jpg");
    glmlv::Image2DRGBA imageSphere = glmlv::readImage(m_AssetsRootPath / m_AppName / "textures" / "sphere.jpg");
    //CUBE
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &textureCube);
    
    glBindTexture(GL_TEXTURE_2D, textureCube);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, imageCube.width(), imageCube.height());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageCube.width(), imageCube.height(), GL_RGBA, GL_UNSIGNED_BYTE, imageCube.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //SPHERE
    glGenTextures(1, &textureSphere);

    glBindTexture(GL_TEXTURE_2D, textureSphere);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, imageSphere.width(), imageSphere.height());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageSphere.width(), imageSphere.height(), GL_RGBA, GL_UNSIGNED_BYTE, imageSphere.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    //SAMPLERS
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //getUniform
    //MATRIX
    modelViewProjMatrix = program.getUniformLocation("uModelViewProjMatrix");
    modelViewMatrix = program.getUniformLocation("uModelViewMatrix");
    normalMatrix = program.getUniformLocation("uNormalMatrix");

    //LIGHT
    directionalLightDir = program.getUniformLocation("uDirectionalLightDir");
    directionalLightIntensity = program.getUniformLocation("uDirectionalLightIntensity");
    pointLightPosition = program.getUniformLocation("uPointLightPosition");
    pointLightIntensity = program.getUniformLocation("uPointLightIntensity");
    uKd = program.getUniformLocation("uKd");
    
}