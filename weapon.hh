#ifndef WEAPON_HH
#define WEAPON_HH

#include <ClanLib/core.h>

#include "crosshair.hh"
#include "bullet.hh"
#include "bulletmanager.hh"

class Weapon
{
public:
	explicit Weapon(size_t fireRate, size_t damage, CL_ResourceManager& rm) : fireRate(fireRate), damage(damage), rm(rm)
	{}
	
	//virtual destructor
	~Weapon() {}
	
	virtual bool reload(double dt) = 0;
	
	virtual bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot) = 0;

	virtual size_t getAmmo() const = 0;

	virtual void addDamage(size_t amount) = 0;

	virtual void draw(CL_GraphicContext gc, Crosshair* crosshair)
	{}
	
	
	size_t getFireRate() const
	{
		return fireRate;
	}
	
	size_t getDamage() const
	{
		return damage;
	}
	

protected:
	size_t fireRate;
	size_t damage;
	CL_ResourceManager rm;
};

#endif




