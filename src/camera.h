#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:

	//atributos da camera
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	//options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	//construtor com vector
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;

		updateCameraVectors();

	}
	//construtor com escalares
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// MOVENDO A CAMERA | POSICAO 	     TARGET               UP
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	void processKeyboard(Camera_Movement direction, float deltaTime)
	{
		glm::vec3 directionVector = glm::vec3(0.0f);
		
		float velocity = movementSpeed * deltaTime;

		if (direction == FORWARD)
			directionVector += front;
		if (direction == BACKWARD)
			directionVector -= front;
		if (direction == LEFT)
			directionVector -= right;
		if (direction == RIGHT)
			directionVector += right;
		if (direction == UP)
			directionVector += worldUp;
		if (direction == DOWN)
			directionVector -= worldUp;

		// Normaliza o vetor resultante se houver movimento
		if (glm::length(directionVector) > 0.0f)
			directionVector = glm::normalize(directionVector);

		// Atualiza a posição
		position += directionVector * velocity;
	}

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw = glm::mod(yaw + xoffset, 360.0f);
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		zoom -= (float)yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 90.0f)
			zoom = 90.0f;
	}


private:

	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(this->front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, this->front));
	}

};


#endif // !CAMERA_H
