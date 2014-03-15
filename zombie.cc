#include <cmath>
#include <iostream>

#include "zombie.hh"
#include "player.hh"
struct Square
{
	int x;
	int y;
	int distance;
};


Zombie::Zombie(CL_ResourceManager& rm, CL_GraphicContext gc, size_t type, float pos_x, float pos_y) : GameObject(rm, gc, pos_x, pos_y, "Game/zombie"), timeSinceBite(0.0)
{
	biteSound = new CL_SoundBuffer("Game/bite", &rm);
	//zombieWave->set_volume(0.3f);
	biteSound->prepare();
	
	zombieHitSound = new CL_SoundBuffer("Game/zombieHit", &rm);
	//zombieWave->set_volume(0.3f);
	zombieHitSound->prepare();
	
	switch(type) //Giving zombies abilities depending on their type
	{
	case NORMAL:
		speed = 8.0;
		hp = 15;
		damage = 5;
		xp = 10;
		radius = 12;
		sprite = new CL_Sprite(gc, "Game/zombie", &rm);
		break;
	
	case FAST:
		speed = 12.0;
		hp = 5;
		damage = 3;
		xp = 5;
		radius = 10;
		sprite = new CL_Sprite(gc, "Game/fastzombie", &rm);
		break;

	case TOUGH:
		speed = 3.0;
		hp = 40;
		damage = 8;
		xp = 20;
		radius = 14;
		sprite = new CL_Sprite(gc, "Game/strongzombie", &rm);
		break;

	default:
		speed = 5.0;
		hp = 5;
		damage = 5;
		xp = 10;
		radius = 12;
		sprite = new CL_Sprite(gc, "Game/zombie", &rm);
	}
	
}

// Move towards the player with the given speed
void Zombie::move(double dt, Player* player, Map* map)
{
	



	//Store previous coordinates
	float xBefore = x;
	float yBefore = y;		
		
	// calculate direction
	float length = sqrt((player->x - x) * (player->x - x) + (player->y - y) * (player->y - y));
	v.x = speed * (player->x - x) / length;
	v.y = speed * (player->y - y) / length;
	
	//Pathfinding also makes baby Jesus cry
/*	if(length > 20)
	{
		std::list<struct Square> list;
		list.push_back(Square{(int)player->x / 20, (int)player->y / 20, 0});
		
		int i = 0;
		for(std::list<struct Square>::iterator iter = list.begin(); iter != list.end(); iter++)
		{
			std::list<struct Square> tempList;
			
			Square one = *iter;
			one.x++;
			
			Square two = *iter;
			two.x--;
			
			Square three = *iter;
			three.y++;
			
			Square four = *iter;
			four.y--;
			
			tempList.push_back(one);
			tempList.push_back(two);
			tempList.push_back(three);
			tempList.push_back(four);
			
			for(std::list<struct Square>::iterator tempIter = tempList.begin(); tempIter != tempList.end(); )
			{
				if(map->isWall(tempIter->x, tempIter->y))
					tempList.erase(tempIter++);
				else 
				{
					for(std::list<struct Square>::iterator tmpIter = list.begin(); tmpIter != list.end(); tmpIter++)
					{
						if(tmpIter->x == tempIter->x && tmpIter->y == tempIter->y && tmpIter->distance <= tempIter->distance)
						{
							tempList.erase(tempIter++);
							break;
						}
						else if(tmpIter == list.end())
							tempIter++;	
					}					
				}
				
			}			
					
		}
	
		
		
	}*/
	
	
	
	// move zombie
	if(v.x != 0 && !map->isWall((int)(x + v.x) / 20, ((int)y) / 20))
		x += v.x * dt;
	if(v.x != 0 && !map->isWall(((int)x) / 20, (int)(y + v.y) / 20))
		y += v.y * dt;
	
	//Change coordinates to 0-49 so you can call the isWall() function
//	int intX = ((int)x)/20;
//	int intY = ((int)y)/20;
//	
//	//Checks if zombie has hit a wall
//	if(map->isWall(intX, intY))
//			{
//				x = xBefore;
//				y = yBefore;
//			}
	
	timeSinceBite += dt;
	if(sqrt(pow(player->x - x, 2) + pow(player->y - y,2)) < 20 && timeSinceBite > 10)
	{
		this->bite(player);
		timeSinceBite = 0;
	}
}

size_t Zombie::getRadius() const
{
	return radius;
}

size_t Zombie::getXp() const
{
	return xp;
}

size_t Zombie::getHp() const	
{
	return hp;
}


int Zombie::takeDamage(size_t damage)
{
	zombieHitSound->play();
	hp -= damage;
	return hp;
}

size_t Zombie::getDamage()	const
{
	return damage;
}

void Zombie::bite(Player* player)
{
	biteSound->play();
	player->takeDamage(damage);
}

void Zombie::draw(CL_GraphicContext gc, const Player& player)
{
	CL_Vec2f vector(x - player.x, y - player.y);
	CL_Vec2f up(0.0f, 1.0f);

	// Calculate angle from current sprite position to mouse position

	float deg = up.angle(vector).to_degrees();
	if(player.x < x)
		deg = 360.0f - deg;

	CL_Angle angle(deg, cl_degrees);
	sprite->set_angle(angle);
	sprite->draw(gc, (x - (player.x - 400)), (y - (player.y - 300)));
}

