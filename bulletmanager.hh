#ifndef BULLETMANAGER_HH
#define BULLETMANAGER_HH

#include <list>

#include "map.hh"
#include "bullet.hh"
#include "bomb.hh"
#include "zombiemanager.hh"


class BulletManager
{
public:

	static BulletManager& instance();

	~BulletManager();

	void init(CL_GraphicContext gc, CL_ResourceManager& rm);

	void moveBullets(double dt, Map* map);

	void updateBombs(double dt, Map* map);

	void drawBullets(CL_GraphicContext gc, CL_Pointf player);
	
	void add(float x, float y, float speed_x, float speed_y, size_t damage);

	void addMine(float x, float y, size_t damage);

	void addGrenade(float x, float y, size_t damage, CL_Pointf v);

	//Removes one bullet from the list
	std::list<Bullet>::iterator removeBullet(std::list<Bullet>::iterator bullet);

	std::list<Bullet>* getBullets();

	void damageZombies(ZombieManager* zm);


private:
	std::list<Bullet> bullets;
	std::list<Bomb*> bombs;

	CL_ResourceManager rm;
	CL_GraphicContext gc;

	static BulletManager object;

	BulletManager();

};

#endif
