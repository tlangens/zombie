#ifndef AK47_HH
#define AK47_HH

#include "ranged.hh"
#include "bulletmanager.hh"
#include "bullet.hh"
#include "crosshair.hh"

#include <iostream>

class AK47 : public Ranged
{
public:
	/*Constructor:
		First parameter: 	fireRate
		Second parameter:	damage
		Third parameter:	ammo
		TODO balance everything
	*/
	AK47(CL_ResourceManager& rm) : Ranged(1.5, 10, 30, rm), reloadTime(0)
	{
			ak47Sound = new CL_SoundBuffer("Game/ak47", &rm);
			//rangedSound->set_volume(0.3f);
			ak47Sound->prepare();
			
			noAmmoSound = new CL_SoundBuffer("Game/noAmmo", &rm);
			//rangedSound->set_volume(0.3f);
			noAmmoSound->prepare();
	}
	
	bool reload(double dt)
	{
		reloadTime += dt;
		if(reloadTime > 10)
		{
			ammo = maxAmmo;
			reloadTime = 0;
			return false;
		}
		return true;
	}
	
	bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot)
	{

		if(timeSinceLastShot > fireRate)
		{
			BulletManager* bm = &BulletManager::instance();
			// TODO: this makes baby jesus cry.
			float centerx = 400;
			float centery = 300;
			if(ammo > 0)
			{
				ak47Sound->play();
				float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
				float speed_x = 200 * (crosshair->x - centerx) / length;
				float speed_y = 200 * (crosshair->y - centery) / length;
				bm->add(x, y, speed_x, speed_y, this->damage);
				ammo--;
			}
			else
				noAmmoSound->play();
			return true;
		}
		else
			return false;
		
	}
	
private:
	double reloadTime; // TODO: this in ranged class?
	CL_SoundBuffer* ak47Sound;
	CL_SoundBuffer* noAmmoSound;
	
};
#endif
