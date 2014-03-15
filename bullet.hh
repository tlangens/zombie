#ifndef BULLET_HH
#define BULLET_HH

#include <cmath>
#include "zombiemanager.hh"
#include <ClanLib/display.h>
#include "map.hh"

class Bullet : public CL_Pointf
{
public:

	Bullet(CL_ResourceManager rm, CL_GraphicContext gc, float pos_x, float pos_y, float speed_x, float speed_y, size_t damage) : CL_Pointf(pos_x, pos_y), v(speed_x, speed_y), damage(damage)
	{
		blood = new CL_Sprite(gc, "Game/blood", &rm);
		spark = new CL_Sprite(gc, "Game/bullet", &rm);
		splatting = false;
		sprite = spark;
	}

	~Bullet()
	{
//		delete blood;
//		delete spark;
	}

	void splat()
	{
		splatting = true;
		sprite = blood;
	}

	bool move(double dt, Map* map, ZombieManager* zm)
	{
		if(splatting)
		{
			sprite->update(dt * 100);
			if(sprite->is_finished())
				return false;
		}
		else
		{
//			float x2 = x + v.x * dt;
//			float y2 = y + v.y * dt;

			// Check for collisions along path
			for(int i = 1; i < 20; i++)
			{
				//Checks if bullet is outside map
				if(x <= 0 || x >= 1000 || y <= 0 || y >= 1000)
					return false;

				int intX = ((int)x)/20;
				int intY = ((int)y)/20;

				//Checks if bullet has hit a wall
				if(map->isWall(intX, intY))
					return false;
				x = x + (v.x / 20) * dt;
				y = y + (v.y / 20) * dt;

				// Check for zombies
				for(std::list<Zombie>::iterator ziter = zm->getZombies()->begin(); ziter != zm->getZombies()->end();)	
				{
					float distance = sqrt(((ziter->x - x) * (ziter->x - x)) + ((ziter->y - y) * (ziter->y - y)));

					if(distance < ziter->getRadius())
					{
						splat();
						if(ziter->takeDamage(damage) < 0)
						{
							zm->removeZombie(ziter);
						}
						return true;
					}
					else ziter++;
				}

			}
		}
		return true;
	}
	
	size_t getDamage() const
	{
		return damage;
	}

	void draw(CL_GraphicContext gc, CL_Pointf player)
	{
		sprite->draw(gc, x - (player.x - 400), y - (player.y - 300));
	}
	
	bool isSplatting() const
	{
		return splatting;
	}
	

private:
	size_t damage;
	CL_Sprite* blood;
	CL_Sprite* spark;
	CL_Sprite* sprite;
	CL_Pointf v;
	bool splatting;
};

#endif
