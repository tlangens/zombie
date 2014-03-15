#include "itemmanager.hh"
#include <time.h>
#include <cmath>
#include "player.hh"


// creating the singleton object.
ItemManager ItemManager::object = ItemManager();


	ItemManager::ItemManager()
	{}

	ItemManager& ItemManager::instance ()
	{
		return object;
	}

	void ItemManager::init(Player* newplayer, CL_ResourceManager& newrm, CL_GraphicContext newgc)
	{
	//	player = newplayer;
		rm = newrm;
		gc = newgc;
	}

	ItemManager::~ItemManager()
	{
		for(std::list<Item*>::iterator iter = items.begin(); iter != items.end();)
		{
			delete *iter;
			iter = items.erase(iter);
		}
	}

	

	void ItemManager::drawItems(CL_GraphicContext gc, CL_Pointf player)
	{
		for(std::list<Item*>::iterator iter = items.begin(); iter != items.end(); iter++)
			(*iter)->draw(gc, player);
	}


	void ItemManager::addItem(float x, float y)
	{
		size_t nr = size_t(rand()) % 100;
		ItemClass type;
		if(nr < 50)
			type = HP;
		else
		{
			if(nr < 90)
				type = BOMBS;
			else
				type = NUKE;
		}
	
		switch(type) //Giving Items abilities depending on their type
		{
		case HP:
		{
			Item* item = new HealthItem(rm, gc, x, y);
			items.push_back(item);
			break;
		}
		case NUKE:
		{
			Item* item = new NukeItem(rm, gc, x, y);
			items.push_back(item);
			break;
		}
		case BOMBS:
		{
			Item* item = new BombsItem(rm, gc, x, y);
			items.push_back(item);
			break;
		}
		default:
			Item* item = new HealthItem(rm, gc, x, y);
			items.push_back(item);
		}
	}

void ItemManager::pickupItems(Player& player)
{
	for(std::list<Item*>::iterator iter = items.begin(); iter != items.end(); iter++)
	{
		float distance = sqrt((((*iter)->x - player.x) * ((*iter)->x - player.x)) + (((*iter)->y - player.y) * ((*iter)->y - player.y)));
		if(distance < (*iter)->getRadius())
		{
			(*iter)->pickup(player);
			removeItem(iter);
			return;
		}
	}
}

std::list<Item*>::iterator ItemManager::removeItem(std::list<Item*>::iterator item)
{
	delete *item;
	return items.erase(item);
}
