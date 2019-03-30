#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glm/ext.hpp>

class Trackball : public glmlv::ViewController {

	public:
		Trackball(GLFWwindow* window, float speed = 1.f) :
			glmlv::ViewController(window, speed) {
			
			glfwSetScrollCallback(m_pWindow, moveFront);
			setViewMatrix(glm::rotate(m_ViewMatrix, -90.0f, glm::vec3(1, 0, 0)));
		}

		static void moveFront(GLFWwindow* window, double xoffset, double yoffset) {
			distance = (float)yoffset;
		}

		bool update(float elapsedTime) override;

		/*Static variable*/
		static float distance;

	private:
		float m_AngleX = 0.f;
		float m_AngleY;

		bool m_RightButtonPressed;
};