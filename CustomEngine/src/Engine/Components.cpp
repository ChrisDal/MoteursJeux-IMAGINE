#include "Components.h"
#include "GameObject.hpp"

void Velocity::move(const glm::vec3& direction, GameObject* gmo){
	glm::vec3 pos = gmo->Position() + glm::vec3(direction.x * vx,
		direction.y * vy,
		direction.z * vz);
	gmo->Position(pos);
};
