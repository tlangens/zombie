#ifndef COMBATKNIFE_HH
#define COMBATKNIFE_HH

#include "melee.hh"
#include "crosshair.hh"

class CombatKnife : public Melee
{
public:
	/*Constructor:
		First parameter: 	fireRate
		Second parameter:	damage
		TODO balance everything
	*/
	CombatKnife(CL_ResourceManager& rm, CL_GraphicContext gc) : Melee(15, 100, rm)
	{
		knifeSound = new CL_SoundBuffer("Game/combatKnife", &rm);
		//zombieWave->set_volume(0.3f);
		knifeSound->prepare();

		sprite = CL_Sprite(gc, "Game/knifeSlash", &rm);
	}

	
	bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot)
	{

		if(timeSinceLastShot > fireRate)
		{
			knifeSound->play();
			// TODO: Baby Jesus keeps crying.
			float centerx = 400;
			float centery = 300;

			float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
			float xCN = 30 * (crosshair->x - centerx) / length;
			float yCN = 30 * (crosshair->y - centery) / length;

			ZombieManager* zm = &ZombieManager::instance();

			for(std::list<Zombie>::iterator ziter = zm->getZombies()->begin(); ziter != zm->getZombies()->end();)	
			{
				float distance = sqrt(((ziter->x - (xCN + x)) * (ziter->x - (xCN + x))) + ((ziter->y - (yCN + y)) * (ziter->y - (yCN + y))));

				if(distance < 20)
				{
					if(ziter->takeDamage(damage) < 0)
					{
						ziter = zm->removeZombie(ziter);
					}
					else
						ziter++;
				}
				else
					ziter++;
			}
			return true;
		}
		else
			return false;
	}

//	void draw(CL_GraphicContext gc, Crosshair* crosshair)
//	{
//		// TODO: Baby Jesus keeps crying.
//		float centerx = 400;
//		float centery = 300;

//		float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
//		float xCS = 20 * (crosshair->x - centerx) / length;
//		float yCS = 20 * (crosshair->y - centery) / length;

//		CL_Vec2f vector(centerx - crosshair->x, centery - crosshair->y);
//		CL_Vec2f up(0.0f, 1.0f);

//		// Calculate angle from current sprite position to mouse position

//		float deg = up.angle(vector).to_degrees();
//		if(crosshair->x < centerx)
//			deg = 360.0f - deg;

//		CL_Angle angle(deg, cl_degrees);
//		sprite.set_angle(angle);
//		sprite.draw(gc, centerx + xCS, centery + yCS);
//	}

private:
	CL_SoundBuffer* knifeSound;
	CL_Sprite sprite;
};
#endif


