#pragma once
#include "glm/glm.hpp"

struct DirLight {
	glm::vec3 Direction{ 1.0 };
	glm::vec3 Color{ 1.0f };
	float Intensity{ 1.0f };
};

struct PointLight {
	glm::vec3 Position{ 100.0f };
	glm::vec3 Color{ 1.0f, 0.1f, 0.1f };
	float Constant{ 0.80f };
	float Quadratic{ 0.032f };
	float Linear{ 0.01f };
};