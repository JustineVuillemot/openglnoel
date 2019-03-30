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

#include "Trackball.hpp"


int Application::run()
{
    //sampler
    //auto uKdSampler = program.getUniformLocation("uKdSampler");

	program.use();

    ProjMatrix = glm::perspective(glm::radians(70.f), 1.0f*m_nWindowWidth / m_nWindowHeight, 0.1f, 100.0f);
    
    //glActiveTexture(GL_TEXTURE0);
    //glUniform1i(uKdSampler, 0); // Set the uniform to 0 because we use texture unit 0
    //glBindSampler(0, sampler);


    //glClearColor(1,0,0,1);

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        
        //MVMatrixSphere = glm::translate(view.getViewMatrix(), glm::vec3(2.0f, 0.0f, -5.0f));
        
        //NormalMatrixSphere = glm::transpose(glm::inverse(MVMatrixSphere));

        // Put here rendering code
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto fbSize = m_GLFWHandle.framebufferSize();

        //TEXTURES
        //glBindTexture(GL_TEXTURE_2D, textureCube);

        //Lightning - General
        /*glUniform3fv( directionalLightDir, 1, glm::value_ptr( view.getViewMatrix() * glm::vec4(sin(anglePhi)*cos(angleTheta), sin(anglePhi)*sin(angleTheta), cos(anglePhi), 0)));
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
        glBindVertexArray(0);*/

		/*
		Ensuite pour le rendu on fait une boucle sur tous les VAOs, on bind et on dessine avec glDrawElements, 

		en plus passant le mode de la primitive associé, le nombre d'index (stocké dans le "count" de l'accessor 
		"indices" de la primitive, le type des index (stocké dans l'accessor aussi), et le byteOffset de l'accessor 
		pour l'argument "indices" de la fonction (casté en (const void*)). Toutes ces infos sont accessible à partir 
		du tableau "primitives" qu'on a remplit en meme temps que le tableau "vaos"
		*/

		//std::cout << "test" << std::endl;

		


		for (int i = 0; i < vaos.size(); ++i) {
			//std::cout << meshMatrix[i].size() << std::endl;
			//std::cout << meshMatrix[i][15] << std::endl;
			/*glm::mat4 meshMat{ meshMatrix[i][0], meshMatrix[i][1] , meshMatrix[i][2], meshMatrix[i][3]
								, meshMatrix[i][4], meshMatrix[i][5], meshMatrix[i][6], meshMatrix[i][7]
								, meshMatrix[i][8], meshMatrix[i][9], meshMatrix[i][10], meshMatrix[i][11]
								, meshMatrix[i][12], meshMatrix[i][13], meshMatrix[i][14], meshMatrix[i][15] };*/
				
				
				//= glm::make_mat4(meshMatrix[i].data());
			//MVMatrixCube = glm::translate(meshMat*view.getViewMatrix(), glm::vec3(0.0f, 0.0f, -5.0f));
			MVMatrixCube = glm::translate(view->getViewMatrix(), glm::vec3(0.0f, 0.0f, -5.0f));
			NormalMatrixCube = glm::transpose(glm::inverse(MVMatrixCube));

			//Envoie des matrices
			glUniformMatrix4fv(modelViewProjMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrixCube));
			glUniformMatrix4fv(modelViewMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrixCube));
			glUniformMatrix4fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrixCube));


			glBindVertexArray(vaos[i]);
			
			tinygltf::Accessor indexAccessor = model.accessors[primitives[i].indices];
			glDrawElements(primitives[i].mode, indexAccessor.count, indexAccessor.componentType, (const void*) indexAccessor.byteOffset);
			glBindVertexArray(0);
		}
		//std::cout << "test2" << std::endl;

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
            view->update(ellapsedTime);
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
    view { new Trackball(m_GLFWHandle.window(), 10) },
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
	const GLuint VERTEX_ATTR_TEXCOORDS1 = 3;
	const GLuint VERTEX_ATTR_TANGENT = 4;


	//std::map<int, std::string> attribIndexOf
	attribIndexOf.insert({"POSITION", VERTEX_ATTR_POSITION});
	attribIndexOf.insert({"NORMAL", VERTEX_ATTR_NORMAL});
	attribIndexOf.insert({ "TANGENT", VERTEX_ATTR_TANGENT });
	attribIndexOf.insert({ "TEXCOORD_0", VERTEX_ATTR_TEXCOORDS });
	attribIndexOf.insert({ "TEXCOORD_1", VERTEX_ATTR_TEXCOORDS1 });
	//std::map<int, int> numberOfComponentOf
	numberOfComponentOf.insert({TINYGLTF_TYPE_VEC2, 2});
	numberOfComponentOf.insert({TINYGLTF_TYPE_VEC3, 3});
	numberOfComponentOf.insert({ TINYGLTF_TYPE_VEC4, 4 });
	//std::map<int, GLenum> attribEnum
	/*
	#define GL_POINTS                         0x0000
	#define GL_LINES                          0x0001
	#define GL_LINE_LOOP                      0x0002
	#define GL_LINE_STRIP                     0x0003
	//#define GL_TRIANGLES                      0x0004
	#define GL_TRIANGLE_STRIP                 0x0005
	#define GL_TRIANGLE_FAN                   0x0006
	
	*/
	attribEnum.insert({ 4, GL_TRIANGLES });


	


	//TINY GLTF
	std::cout << argv[1] << std::endl;

	const auto ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);

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

	//std::vector<GLuint> vaos;
	//std::vector<tinygltf::Primitive> primitives; /// Pour chaque VAO on va aussi stocker les données de la primitive associé car on doit l'utiliser lors du rendu

	for (int i = 0; i < model.meshes.size(); ++i) {
		for (int j = 0; j < model.meshes[i].primitives.size(); ++j) {
			GLuint vaoId;

			//std::cout << model.meshes[i].name << std::endl;
			
			

			glGenVertexArrays(1, &vaoId);
			glBindVertexArray(vaoId);
			tinygltf::Accessor indexAccessor = model.accessors[model.meshes[i].primitives[j].indices];
			tinygltf::BufferView bufferView = model.bufferViews[indexAccessor.bufferView];
			int bufferIndex = bufferView.buffer;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[bufferIndex]); // Ici on bind le buffer OpenGL qui a été rempli dans la premiere boucle


			for (std::map<std::string, int>::iterator it = model.meshes[i].primitives[j].attributes.begin(); it != model.meshes[i].primitives[j].attributes.end(); ++it) {
				//std::cout << it->first << std::endl;
				
				tinygltf::Accessor accesor = model.accessors[model.meshes[i].primitives[j].attributes[it->first]];
				bufferView = model.bufferViews[accesor.bufferView];
				bufferIndex = bufferView.buffer;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferIndex]);
				glEnableVertexAttribArray(attribIndexOf[it->first]); // Ici je suppose qu'on a prérempli une map attribIndexOf qui associe aux strings genre "POSITION" un index d'attribut du vertex shader (les location = XXX du vertex shader); dans les TPs on utilisait 0 pour position, 1 pour normal et 2 pour tex coords
				glVertexAttribPointer(attribIndexOf[it->first], numberOfComponentOf[accesor.type], accesor.componentType, GL_FALSE, bufferView.byteStride, (const GLvoid*)(bufferView.byteOffset + accesor.byteOffset)); // Ici encore il faut avoir remplit une map numberOfComponentOf qui associe un type gltf (comme "VEC2") au nombre de composantes (2 pour "VEC2", 3 pour "VEC3")
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				
			}

			vaos.push_back(vaoId);
			vaos.size();
			primitives.push_back(model.meshes[i].primitives[j]);
		}
	}
	//std::cout << vaos.size() << std::endl;


	//get my matrix

	/*for (int i = 0; i < model.nodes.size(); ++i) {
		//if (model.nodes[i].mesh != -1) {
			std::cout << "NODES" << std::endl;
			std::cout << i << std::endl;
			std::cout << "mesh" << std::endl;
			std::cout << model.nodes[i].mesh << std::endl;
			std::cout << "matrix" << std::endl;
			std::cout << model.nodes[i].matrix.size() << std::endl;
			std::cout << model.nodes[i].rotation.size() << std::endl;
			std::cout << model.nodes[i].translation.size() << std::endl;
			std::cout << model.nodes[i].scale.size() << std::endl;
			meshMatrix.insert({ i, model.nodes[i].matrix });
		//}
	}*/



    

    //Construction cube
    
    //glGenBuffers(2, vbo);
    //glGenBuffers(2, ibo);
    //glGenVertexArrays(2, vao);

    /*//______CUBE_____________________________________
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

    glBindVertexArray(0);*/

    //PROGRAM
    const auto pathToSMVS = m_ShadersRootPath / m_AppName / "forward.vs.glsl";
    const auto pathToSMFS = m_ShadersRootPath / m_AppName / "forward.fs.glsl";

    program = glmlv::compileProgram({pathToSMVS, pathToSMFS});
    program.use();

    /*//TEXTURES
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
	*/
    //getUniform
    //MATRIX
    modelViewProjMatrix = program.getUniformLocation("uModelViewProjMatrix");
    modelViewMatrix = program.getUniformLocation("uModelViewMatrix");
    normalMatrix = program.getUniformLocation("uNormalMatrix");

    //LIGHT
    /*directionalLightDir = program.getUniformLocation("uDirectionalLightDir");
    directionalLightIntensity = program.getUniformLocation("uDirectionalLightIntensity");
    pointLightPosition = program.getUniformLocation("uPointLightPosition");
    pointLightIntensity = program.getUniformLocation("uPointLightIntensity");
    uKd = program.getUniformLocation("uKd");*/

	const auto pathToGCCS = m_ShadersRootPath / m_AppName / "gammaCorrect.cs.glsl";

	m_gammaCorrectionProgram = glmlv::compileProgram({ pathToGCCS });
	m_gammaCorrectionProgram.use();

	m_uGammaExponent = m_gammaCorrectionProgram.getUniformLocation("uGammaExponent");
    
}