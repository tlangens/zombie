#include "zombiemanager.hh"
#include <time.h>
#include <cmath>
#include "player.hh"
#include "itemmanager.hh"
#include <algorithm>
struct Square
{
	int x;
	int y;
	int distance;
};

// creating the singleton object.
ZombieManager ZombieManager::object = ZombieManager();


	ZombieManager::ZombieManager() : timeSinceLastBite(0), spawnTime(0)
	{
		amount = 5;
	}

	ZombieManager& ZombieManager::instance ()
	{
		return object;
	}

	void ZombieManager::init(Player* newplayer, CL_ResourceManager& newrm)
	{
		player = newplayer;
		rm = newrm;
		zombieWave = new CL_SoundBuffer("Game/zombieWave", &rm);
		zombieWave->set_volume(0.99f);
		zombieWave->prepare();
	}

	ZombieManager::~ZombieManager()
	{
		for(std::list<Zombie>::iterator iter = zombies.begin(); iter != zombies.end();)
			iter = zombies.erase(iter);
		delete zombieWave;
	}

	int** ZombieManager::createMovementMap(Player* player, Map* map)
	{
		//-2 = not checked yet
		//-1 = wall
		// 0 = Player
		// 1,2,3,... = steps to the player
		
		int** movementMap;
		//movementMap[(int)player->x / 20][(int)player->y / 20] = 0; // Put players position as 0 in the 2D int-array
		
		
		return movementMap;
	}

	void ZombieManager::moveZombies(double dt, Player* player, Map* map)
	{
		ZombieManager::createMovementMap(player, map);
		
		
		float x = 0.1;
		for(std::list<Zombie>::iterator iter = zombies.begin(); iter != zombies.end(); iter++)	
		{
			
			if(collides(iter, map))	
			{
				x = x*(-1);
				iter->move(dt, player, map);	
			}

			else	
				iter->move(dt, player, map); 
			
		}
	}

	void ZombieManager::drawZombies(CL_GraphicContext gc, const Player& player)
	{
		for(std::list<Zombie>::iterator iter = zombies.begin(); iter != zombies.end(); iter++)
			iter->draw(gc, player);
	}

	//Zombie collision detection
	bool ZombieManager::collides(std::list<Zombie>::iterator zombie, Map* map)	
	{
		float zombieMapX;
		float zombieMapY;
		
		float iterMapY;
		float iterMapX;
		
		
		for(std::list<Zombie>::iterator iter = zombies.begin(); iter != zombies.end(); iter++)	
		{
		/*  zombieMapX = ((int)zombie->x)/20;
			zombieMapY = ((int)zombie->y)/20;
			iterMapX = ((int)iter->x)/20;
			iterMapY = ((int)iter->y)/20;
		*/	
			//if(zombieMapY >= 1 && zombieMapY <= 48 && zombieMapX >= 1 && zombieMapX <= 48)
//				std::cout << !map->isWall(zombieMapX + 1, zombieMapY) << " " << !map->isWall(zombieMapX - 1, zombieMapY) << std::endl; 
			
			if(sqrt(pow(zombie->x - iter->x, 2) + pow(zombie->y - iter->y,2)) < 18 && zombie != iter) //If zombies are closer than 18 pixels from each other	
			{	
				if(zombie->x > iter->x)
				{
					iter->x -= 1;
					zombie->x += 1;
					
					if(map->isWall(((int)zombie->x)/20, ((int)zombie->y)/20))
					{
						while(map->isWall(((int)zombie->x)/20, ((int)zombie->y)/20))
							zombie->x -= 1;
					}
					
					if(map->isWall(((int)iter->x)/20, ((int)iter->y)/20))
					{
						while(map->isWall(((int)iter->x)/20, ((int)iter->y)/20))
							iter->x += 1;
					}	
				}	
				
				
				if(zombie-> y > iter->y)
				{
					iter->y -= 1;
					zombie->y += 1;	
					
					if(map->isWall(((int)zombie->x)/20, ((int)zombie->y)/20))
					{
						while(map->isWall(((int)zombie->x)/20, ((int)zombie->y)/20))
							zombie->y -= 1;
					}
					
					if(map->isWall(((int)iter->x)/20, ((int)iter->y)/20))
					{
						while(map->isWall(((int)iter->x)/20, ((int)iter->y)/20))
							iter->y += 1;
					}
				}
				return true;
			}
		}		
		return false;
	}

	void ZombieManager::spawnZombies(double dt, CL_ResourceManager& rm, CL_GraphicContext gc)		// TODO: fix spawner so that zombies spawn in waves and over time.
	{
	
	//Zombie zombie(rm, gc, 1, 50, 5);  these three rows are used to test the game with only one zombie :)
					
		//if(zombies.empty())
		//	zombies.push_back(zombie);
		
		spawnTime += dt;					
		
		if(50.0 < spawnTime && zombies.size() == 0)
		{
			zombieWave->play();
			spawnTime = 0.0;
			amount += 5;
			for(size_t x = 0; x < amount; x++)	
			{
				if(amount % 2 == 0)
				{	
					Zombie zombie(rm, gc, (x % 3) + 1, (rand()*x*40 % 1000), ((x+1) % 2) * 1000);
					zombies.push_back(zombie);
				}
				else
				{
					Zombie zombie(rm, gc, (x % 3) + 1,((x+1) % 2) * 1000, (rand()*x*40 % 1000));
					zombies.push_back(zombie);
				}
			}
		} 
	}


std::list<Zombie>* ZombieManager::getZombies()
{
	return &zombies;
}

std::list<Zombie>::iterator ZombieManager::removeZombie(std::list<Zombie>::iterator zombie)
{
	player->addXp(zombie->getXp());
	size_t number = size_t(rand()); 

	if(number % 100 < 5)
		ItemManager::instance().addItem(zombie->x, zombie->y);	// adds an item by a certain propability
	return zombies.erase(zombie);	// returns the next zombie-iterator in the list
	
}

//Checks if there is a wall somewhere around the zombie unless the zombie is in a corner
bool ZombieManager::isNearWall(std::list<Zombie>::iterator zombie, Map* map)
{
	int zombieMapX = ((int)zombie->x)/20;
	int zombieMapY = ((int)zombie->y)/20;
	
	if(zombieMapX < 49 && map->isWall(zombieMapX + 1, zombieMapY))
		return true;
	if(zombieMapX < 49 && zombieMapY > 0 && map->isWall(zombieMapX + 1, zombieMapY - 1))
		return true;
	if(zombieMapX < 49 && zombieMapY < 49 && map->isWall(zombieMapX + 1, zombieMapY + 1))
		return true;
	if(zombieMapX > 0 && map->isWall(zombieMapX - 1, zombieMapY))
		return true;
	if(zombieMapX > 0 && zombieMapY < 49 && map->isWall(zombieMapX - 1, zombieMapY + 1))
		return true;
	if(zombieMapX > 0 && zombieMapY > 0 && map->isWall(zombieMapX - 1, zombieMapY - 1))
		return true;
	if(zombieMapY > 0 && map->isWall(zombieMapX, zombieMapY - 1))
		return true;
	if(zombieMapY < 49 && map->isWall(zombieMapX, zombieMapY + 1))
		return true;
	
	return false;
}


void ZombieManager::clearZombies()
{
	zombies.clear();
}

