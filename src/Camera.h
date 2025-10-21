#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
	unsigned int SCR_WIDTH{};
	unsigned int SCR_HEIGHT{};
	glm::vec3 Position{ 0.0f };
	glm::vec3 Direction{ 0.0f , 0.0f , -1.0f };
	glm::vec3 UpVector{ 0.0f, 1.0f, 0.0f };
	glm::mat4 view{ 1.0f };
	glm::mat4 projection{ 1.0f };

	void RecalculateProjectionMatrix() {
		projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
	}

	void RecalculateViewMatrix() {
		view = glm::lookAt(Position, Position + Direction, UpVector);
	}

	Camera(unsigned int _ScrWidth, unsigned int _ScrHeight) :SCR_WIDTH(_ScrWidth), SCR_HEIGHT(_ScrHeight) 
	{
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	};

	


	/*void SetPosition(glm::vec3 newPos) {
		Position = newPos;
		RecalculateViewMatrix();
	}
	void SetDirection(glm::vec3 newDir) {
		Direction = newDir;
		RecalculateViewMatrix();
	}*/

};