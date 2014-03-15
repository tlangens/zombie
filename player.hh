#ifndef PLAYER_HH
#define PLAYER_HH

#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>

#include "GameObject.hh"
#include "weapon.hh"
#include "ranged.hh"
#include "melee.hh"
#include "AK47.hh"
#include "shotgun.hh"
#include "chainsaw.hh"
#include "combatknife.hh"
#include "explosives.hh"
#include "mines.hh"
#include "grenades.hh"
#include "map.hh"




enum PlayerClass{ LUMBERJACK, GI };



class Player : public GameObject
{
public:
	Player(CL_ResourceManager& rm, CL_GraphicContext gc, Crosshair* crosshair, PlayerClass pc);

	~Player();
	void move(bool w, bool a, bool s, bool d, double dt, Map* map);

	void changeWeapon(bool key1, bool key2, bool key3);

	void shoot(bool clicked, double dt);

	void draw(CL_GraphicContext gc, Crosshair* crosshair);

	size_t getHp()	const;

	void takeDamage(size_t damage);

	bool isDead();
	
	void addXp(size_t exp); 
	
	void reload(bool R, double dt);

	size_t getLevel() const;
	
	size_t getXp() const;

	void addHp(size_t newhp);

	void addBombs(size_t newbombs);
	
	Weapon* getCurrentWeapon() const;
	
	size_t getSkillpoints() const;
	
	void addSkillpoint();
	
	bool removeSkillpoints(size_t amount);
	
	void addMaxHp(size_t amount);
	
	void addSpeed(size_t amount);
	
//	
//	
//	bool addXp(size_t exp); //returns true if player leveled up
//	
//	//Below this comment are only get-functions to get private members (all const)
//	
//	Weapon* getExplosives() const; // Weapon*? Or Explosives* etc.
//	
//	Weapon* getRanged() const;
//	
//	Weapon* getMelee() const;
//	
//	size_t getDamage() const;
//	
//	size_t getSpeed() const;
//	
//	
//	size_t getLevel() const; //Maybe count level from experience? -> this function unnecessary
//	
//	int getHp() const;
	


private:
	
	CL_ResourceManager rm;
	CL_GraphicContext gc;
	Crosshair* crosshair;

	//RO3
	Player(const Player&);
	Player& operator=(const Player&);
	
	Weapon* currentWeapon; //holds track of which of the weapons is currently in use

	Explosives* explosives;
	Ranged* ranged;
	Melee* melee;
	
	float speed;
	size_t hp;
	size_t maxhp;
	
	size_t experience;
	size_t level;
	size_t xpNeeded;
	int skillpoints;
	
	double timeSinceLastShot;
	bool isReloading;
	
	
	
};

#endif
