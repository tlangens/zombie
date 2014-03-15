#ifndef SHOTGUN_HH
#define SHOTGUN_HH

#include "ranged.hh"
#include "bulletmanager.hh"
#include "bullet.hh"
#include "crosshair.hh"
#include <cmath>
#include <time.h>

class Shotgun : public Ranged
{
public:
	//Parameters: firerate, damage, ammo
	Shotgun(CL_ResourceManager& rm) :  Ranged(10,3,8,rm)
	{
		shotgunSound = new CL_SoundBuffer("Game/shotgun", &rm);
		//rangedSound->set_volume(0.3f);
		shotgunSound->prepare();
		
		noAmmoSound = new CL_SoundBuffer("Game/noAmmo", &rm);
		//rangedSound->set_volume(0.3f);
		noAmmoSound->prepare();
		
		reloadBuffer = new CL_SoundBuffer("Game/shotgunReload", &rm);
		//rangedSound->set_volume(0.3f);
		reloadSound = reloadBuffer->prepare();
	}
	
	bool reload(double dt)
	{
		reloadTime += dt;
		
		 //Play only when you start reloading and set sound to beginning before playing it
		if(!reloadSound.is_playing() && reloadTime == dt)
			{
				reloadSound.set_position_relative(0.0);
				reloadSound.play();  
			}
		
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
			//TODO: this also makes baby jesus cry
			float centerx = 400;
			float centery = 300;
			if(ammo > 0)
			{
				shotgunSound->play();
				float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
			
				srand(time(NULL));
			
				for(int i = 0; i < 10; i++)
				{
					float speed_x = 200 * (crosshair->x - centerx) / length;
					float speed_y = 200 * (crosshair->y - centery) / length;

					bm->add(x + pow(-1, i)*(rand() % 5), y + pow(-1, i)*(rand() % 5), speed_x + pow(-1, i)*(rand() % 25), speed_y + pow(-1, i)*(rand() % 25), this->damage); //randomizing speed vector and spawnpoints for bullets
				}
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
	CL_SoundBuffer* shotgunSound;
	CL_SoundBuffer* noAmmoSound;
	CL_SoundBuffer* reloadBuffer;
	CL_SoundBuffer_Session reloadSound;
};

#endif
