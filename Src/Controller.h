#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Settings.h"
#include <iostream>

using namespace std;

namespace Mechanicraft{
	enum CamDir { CamPosXDir = 0, CamNegXDir, CamPosZDir, CamNegZDir };
	CamDir PlayerDir;
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow *window);
	
	// camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 6.0f, 6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 walkDirFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;
	
	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	
	GLFWwindow* InitGL(){
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif
	
		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mechanicraft", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to start GLFW" << std::endl;
			glfwTerminate();
			return nullptr;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
	
		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return nullptr;
		}
		return window;
	}
	
	
	bool CPressed = false;
	bool EPressed = false;
	bool QPressed = false;
	bool MPressed = false;
	bool NeedSaving = false;
	
	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	
		float cameraSpeed = 2.5 * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * walkDirFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * walkDirFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			if (!CPressed) IsCreativeMode = !IsCreativeMode;
			CPressed = true;
		} else CPressed = false;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			if (!EPressed) ShowInventory = !ShowInventory;
			EPressed = true;
		} else EPressed = false;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			if (!QPressed) {
				IDInHand++;
				if(IDInHand >= BlockNum) {
					IDInHand = 1; //Skip Air...
				}
			}
			QPressed = true;
		} else QPressed = false;
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			if (!MPressed) {
				NeedSaving = true;
			} else NeedSaving = false;
			MPressed = true;
		} else {
			MPressed = false;
			NeedSaving = false;
		}
	}
	
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
	
	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
	
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;
	
		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;
	
		yaw += xoffset;
		pitch += yoffset;
	
		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	
		glm::vec3 front;
		yaw = fmod(yaw, 360.0);
		if (yaw < 0.0) yaw += 360.0;
		//cout << yaw << endl;
		if ((yaw >= 0.0 && yaw < 45.0) || (yaw >= 315.0 && yaw <= 360.0)) PlayerDir = CamPosXDir;
		else if (yaw >= 45.0 && yaw < 135.0) PlayerDir = CamPosZDir;
		else if (yaw >= 135.0 && yaw < 225.0) PlayerDir = CamNegXDir;
		else if (yaw >= 225.0 && yaw < 315.0) PlayerDir = CamNegZDir;
		front.y = 0;
		front.x = cos(glm::radians(yaw));
		front.z = sin(glm::radians(yaw));
		walkDirFront = glm::normalize(front);
		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
	
	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		IDInHand += (yoffset > 0) ? 1 : -1;
		if(IDInHand >= BlockNum) {
			IDInHand = 1; //Skip Air...
		} else if(IDInHand <= 0) {
			IDInHand = BlockNum - 1;
		}
	}
}
