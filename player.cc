#include <cmath>
#include <iostream>


#include "player.hh"


	Player::Player(CL_ResourceManager& rm, CL_GraphicContext gc, Crosshair* crosshair, PlayerClass pc = LUMBERJACK) : GameObject(rm, gc, 500, 500, "Game/player"), rm(rm), gc(gc), crosshair(crosshair), timeSinceLastShot(0), level(1), experience(0), isReloading(false), xpNeeded(200), skillpoints(0)
	{
	
		if(pc == LUMBERJACK)
		{
				ranged = new Shotgun(rm);
				explosives = new Mines(rm);
				melee = new ChainSaw(rm, gc);
				currentWeapon = ranged;
				speed = 10;
				hp = 100;
				maxhp = hp;
				
		}
		
		else if(pc == GI)
		{
			ranged = new AK47(rm);
			explosives = new Grenades(rm);
			melee = new CombatKnife(rm, gc);
			currentWeapon = ranged;
			speed = 10;
			hp = 100;
		}
	}

	Player::~Player()
	{
		delete ranged;
		delete melee;
		delete explosives;
	}

	void Player::move(bool w, bool a, bool s, bool d, double dt, Map* map)
	{
		float xBefore = x;
		float yBefore = y;
		
		int intX = x;
		int intY = y;
		intX = intX/20;
		intY = intY/20;
		
		float xSpeed = 0;
		float ySpeed = 0;
		//Moves the player accordingly to buttons pressed	
		if(w || a || s || d)
		{
			if(w)
				ySpeed -= 1;
			if(s)
				ySpeed += 1;
			if(a)
				xSpeed -= 1;
			if(d)
				xSpeed += 1;
			
			double length = sqrt(xSpeed*xSpeed + ySpeed*ySpeed);

			int newX = (int)(x + (xSpeed/length) * 10)/20;
			int newY = (int)(y + (ySpeed/length) * 10)/20;

			if(xSpeed != 0 && length != 0 && !map->isWall(newX, int(y / 20)))
				x += (xSpeed/length) * speed * dt;

			if(ySpeed != 0 && length != 0 && !map->isWall(int(x / 20), newY))
				y += (ySpeed/length) * speed * dt;				
		}
		
		
		//Player cannot go outside map
		if(x < 1)
			x = 1;
		if(x > 999)
			x = 999;
		if(y < 1)
			y = 1;
		if(y > 999)
			y = 999;	
	}

	void Player::changeWeapon(bool key1, bool key2, bool key3)
	{
		if(key1 && !isReloading)
			currentWeapon = ranged;
		if(key2 && !isReloading)
			currentWeapon = melee;
		if(key3 && !isReloading)
			currentWeapon = explosives;
	}
	
	//Calls the weapons shoot function
	void Player::shoot(bool clicked, double dt)
	{
		timeSinceLastShot += dt;
		if(clicked && !isReloading)
		{
			if(currentWeapon->shoot(x, y, crosshair, timeSinceLastShot)) //calls the weapons shoot function
			{
				timeSinceLastShot = 0;
			}
		}
	}
	
	void Player::reload(bool R, double dt)
	{
		if(R)
			isReloading = true;
		if(isReloading)
			isReloading = currentWeapon->reload(dt);
	}

	void Player::draw(CL_GraphicContext gc, Crosshair* crosshair) //TODO: players coordinates to window not always 400, 300?
	{
		CL_Vec2f vector(400 - crosshair->x, 300 - crosshair->y);
		CL_Vec2f up(0.0f, 1.0f);

	// Calculate angle from current sprite position to mouse position
	
		float deg = up.angle(vector).to_degrees();
		if(crosshair->x < 400)
			deg = 360.0f - deg;

		CL_Angle angle(deg, cl_degrees);
		sprite->set_angle(angle);
		sprite->draw(gc, 400, 300);
		currentWeapon->draw(gc, crosshair);
	}
	
	
	bool Player::isDead()	{
			if(hp == 0)	
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
	void Player::takeDamage(size_t damage)	{
			if(int(this->
			getHp() - damage) < 0)	
			{
				hp = 0;
			}
			else	
			{
				hp-=damage;
			}
		}
		
	size_t Player::getHp() const
	{
		return this->hp;
	}
		
	//Returns true if player leveled up
	void Player::addXp(size_t exp)
	{
		
		this->experience += exp;
		
		if(this->experience >= xpNeeded)
		{
			this->level++;
			addSkillpoint();
			std::cout << "LEVEL UP! New level: " << this->level << std::endl;
			xpNeeded = 1.5*this->experience + 200;
		}
	}
	
	size_t Player::getLevel() const
	{
		return this->level;
	}
	
	size_t Player::getSkillpoints() const
	{
		return this->skillpoints;
	}
	
	void Player::addSkillpoint()
	{
		this->skillpoints++;
	}
	
	bool Player::removeSkillpoints(size_t amount)
	{
		if(amount > this->skillpoints)
			return false;
		else 
		{
			this->skillpoints -= amount;
			return true;
		}
	}
	
	Weapon* Player::getCurrentWeapon() const
	{
		return this->currentWeapon;
	}


	size_t Player::getXp() const
	{
		return this->experience;
	}

	void Player::addHp(size_t newhp)
	{
		hp += newhp;
		if(hp > maxhp)
			hp = maxhp;
	}
	
	void Player::addMaxHp(size_t amount)
	{
		this->maxhp += amount;
	}

	void Player::addBombs(size_t newbombs)
	{
		explosives->addAmmo(newbombs);
	}
	
	
	void Player::addSpeed(size_t amount)
	{
		this->speed += amount;
	}
/*	
	
	
	//returns true if player dies. Parameter Zombie* or size_t?
	bool Player::takedamage(size_t damage)
	{
		this->hp -= damage;
		if(this->hp <= 0)
		{
			//TODO everything that happens when player dies
			return true; //player died
		}
		else
		{
			//TODO the stuff that happens when player doens't die. Maybe nothing?
			return false; //player is still alive
		}
	}
	
	
	//returns true if player leveled up
	bool Player::addXp(size_t exp)
	{
		this->xp += exp;
		if( //TODO some way to count how much xp is needed for level )
		{
			//TODO level-system
			return true;  //player leveled
		}
		else
		{
			return false; //player did not level
		}
	}	
*/	
//	Below this comment are only get-functions to get private members (all const)
/*	
	Explosives* Player::getExplosives() const
	{
		return this->explosives;
	} 
	
	Ranged* Player::getRanged() const
	{
		return this->ranged;
	}
	
	Melee* Player::getMelee() const
	{
		return this->melee;
	}
	size_t Player::getSpeed() const
	{
		return this->speed;
	}
	
	size_t Player::getLevel() const
	{
		return this->level;
	}
	*/
	
	


