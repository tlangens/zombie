#ifndef MINES_HH
#define MINES_HH

#include "weapon.hh"

class Mines : public Explosives
{
public:
	Mines(CL_ResourceManager& rm) : Explosives(15, 100, 10, rm)
	{}

	bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot)
	{
		if(timeSinceLastShot > fireRate && ammo > 0)
		{
			BulletManager* bm = &BulletManager::instance();
			bm->addMine(x, y, damage);
			ammo--;
			return true;
		}
		else 
			return false;
	}
};


#endif
