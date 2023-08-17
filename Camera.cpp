#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// initialize the matrices otherwise they would have null values
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// view matrix help set the camera at the right position and direction
	view = glm::lookAt(Position, Position + Orientation, Up);
	// projection creates perspective for the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::UpdateAspectRatio(int width, int height)
{
	Camera::width = width;
	Camera::height = height;
}

void Camera::SetShaderMatrix(Shader& shader, const char* uniform)
{
	// passing the camera matrix to the shader program
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::ProcessKeyboardInputs(GLFWwindow* window)
{
	// using the time gap between frames to determine the speed, so that the speed stays the same across different hardware
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	float speed = 2.5f * deltaTime * speedScaler;

	// process keyboard inputs with glfw
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speedScaler = 4;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speedScaler = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Camera::ProcessMouseInputs(GLFWwindow* window)
{
	// start controlling camera
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstMouseMove = true;
	}

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
	{
		return;
	}

	// set camera to center so the view won't jump around whenever we start controlling the camera
	if (firstMouseMove)
	{
		glfwSetCursorPos(window, (width / 2), (height / 2));
		firstMouseMove = false;
	}

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// compute the rotation around X/Y axis, using the distance of mouse postion between frames
	float rotationX = sensitivity * (float)(mouseY - (height / 2)) / height;
	float rotationY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// vertical rotation
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotationX), glm::normalize(glm::cross(Orientation, Up)));

	// cap the max vertical rotation for stability
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	// horizontal rotation
	Orientation = glm::rotate(Orientation, glm::radians(-rotationY), Up);

	glfwSetCursorPos(window, (width / 2), (height / 2));
}