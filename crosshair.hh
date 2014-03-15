#ifndef CROSSHAIR_HH
#define CROSSHAIR_HH

#include <ClanLib/display.h>

#include "GameObject.hh"

class Crosshair : public GameObject
{
public:

	Crosshair(CL_ResourceManager& rm, CL_GraphicContext gc) : GameObject(rm, gc, 500, 300, "Game/crosshair")
	{}

	void move(double mx, double my)
	{
		x = mx;
		y = my;
	}

	void draw(CL_GraphicContext gc)
	{
		sprite->draw(gc, x, y);
	}

};

#endif
