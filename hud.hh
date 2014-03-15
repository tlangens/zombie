#ifndef HUD_HH
#define HUD_HH

#include <ClanLib/display.h>

class HealthBar
{
public:
	HealthBar() {}

	void draw(CL_GraphicContext gc, size_t hp)
	{
		CL_Draw::fill(gc, 620, 20, 770, 25, CL_Colorf(255, 0 , 0));		// Player's max hp must be 100. if changed change this also.
		CL_Draw::fill(gc, 620, 20, (620 + (1.5 * hp)), 25, CL_Colorf(0, 255 , 0));
	}
};

class AmmoBar
{
public:
	AmmoBar() {}

	void draw(CL_GraphicContext gc, size_t ammo)
	{
		for(size_t i = 0; i < ammo; i++)
			CL_Draw::line(gc, 400 + (i * 5), 15, 400 + (i * 5), 30, CL_Colorf(255, 255 , 255));
	}
};



class HUD
{
public:
	HUD(CL_GraphicContext gc, Player* player) : gc(gc), player(player)
	{}

	void draw()
	{
		healthbar.draw(gc, player->getHp());
		ammobar.draw(gc, player->getCurrentWeapon()->getAmmo());
	}

private:
	CL_GraphicContext gc;
	Player* player;
	HealthBar healthbar;
	AmmoBar ammobar;
};


#endif
