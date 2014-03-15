#ifndef ITEMMANAGER_HH
#define ITEMMANAGER_HH

#include <list>

#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>

#include "item.hh"
#include "zombie.hh"
#include "player.hh"
#include "map.hh"

enum ItemClass{HP, BOMBS, NUKE};

class ItemManager
{
public:

	~ItemManager();

	static ItemManager& instance();

	void init(Player* newplayer, CL_ResourceManager& rm, CL_GraphicContext gc);

	void drawItems(CL_GraphicContext gc, CL_Pointf player);

	void addItem(float x, float y);

	void pickupItems(Player& player);

	std::list<Item*>::iterator removeItem(std::list<Item*>::iterator item);

private:

	static ItemManager object;
	std::list<Item*> items;
	//CL_SoundBuffer* itemSound;
	CL_ResourceManager rm;
	CL_GraphicContext gc;

	ItemManager();
};

#endif

