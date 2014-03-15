#include "bulletmanager.hh"
#include "player.hh"

// creating the singleton object.
BulletManager BulletManager::object = BulletManager();


	BulletManager::BulletManager()
	{}

	BulletManager::~BulletManager()
	{
		for(std::list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end();)
			iter = bullets.erase(iter);
		for(std::list<Bomb*>::iterator iter = bombs.begin(); iter != bombs.end(); iter++)
			delete *iter;
	}

	BulletManager& BulletManager::instance ()
	{
		return object;
	}

	void BulletManager::init(CL_GraphicContext newgc, CL_ResourceManager& newrm)
	{
		gc = newgc;
		rm = newrm;
	}

	void BulletManager::moveBullets(double dt, Map* map)
	{

		ZombieManager* zm = &ZombieManager::instance();
		float xBefore, yBefore, kBullet, kZombie;
		
		for(std::list<Bullet>::iterator biter = bullets.begin(); biter != bullets.end();)
		{
			if(!biter->move(dt, map, zm))
				biter = removeBullet(biter);
			else
				biter++;
		}
	}

	void BulletManager::drawBullets(CL_GraphicContext gc, CL_Pointf player)
	{
		for(std::list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end(); iter++)
			iter->draw(gc, player);
		for(std::list<Bomb*>::iterator biter = bombs.begin(); biter != bombs.end(); biter++)
			(*biter)->draw(gc, player);
	}

	void BulletManager::add(float x, float y, float speed_x, float speed_y, size_t damage)
	{
		Bullet bullet(rm, gc, x, y, speed_x, speed_y, damage);
		bullets.push_back(bullet);
	}

	void BulletManager::addMine(float x, float y, size_t damage)
	{
		Bomb* bomb = new Mine(rm, gc, x, y, damage);
		bombs.push_back(bomb);
	}

	void BulletManager::addGrenade(float x, float y, size_t damage, CL_Pointf v)
	{
		Bomb* bomb = new Grenade(rm, gc, x, y, damage, v);
		bombs.push_back(bomb);
	}

	void BulletManager::updateBombs(double dt, Map* map)
	{	
		for(std::list<Bomb*>::iterator biter = bombs.begin(); biter != bombs.end();)
		{
			if((*biter)->update(dt, map))
			{
				delete *biter;
				biter = bombs.erase(biter);
			}
			else
				biter++;
		}
	}

	//Removes one bullet from the list
	std::list<Bullet>::iterator BulletManager::removeBullet(std::list<Bullet>::iterator bullet)
	{
		return bullets.erase(bullet);
	} 
	
                    
	std::list<Bullet>* BulletManager::getBullets()
	{
		return &bullets;
	}

