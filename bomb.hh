#ifndef BOMB_HH
#define BOMB_HH

#include <iostream>

class Bomb : public CL_Pointf
{
public:
	Bomb(CL_ResourceManager& rm, CL_GraphicContext gc, float x, float y, CL_StringRef filename, size_t damage) : CL_Pointf(x, y), damage(damage)
	{
		bombsprite = new CL_Sprite(gc, filename, &rm);
		explosion = new CL_Sprite(gc, "Game/explosion", &rm);
		sprite = bombsprite;
		exploding = false;
		
		bombDetonateSound = new CL_SoundBuffer("Game/bombDetonate", &rm);
		//rangedSound->set_volume(0.3f);
		bombDetonateSound->prepare();
	}

	~Bomb()
	{
		delete explosion;
		delete bombsprite;
	}

	virtual bool update(double dt, Map* map) = 0;

	bool detonate(ZombieManager* zm)
	{
		exploding = true;
		sprite = explosion;
		bombDetonateSound->play();
		for(std::list<Zombie>::iterator ziter = zm->getZombies()->begin(); ziter != zm->getZombies()->end();)	
		{
			float distance = sqrt(((ziter->x - x) * (ziter->x - x)) + ((ziter->y - y) * (ziter->y - y)));
			if(distance < 70)
			{
				if(ziter->takeDamage(damage) < 0)
				{
					ziter = zm->removeZombie(ziter);
				}
				else ziter++;
			}
			else ziter++;
		}

	}

	void draw(CL_GraphicContext gc, CL_Pointf player)
	{
		sprite->draw(gc, x - (player.x - 400), y - (player.y - 300));
	}

protected:
	CL_Sprite* explosion;
	CL_Sprite* bombsprite;
	CL_Sprite* sprite;
	bool exploding;
	size_t damage;
	CL_SoundBuffer* bombDetonateSound;
};


class Mine : public Bomb
{
public:
	Mine(CL_ResourceManager& rm, CL_GraphicContext gc, float x, float y, size_t damage) : Bomb(rm, gc, x, y, "Game/mine", damage)
	{}

	bool update(double dt, Map* map)
	{
		if(exploding)
		{
			sprite->update(dt * 100);
			if(sprite->is_finished())
				return true;
		}
		else
		{
			ZombieManager* zm = &ZombieManager::instance();
			for(std::list<Zombie>::iterator ziter = zm->getZombies()->begin(); ziter != zm->getZombies()->end(); ziter++)	
			{
				float distance = sqrt(((ziter->x - x) * (ziter->x - x)) + ((ziter->y - y) * (ziter->y - y)));
				if(distance < ziter->getRadius())
				{
					detonate(zm);
					return false;
				}
			}
			return false;
		}
	}

};


class Grenade : public Bomb
{
public:
	Grenade(CL_ResourceManager& rm, CL_GraphicContext gc, float x, float y, size_t damage, CL_Pointf v) : Bomb(rm, gc, x, y, "Game/mine", damage), detonationTime(13), v(v)
	{}

	bool update(double dt, Map* map)
	{
		if(exploding)
		{
			sprite->update(dt * 100);
			if(sprite->is_finished())
				return true;
		}
		else
		{
			ZombieManager* zm = &ZombieManager::instance();
			detonationTime -= dt;
			if(detonationTime < 0)
			{
				detonate(zm);
				return false;
			}

			float dv = sqrt((v.x * v.x) + (v.y * v.y));
			float x0 = x;
			float y0 = y;
			while(sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0)) < dv)
			{
				int intX = ((int)x)/20;
				int intX2 = ((int)(x + (v.x / dv)))/20;

				int intY = ((int)y)/20;
				int intY2 = ((int)(y + (v.y / dv)))/20;

				//Checks if grenade has hit a wall
				if(map->isWall(intX, intY2))
					v.y *= (-0.5);
				if(map->isWall(intX2, intY))
					v.x *= (-0.5);

				x = x + (v.x / dv) * dt;
				y = y + (v.y / dv) * dt;
			}

			v.x /= 1.105;
			v.y /= 1.105;
			return false;
		}
	}

private:
	double detonationTime;
	CL_Pointf v;
};

#endif
