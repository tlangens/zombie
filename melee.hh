#ifndef MELEE_HH
#define MELEE_HH

#include "weapon.hh"

class Melee : public Weapon
{
public:
	Melee(size_t fireRate, size_t damage, CL_ResourceManager& rm) : Weapon(fireRate, damage, rm)
	{}

	bool reload(double dt) {}

	virtual bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot) = 0;

	size_t getAmmo() const
	{
		return 0;
	}
	
	void addDamage(size_t amount)
	{
		(int)amount; 
	}

};
#endif
