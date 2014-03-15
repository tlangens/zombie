#ifndef MAP_HH
#define MAP_HH

#include <string>
#include <iostream>
#include <fstream>
#include <ClanLib/display.h>
#include <stdexcept>

class Map
{
public:
	Map(std::string filename, size_t height, size_t width, CL_ResourceManager rm, CL_GraphicContext gc) : height(height), width(width)
	{

		std::fstream mapFile;											//open file
		mapFile.open(filename, std::fstream::in);

		if(mapFile == NULL)												//if not found then throw
			throw std::invalid_argument("File not found.");

		matrix = new char*[height];

		for(size_t i = 0; i < height; i++)
			matrix[i] = new char[width];


		for(size_t y = 0; y < height; y++)
		{
			for(size_t x = 0; x < width; x++)
				mapFile >> matrix[y][x];
		}

		background = CL_Texture(gc, "Game/background", &rm);
		dirt = CL_Texture(gc, "Game/dirt", &rm);
		wall = CL_Texture(gc, "Game/wall", &rm);
	}

	~Map()
	{
		for(size_t i = 0; i < height; i++)
			delete [] matrix[i];
		delete [] matrix;
	}

	void drawMap(CL_GraphicContext gc, CL_Pointf player)
	{
		float x1 = (0 - (player.x - 400));
		float y1 = (0 - (player.y - 300));
		CL_Rect square(x1, y1, x1 + 1000, y1 + 1000);

		gc.set_texture(0, background);
		CL_Draw::texture(gc, square);
		gc.reset_texture(0);

		for(size_t y = 0; y < height; y++)
		{
			for(size_t x = 0; x < width; x++)
			{
				x1 = (20 * x) - (player.x - 400);
				y1 = (20 * y) - (player.y - 300);
				CL_Rect square(x1, y1, x1 + 20, y1 + 20);
				if(matrix[y][x] == '.')
				{
					continue;
				}
				else
				{
					if(matrix[y][x] == 'w')
					gc.set_texture(0, wall);
				}

				CL_Draw::texture(gc, square);
				gc.reset_texture(0);
			}
		}
	}
	
	bool isWall(int x, int y) const
	{
		if(x > 49 || x < 0 || y < 0 || y > 49)
		{
//			std::cout << "Index out of bounds on isWall-function call :(\n";
//			std::cout << "X: " << x << " Y: " << y << std::endl;
			return false;
		}
		if(matrix[y][x] == 'w')
			return true;
		else 
			return false;
	}
	
	char** getMatrix() const
	{
		return matrix;
	}



private:
	size_t width;
	size_t height;
	char** matrix;

	CL_Texture background;
	CL_Texture dirt;
	CL_Texture wall;
};

#endif
