#ifndef ITEM_HH
#define ITEM_HH

#include <cmath>
#include <iostream>


#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>

#include "GameObject.hh"
#include "map.hh"

class Player;
class Zombie;


class Item : public GameObject 	
{
	public:
		Item(CL_ResourceManager& rm, CL_GraphicContext gc, float pos_x, float pos_y, CL_StringRef filename);
		
		~Item() {} // destructor
		
		virtual void pickup(Player& player) = 0;
		
		size_t getRadius() const;
	
		bool onItem(Player* player);
		
	protected:
		size_t radius;
		//CL_SoundBuffer* biteSound;

};


class HealthItem : public Item
{
public:
	HealthItem(CL_ResourceManager& rm, CL_GraphicContext gc, float pos_x, float pos_y) : Item(rm, gc, pos_x, pos_y, "Game/health"), player(player)
	{}

	void pickup(Player& player);

private:
	Player& player;
};

class NukeItem : public Item
{
public:
	NukeItem(CL_ResourceManager& rm, CL_GraphicContext gc, float pos_x, float pos_y) : Item(rm, gc, pos_x, pos_y, "Game/nuke")
	{}

	void pickup(Player& player);
};

class BombsItem : public Item
{
public:
	BombsItem(CL_ResourceManager& rm, CL_GraphicContext gc, float pos_x, float pos_y) : Item(rm, gc, pos_x, pos_y, "Game/mines")
	{}

	void pickup(Player& player);
};

#endif
