#ifndef ZOMBIEMANAGER_HH
#define ZOMBIEMANAGER_HH

#include <list>

#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
//#include "itemmanager.hh"
#include "zombie.hh"
#include "map.hh"



class ZombieManager
{
public:

	~ZombieManager();

	static ZombieManager& instance();

	void init(Player* newplayer, CL_ResourceManager& rm);

	void moveZombies(double dt, Player* player, Map* map);

	void drawZombies(CL_GraphicContext gc, const Player& player);

	void spawnZombies(double dt, CL_ResourceManager& rm, CL_GraphicContext gc);		// TODO: fix spawner so that zombies spawn in waves and over time.

	std::list<Zombie>* getZombies(); //TODO Should be const.....

	std::list<Zombie>::iterator removeZombie(std::list<Zombie>::iterator zombie);

	bool collides(std::list<Zombie>::iterator zombie, Map* map);
	
	Player* getPlayer()
	{return player;}
	
	bool isNearWall(std::list<Zombie>::iterator zombie, Map* map);

	void clearZombies();
	
	int** createMovementMap(Player* player, Map* map);

	

private:

	static ZombieManager object;
	std::list<Zombie> zombies;
	double spawnTime;
	size_t amount;	
	Player* player;
	size_t timeSinceLastBite;
	CL_SoundBuffer* zombieWave;
	CL_ResourceManager rm;
	ZombieManager();
};

#endif

