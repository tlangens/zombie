#ifndef CHAINSAW_HH
#define CHAINSAW_HH

#include "melee.hh"
#include "crosshair.hh"

class ChainSaw : public Melee
{
public:
	/*Constructor:
		First parameter: 	fireRate
		Second parameter:	damage
		TODO balance everything
	*/
	ChainSaw(CL_ResourceManager& rm, CL_GraphicContext gc) : Melee(1, 100, rm)
	{
		sound = new CL_SoundBuffer("Game/chainsaw", &rm);
		//rangedSound->set_volume(0.3f);
		chainsawSound = sound->prepare();

		sprite = CL_Sprite(gc, "Game/chainsawSprite", &rm);
	}
	
	bool shoot(float x, float y, Crosshair* crosshair, double timeSinceLastShot)
	{

		if(timeSinceLastShot > fireRate)
		{
			
			
			//Starts the chainsaw sound
			if(!chainsawSound.is_playing()) //If sound has ended
			{
				chainsawSound.set_position_relative(0.0);
				chainsawSound.play();  
			}

			sprite.update(5);

			// TODO: Baby Jesus keeps crying.
			float centerx = 400;
			float centery = 300;

			float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
			float xCS = 20 * (crosshair->x - centerx) / length;
			float yCS = 20 * (crosshair->y - centery) / length;

			ZombieManager* zm = &ZombieManager::instance();

			for(std::list<Zombie>::iterator ziter = zm->getZombies()->begin(); ziter != zm->getZombies()->end();)	
			{
				float distance = sqrt(((ziter->x - (xCS + x)) * (ziter->x - (xCS + x))) + ((ziter->y - (yCS + y)) * (ziter->y - (yCS + y))));

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

	void draw(CL_GraphicContext gc, Crosshair* crosshair)
	{
		// TODO: Baby Jesus keeps crying.
		float centerx = 400;
		float centery = 300;

		float length = sqrt((crosshair->x - centerx) * (crosshair->x - centerx) + (crosshair->y - centery) * (crosshair->y - centery));
		float xCS = 20 * (crosshair->x - centerx) / length;
		float yCS = 20 * (crosshair->y - centery) / length;

		CL_Vec2f vector(centerx - crosshair->x, centery - crosshair->y);
		CL_Vec2f up(0.0f, 1.0f);

		// Calculate angle from current sprite position to mouse position

		float deg = up.angle(vector).to_degrees();
		if(crosshair->x < centerx)
			deg = 360.0f - deg;

		CL_Angle angle(deg, cl_degrees);
		sprite.set_angle(angle);
		sprite.draw(gc, centerx + xCS, centery + yCS);
	}

private:
	CL_SoundBuffer* sound;
	CL_SoundBuffer_Session chainsawSound;
	CL_Sprite sprite;
};
#endif


