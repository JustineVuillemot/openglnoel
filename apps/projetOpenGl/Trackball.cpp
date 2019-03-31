#include <glmlv/glfw.hpp>

#include "Trackball.hpp"

float Trackball::distance = 0;

bool Trackball::update(float elapsedTime) {

	auto frontVector = -glm::vec3(m_RcpViewMatrix[2]);
	auto leftVector = -glm::vec3(m_RcpViewMatrix[0]);
	auto upVector = glm::vec3(m_RcpViewMatrix[1]);
	auto position = glm::vec3(m_RcpViewMatrix[3]);

	bool hasMoved = false;
	glm::vec3 localTranslationVector(0.f);

	/* TRANSLATION */

	if ((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) || glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_MIDDLE))
		&& !m_RightButtonPressed && !m_LeftButtonPressed) {
		m_RightButtonPressed = true;
		glfwGetCursorPos(m_pWindow, &m_LastCursorPosition.x, &m_LastCursorPosition.y);
	}
	else if (!glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) && !glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_MIDDLE)
		&& m_RightButtonPressed) {
		m_RightButtonPressed = false;
	}

	if (m_RightButtonPressed) {
		glm::dvec2 cursorPosition;
		glfwGetCursorPos(m_pWindow, &cursorPosition.x, &cursorPosition.y);
		glm::dvec2 delta = cursorPosition - m_LastCursorPosition;

		m_LastCursorPosition = cursorPosition;

		localTranslationVector += 0.1f * delta.x * leftVector;
		localTranslationVector += 0.1f * delta.y * upVector;
	}

	if (distance != 0) {
		localTranslationVector += distance * frontVector;
		distance = 0;
		hasMoved = true;
	}

	position += localTranslationVector;

	if (localTranslationVector != glm::vec3(0.f)) {
		hasMoved = true;
	}

	auto newRcpViewMatrix = m_RcpViewMatrix;

	frontVector = -glm::vec3(newRcpViewMatrix[2]);
	leftVector = -glm::vec3(newRcpViewMatrix[0]);
	upVector = cross(frontVector, leftVector);

	if (hasMoved) {
		setViewMatrix(lookAt(position, position + frontVector, upVector));
	}

	/* ROTATION */

	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) && !m_LeftButtonPressed && !m_RightButtonPressed) {
		m_LeftButtonPressed = true;
		glfwGetCursorPos(m_pWindow, &m_LastCursorPosition.x, &m_LastCursorPosition.y);
	}
	else if (!glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) && m_LeftButtonPressed) {
		m_LeftButtonPressed = false;
	}

	if (m_LeftButtonPressed) {
		glm::dvec2 cursorPosition;
		glfwGetCursorPos(m_pWindow, &cursorPosition.x, &cursorPosition.y);
		glm::dvec2 delta = cursorPosition - m_LastCursorPosition;

		m_LastCursorPosition = cursorPosition;

		if (delta.x || delta.y) {

			if (m_AngleX + delta.y <= -90) {
				delta.y = -90 - m_AngleX;
				m_AngleX = -90;
			}
			else if (m_AngleX + delta.y >= 180) {
				delta.y = 180 - m_AngleX;
				m_AngleX = 180;
			}
			else {
				m_AngleX += delta.y;
			}

			setViewMatrix(glm::rotate(m_ViewMatrix, 0.01f * float(delta.y), glm::vec3(1, 0, 0)));
			setViewMatrix(glm::rotate(m_ViewMatrix, 0.01f * float(delta.x), glm::vec3(0, 0, 1)));
			
			hasMoved = true;
		}
	}

	return hasMoved;
}