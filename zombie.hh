#ifndef ZOMBIE_HH
#define ZOMBIE_HH

#include <cmath>
#include <iostream>


#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>

#include "GameObject.hh"
#include "map.hh"



class Player;

enum ZombieClass{NORMAL, FAST, TOUGH};



class Zombie : public GameObject 	
{
	public:
		Zombie(CL_ResourceManager& rm, CL_GraphicContext gc, size_t type = NORMAL, float pos_x = 0.0, float pos_y = 0.0);
		
		~Zombie() {} // destructor

		// Move towards the player with the given speed
		void move(double dt, Player* player, Map* map);

		size_t getHp() const;
		
		int takeDamage(size_t damage);
	 	
		size_t getXp() const;
		
		size_t getDamage() const;

		void bite(Player* player);

		void draw(CL_GraphicContext gc, const Player& player);
		
		size_t getType();
		
		size_t getRadius() const;
		
	private:
		int hp;
		CL_Pointf v;
		float speed;
		size_t damage;
		double timeSinceBite;
		size_t xp;
		size_t type;
		size_t radius;
		CL_SoundBuffer* biteSound;
		CL_SoundBuffer* zombieHitSound;
};


#endif
