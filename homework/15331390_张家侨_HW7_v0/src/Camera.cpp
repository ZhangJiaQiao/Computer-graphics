#include"Camera.h"

Camera::Camera(glm::vec3 position,
	glm::vec3 up,
	float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float v = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * v;
	if (direction == BACKWARD)
		Position -= Front * v;
	if (direction == LEFT)
		Position -= Right * v;
	if (direction == RIGHT)
		Position += Right * v;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
	GLboolean constrainPitch = true;
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}