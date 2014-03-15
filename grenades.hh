#ifndef GRENADES_HH
#define GRENADES_HH

#include "weapon.hh"

class Grenades : public Explosives
{
public:
	Grenades(CL_ResourceManager& rm) : Explosives(15, 100, 10, rm)
	{}

	bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot)
	{
		if(timeSinceLastShot > fireRate && ammo > 0)
		{
			BulletManager* bm = &BulletManager::instance();
			// Baby jesus, etc...
			float centerx = 400;
			float centery = 300;
			float speed_x = (crosshair->x - centerx) / 10;
			float speed_y = (crosshair->y - centery) / 10;
			bm->addGrenade(x, y, damage, CL_Pointf(speed_x, speed_y));
			ammo--;
			return true;
		}
		else
			return false;
	}
};


#endif
