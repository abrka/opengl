#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	glm::vec3 position{ 0.0f };
	glm::vec3 normal{ 0.0f };
	glm::vec2 texCoord{ 0.0f };
	glm::vec2 texCoord2{ 0.0f };

	static constexpr std::vector<int> getNumFloatsPerAttr() {
		return { 3,3,2,2 };
	}
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
	os << "x: " << v.x << " y: " << v.y << " z: " << v.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v) {
	os << "x: " << v.x << " y: " << v.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vertex& vt)
{
	os << "position " << vt.position << " normal " << vt.normal << " tex coord " << vt.texCoord;
	return os;
}