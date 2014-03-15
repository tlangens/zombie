#ifndef EXPLOSIVES_HH
#define EXPLOSIVES_HH

#include "weapon.hh"

class Explosives : public Weapon
{
public:
	Explosives(size_t fireRate, size_t damage, size_t ammo, CL_ResourceManager& rm) : Weapon(fireRate, damage, rm), ammo(ammo), maxAmmo(ammo)
	{}

	bool reload(double dt) {}

	virtual bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot) = 0;

	size_t getAmmo() const
	{
		return ammo;
	}

	void addAmmo(size_t newammo)
	{
		ammo += newammo;
		if(ammo > maxAmmo)
		{
			ammo = maxAmmo;
		}
	}
	
	void addDamage(size_t amount)
	{
		(int)amount;
	}

protected:
	size_t ammo;
	size_t maxAmmo;
};


#endif
