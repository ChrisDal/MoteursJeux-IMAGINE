#include "Components.h"
#include "GameObject.hpp"

double Velocity::DEAD_SPEED = 0.005;
 
void Velocity::move(GameObject* gmo, float dt)
{
	Velocity::clampV();

	// position += velocity 
	gmo->Translate(dt * vx, dt * vy, dt * vz, true); 

	vx *= dv; 
	vy *= dv; 
	vz *= dv; 

	/*std::cout << "Move GMO[" << gmo->getId() << " " << gmo->getTag() 
		<< "] by " << dt * vx << "," << dt * vy << "," << dt * vz << ".\n";*/ 
}


inline void Velocity::clampV()
{
	if (vx < min_v) { vx = min_v; }
	if (vy < min_v) { vy = min_v; }
	if (vz < min_v) { vz = min_v; }

	if (vx > max_v) { vx = max_v; }
	if (vy > max_v) { vy = max_v; }
	if (vz > max_v) { vz = max_v; }

	if (std::abs(vx) < Velocity::DEAD_SPEED) { vx = 0.0;  }
	if (std::abs(vy) < Velocity::DEAD_SPEED) { vy = 0.0;  }
	if (std::abs(vz) < Velocity::DEAD_SPEED) { vz = 0.0;  }

}
