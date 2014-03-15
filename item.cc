#include <cmath>
#include <iostream>

#include "item.hh"
#include "player.hh"
#include <cmath>

#define RADIUS 15


Item::Item(CL_ResourceManager& rm, CL_GraphicContext gc, float pos_x, float pos_y, CL_StringRef filename) : GameObject(rm, gc, pos_x, pos_y, filename), radius(RADIUS)
{
	//biteSound = new CL_SoundBuffer("Game/bite", &rm);
	//ItemWave->set_volume(0.3f);
	//biteSound->prepare();
}


// Move towards the player with the given speed

size_t Item::getRadius() const
{
	return radius;
}

void HealthItem::pickup(Player& player)
{
	player.addHp(10);
}

void NukeItem::pickup(Player& player)
{
	ZombieManager::instance().clearZombies();
	std::cout << "BOOOM!!!" << std::endl;
}

void BombsItem::pickup(Player& player)
{
	player.addBombs(3);
}
