#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;


class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensa;
	float mouseZoom;


	Camera( glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
			float Yaw = YAW,
			float Pitch = PITCH)  :  
			front(0.0f, 0.0f, -1.0f),
			movementSpeed(SPEED),
			mouseSensa(SENSITIVITY),
			mouseZoom(ZOOM)
	{
		position = Position;
		worldUp = Up;
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Yaw, float Pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensa(SENSITIVITY), mouseZoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	void processKeyboard(camera_movement direction, float deltaTime) {
		float velocity = movementSpeed * deltaTime;
		if(direction == FORWARD)
			position += front * velocity;
		if(direction == BACKWARD)
			position -= front * velocity;
		if(direction == LEFT)
			position -= right * velocity;
		if(direction == RIGHT)
			position += right * velocity;
	}

	void processMouseMovement(float xoffset, float yoffset, bool constraintPitch = true) {
		xoffset *= mouseSensa;
		yoffset *= mouseSensa;

		yaw   += xoffset;
		pitch += yoffset;

		if(constraintPitch) {
			if(pitch > 89.0f)
				pitch = 89.0f;
			if(pitch < -89.0f)
				pitch = -89.0f;
		}

		updateCameraVectors();
	}

	void processMouseScroll(float yoffset) {
		mouseZoom -= (float)yoffset;
		if(mouseZoom < 1.0f)
			mouseZoom = 1.0f;
		if(mouseZoom > 45.0f)
			mouseZoom = 45.0f; 
	}


private:
	void updateCameraVectors() {
		glm::vec3 tempFront;
		tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		tempFront.y = sin(glm::radians(pitch));
		tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(tempFront);


		right = glm::normalize(glm::cross(front, worldUp));
		up    = glm::normalize(glm::cross(right, front));
	}
};


#endif