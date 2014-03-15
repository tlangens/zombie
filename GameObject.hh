#ifndef GAMEOBJECT_HH
#define GAMEOBJECT_HH

#include <ClanLib/display.h>

class GameObject : public CL_Pointf
{
public:
// Constructor	
// Parameters:
//	rm: resourcemanager for graphics
// gc: Graphics context for the game window
// x: x-coordinate
// y: y-coordinate
// filename: filename for the graphics used

	GameObject(CL_ResourceManager& rm, CL_GraphicContext gc, float x, float y, CL_StringRef filename) : CL_Pointf(x,y), rm(rm)
	{
		sprite = new CL_Sprite(gc, filename, &rm);
	}

	GameObject(float x, float y) : CL_Pointf(x,y)
	{ }

	virtual ~GameObject() 	// virtual destructor
	{
		// delete sprite;
	}
	
	virtual void draw(CL_GraphicContext gc, CL_Pointf player)
	{
		sprite->draw(gc, x - (player.x - 400), y - (player.y - 300));
	}

protected:
	CL_Sprite* sprite;
	CL_ResourceManager rm;
};

#endif
