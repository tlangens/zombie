#ifndef RANGED_HH
#define RANGED_HH

#include "weapon.hh"

class Ranged : public Weapon
{
public:
	Ranged(size_t fireRate, size_t damage, size_t ammo, CL_ResourceManager& rm) : Weapon(fireRate, damage, rm), ammo(ammo), maxAmmo(ammo), reloadTime(0.0)
	{}
	
	virtual bool reload(double dt) = 0;
	
	virtual bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot) = 0;
	
	void addDamage(size_t amount)
	{
		damage += amount;
	}

	size_t getAmmo() const
	{
		return ammo;
	}

protected:
	size_t ammo;
	size_t maxAmmo;
	double reloadTime;
};
#endif
