#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <stdlib.h>

#include "map.hh"
#include "player.hh"
#include "crosshair.hh"
#include "zombie.hh"
#include "bullet.hh"
#include "bulletmanager.hh"
#include "zombiemanager.hh"
#include "itemmanager.hh"
#include "hud.hh"


#define HEIGHT 600
#define WIDTH 800

struct Highscore
{
//	size_t rank;
	size_t score;
	std::string name;
	/*bool operator <(Highscore& rhs) 	{
		return this.score < rhs.score;
	} */
};



class World
{
private:

	CL_DisplayWindow &window;
	CL_InputDevice keyboard;
	CL_InputDevice mouse;
	CL_ResourceManager &rm;
	bool quit;
	CL_String name;
	CL_Font playerName;
	CL_Font gameOverFont;
	CL_FontDescription playerName_desc;
	CL_FontDescription gameOver_desc;
	size_t score;
	PlayerClass playerClass;
	


public:

	World(CL_DisplayWindow &window, CL_ResourceManager &resources) : window(window), keyboard(window.get_ic().get_keyboard()), mouse(window.get_ic().get_mouse()), rm(resources), playerClass(LUMBERJACK)
	{}
	
	// The Game. You just lost it.
	void run()
	{
		
		quit = false;

		CL_Slot slot_quit = window.sig_window_close().connect(this, &World::on_window_close);	
		
		//Connect a slot to read from the keyboard

		
		
		CL_GraphicContext gc = window.get_gc();

		Map map("map.txt", 50, 50, rm, gc);

		Crosshair crosshair(rm, gc);

		//Initialize font to write with
		playerName_desc = CL_FontDescription();
		playerName_desc.set_typeface_name("Microsoft Sans Serif");
		playerName_desc.set_height(40);
		playerName_desc.set_weight(400);
		playerName = CL_Font(gc, playerName_desc);
		
		gameOver_desc = CL_FontDescription();
		gameOver_desc.set_height(32);
		gameOver_desc.set_weight(400);
		gameOverFont = CL_Font(gc, gameOver_desc);

		//Choose class
		
		CL_Slot slot_choose_class = keyboard.sig_key_down().connect(this, &World::choose_class);
		while(!quit)
		{
			CL_Draw::fill(gc, 0, 0, 800, 600, CL_Colorf::black); //TODO delete som of these ugly fills
			CL_Draw::fill(gc, 20, 20, 780, 580, CL_Colorf::blue);
			CL_Draw::fill(gc, 40, 40, 760, 560, CL_Colorf::yellow);
			CL_Draw::fill(gc, 60, 60, 740, 540, CL_Colorf::green);
			CL_Draw::fill(gc, 80, 80, 720, 520, CL_Colorf::purple);
			CL_Draw::fill(gc, 100, 100, 700, 500, CL_Colorf::maroon);
			gameOverFont.draw_text(gc, 150, 150,   "Choose class:", CL_Colorf::black);
			gameOverFont.draw_text(gc, 150, 250,   "1. Lumberjack", CL_Colorf::black);
			gameOverFont.draw_text(gc, 150, 320,   "2. GI", CL_Colorf::black);
			gameOverFont.draw_text(gc, 150, 450, "Press 1 or 2 to choose", CL_Colorf::black);
			
			window.flip();
			CL_KeepAlive::process();
			CL_System::sleep(200);
		}
		slot_choose_class.disable();
		
		
		quit = false;
		
		Player player(rm, gc, &crosshair, playerClass);

		HUD hud(gc, &player);

		// Initializing singleton classes
		BulletManager::instance().init(gc, rm);
		ZombieManager::instance().init(&player, rm);
		
		ItemManager::instance().init(&player, rm, gc);

		size_t bTime = CL_System::get_time();
		
		//Music
		CL_SoundBuffer gameMusic("Game/gameMusic", &rm);
   		CL_SoundBuffer_Session music = gameMusic.prepare();
    	gameMusic.set_volume(0.2f);
    	music = gameMusic.play(true); //Sets the gameMusic to play in a loop
		
		
		std::stringstream sstream;
		//std::stringstream ;
		CL_String score;
		CL_String levelPrint;
		
		while (!quit)
		{
			size_t levelBegin = player.getLevel();
			
		   // Calculate dt
			size_t eTime = CL_System::get_time();
			double dt = (eTime - bTime)/100.0;
			bTime = eTime;
			// Update Player
			player.move(keyboard.get_keycode(CL_KEY_W), keyboard.get_keycode(CL_KEY_A), keyboard.get_keycode(CL_KEY_S), keyboard.get_keycode(CL_KEY_D), dt, &map);
			player.shoot(mouse.get_keycode(CL_MOUSE_LEFT), dt);
			player.changeWeapon(keyboard.get_keycode(CL_KEY_1), keyboard.get_keycode(CL_KEY_2), keyboard.get_keycode(CL_KEY_3));
			player.reload(keyboard.get_keycode(CL_KEY_R), dt);

			// Pick up items
			ItemManager::instance().pickupItems(player);

			// Update crosshair
			crosshair.move(mouse.get_x(), mouse.get_y());

			// Update bullets
			BulletManager::instance().moveBullets(dt, &map);
			BulletManager::instance().updateBombs(dt, &map);

			// Update zombies
			ZombieManager::instance().moveZombies(dt, &player, &map);
			ZombieManager::instance().spawnZombies(dt, rm, gc);

			// Draw Shit
			gc.clear();
			CL_Pointf playercoords(player.x, player.y);

			map.drawMap(gc, playercoords);
			ItemManager::instance().drawItems(gc, playercoords);
			BulletManager::instance().drawBullets(gc, playercoords);
			player.draw(gc, &crosshair);
			ZombieManager::instance().drawZombies(gc, player);
			crosshair.draw(gc);
			hud.draw();

			//Calculate score
			sstream << player.getXp();
			CL_String tempScore(sstream.str());
			sstream.str("");
			score = tempScore;
			gameOverFont.draw_text(gc, 20, 30, score, CL_Colorf::yellow);
			
	
			size_t levelEnd = player.getLevel();
			//Starts the level up screen if player gained level
			if(levelEnd > levelBegin)
			{
				
				
				
				//CL_String for printing level
				sstream << player.getLevel();
				CL_String lvl(sstream.str());
				sstream.str("");
				
				
				
				while(!quit)
				{
					//CL_String for printing skillpoints
					sstream << player.getSkillpoints();
					CL_String sp(sstream.str());
					sstream.str("");

					eTime = CL_System::get_time();
					dt = (eTime - bTime)/100.0;
					bTime = eTime;
				
					CL_Draw::fill(gc, 150, 100, 650, 480, CL_Colorf::maroon);
				
					gameOverFont.draw_text(gc, 160, 130, "Level up!", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 170, "You are now level", CL_Colorf::black);
					gameOverFont.draw_text(gc, 420, 170, lvl, CL_Colorf::yellow);
					gameOverFont.draw_text(gc, 160, 200, "Skillpoints:", CL_Colorf::black);
					gameOverFont.draw_text(gc, 320, 200, sp, CL_Colorf::yellow);
					gameOverFont.draw_text(gc, 160, 240, "Choose skill to increase:", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 270, "1) Damage     (cost 1 skillpoint)", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 300, "2) Hitpoints    (cost 1 skillpoint)", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 330, "3) Speed        (cost 2 skillpoints)", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 400, "Press number to choose.", CL_Colorf::black);
					gameOverFont.draw_text(gc, 160, 430, "Press C to continue", CL_Colorf::black);
					
					
					//Damage
					if(keyboard.get_keycode(CL_KEY_1))
					{	
						if(player.getSkillpoints() < 1)
						{	
							gameOverFont.draw_text(gc, 160, 360, "Not enough skillpoints!", CL_Colorf::black);
							window.flip();
							CL_System::sleep(1000);	
						}	
						
						else	
						{
							player.removeSkillpoints(1);
							player.getCurrentWeapon()->addDamage(3);
							window.flip();
							CL_System::sleep(400);	
						}
					}
					
					//HP
					if(keyboard.get_keycode(CL_KEY_2))
					{
						if(player.getSkillpoints() < 1)
						{	
								gameOverFont.draw_text(gc, 160, 360, "Not enough skillpoints!", CL_Colorf::black);
								window.flip();
								CL_System::sleep(1000);	
						}	
						
						else	
						{
							player.removeSkillpoints(1);
							player.addMaxHp(10);
							window.flip();
							CL_System::sleep(400);
						}
						
					}
					
					//Speed
					if(keyboard.get_keycode(CL_KEY_3))
					{
						if(player.getSkillpoints() < 2)
						{	
								gameOverFont.draw_text(gc, 160, 360, "Not enough skillpoints!", CL_Colorf::black);
								window.flip();
								CL_System::sleep(1000);	
						}	
						
						else	
						{
							player.removeSkillpoints(2);
							player.addSpeed(1);
							window.flip();
							CL_System::sleep(400);
						}
						
					}
					
					if(keyboard.get_keycode(CL_KEY_C))
					{
						quit = true;
					}			
					
					window.flip();

					CL_KeepAlive::process();
					CL_System::sleep(10);		
				
				}
				
				quit = false;
			}

			// Check if ESC is pressed
			if(keyboard.get_keycode(CL_KEY_ESCAPE) == true)
				quit = true;
			// Check if player is dead
			if(player.isDead())	
				quit = true;			
			
			window.flip();

			CL_KeepAlive::process();
			CL_System::sleep(10);
			
			
		}
		music.stop();
		
		
		quit = false;
		
		//Game over sound effect
		CL_SoundBuffer gameOver("Game/gameOver", &rm);
		gameOver.prepare();
		gameOver.play();

		//Start taking the freaking highscore stuff
		
	
		//Open highscore list
		std::fstream highscore;
		highscore.open("highscore.txt");
		
		
		//Start taking input from user and draw shit on the window
		CL_Slot slot_input_down = keyboard.sig_key_down().connect(this, &World::input_down);
		
		while(!quit)
		{
			//DONT TOUCH THIS FFS
			CL_Draw::fill(gc, 100, 100, 700, 500, CL_Colorf::maroon);
			gameOverFont.draw_text(gc, 150, 150,   "                    Game over!\n       The zombies ate your brains", CL_Colorf::black);
			gameOverFont.draw_text(gc, 150, 250,   "                Your score:", CL_Colorf::black);
			gameOverFont.draw_text(gc, 460, 250, score, CL_Colorf::yellow);
			gameOverFont.draw_text(gc, 150, 320,   "                New highscore!\n                   Enter name:", CL_Colorf::black);
			playerName.draw_text(gc,   350, 400, name, CL_Colorf::yellow);
			gameOverFont.draw_text(gc, 150, 450, "            Press Enter to submit", CL_Colorf::black);
			
			
			CL_System::sleep(50);
			
			window.flip();
			CL_KeepAlive::process();
		}
		struct Highscore final;
		final.score = player.getXp();
		final.name = name.c_str();
		//highscore << "1. " << name.c_str() << "    " << score.c_str() << std::endl;
		std::list<Highscore> results = getResults(highscore, final);
		writeScores(highscore, results);
		std::cout << results.size();		
		highscore.close();
	}

	std::list<Highscore> getResults(std::fstream& file, Highscore finalscore)
	{
		std::list<Highscore> scores;
		scores.push_back(finalscore);
		std::string line;
		while (file.good()) // keep reading until end-of-file
		{
			std::string name = "";
			size_t score;
			file >> score;
			file >> name;
			struct Highscore temporary;		
			temporary.name = name;
			temporary.score = score;
			std::cout << score << " " << name << std::endl;
			if(temporary.name != "")
				scores.push_back(temporary);
       		}
		file.close();
		std::ofstream os("highscore.txt");	
		os.close();
		file.open("highscore.txt");		//erases the data in the file and stores everything to a list
		//qsort(&scores, scores.size(), sizeof(Highscore), int_cmp);
		//sort(scores.begin(), scores.end(), myfunction);
		//file.clear();
		return scores;

	}

	void writeScores(std::fstream&file, std::list<Highscore> results)	
	{
		for(std::list<Highscore>::iterator iter = results.begin(); iter != results.end(); iter++)	
		{	
			//std::cout << iter->name << " " << iter->score << std::endl;
			file << iter->score << " " << iter->name << std::endl;		//writes score and name to data
		}
	}

	void printResults(std::list<Highscore> results)	
	{
		size_t rank = 1;
		for(std::list<Highscore>::iterator iter = results.begin(); iter != results.end(); iter++)	// prints the result to screen
		{
			std::cout << rank << ". " << iter->name << " " << iter->score;
			rank++; 
		}
	}

	bool myfunction(Highscore const& lhs, Highscore const& rhs) 
	{ 
		if (lhs.score != rhs.score)		
			return (lhs.score < rhs.score); // for std::sort(first iterator, last iterator, comparatorfunction)
		return lhs.score; 
	} 

	bool int_cmp(const void *a, const void *b) 
	{ 
	    const size_t *ia = (const size_t *)a; // casting pointer types 
	    const size_t *ib = (const size_t *)b; // for qsort(*container, sizeofcontainer, sizeofelement, comparatorfunction)
	    return *ia  - *ib; 
		/* integer comparison: returns negative if b > a 
		and positive if a > b */ 
	} 

	void choose_class(const CL_InputEvent &key, const CL_InputState &state)
	{
		if(key.id == CL_KEY_1)
		{	
			playerClass = LUMBERJACK;
			quit = true;
		}
		
		if(key.id == CL_KEY_2)
		{
			playerClass = GI;
			quit = true;
		}
	}
		
	void input_down(const CL_InputEvent &key, const CL_InputState &state)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
			
		if(key.id == CL_KEY_ENTER)
			quit = true;
		else
		{	
			if(key.id == 65288) //If backspace is pressed
			{
				if(!name.empty())
					name.erase(name.end()-1); //erase last letter
			}
			else if(name.length() < 10)
				name.append(key.str); //add a letter
		}
	}
	
	void on_window_close()
	{
		quit = true;
	}
};


/*class Menu
{
public:
	quit = false;

		CL_Slot slot_quit = window.sig_window_close().connect(this, &World::on_window_close);

		CL_GraphicContext gc = window.get_gc();		
		
		Map map("map.txt", 50, 50, rm, gc);

		//
		
		
		inputbox = CL_InputBox(player.getHp(),CL_Component* parent, CL_StyleManager* style = NULL);
		

		
		size_t bTime = CL_System::get_time();

		while (!quit)
		{

		   // Calculate dt
			size_t eTime = CL_System::get_time();
			double dt = (eTime - bTime)/100.0;
			bTime = eTime;

			// Draw map

		


			// Check if ESC is pressed
			if(keyboard.get_keycode(CL_KEY_ESCAPE) == true)
				quit = true;			
			
			window.flip();

			CL_KeepAlive::process();
		}
	}


	void on_window_close()
	{
		quit = true;
	}

};*/

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_ResourceManager resources("resources.xml");
		CL_DisplayWindow window("Hanna vs. Zombies", WIDTH, HEIGHT);
		
	    CL_SetupSound setup_sound;
	    CL_SetupVorbis setup_vorbis;

    	CL_SoundOutput sound_output(44100);
    
		/*CL_SpriteDescription desc_simple;
		desc_simple.add_frame(CL_ImageProviderFactory::load("background.jpg")); */
		

		try
		{
			World world(window, resources);
			world.run();
		}
		catch(CL_Exception &exception)
		{
			// Create a console window for text-output if not available
			CL_ConsoleWindow console("Console", 80, 160);
			CL_Console::write_line("Error: " + exception.get_message_and_stack_trace());

			console.display_close_message();

			return -1;
		}
		return 0;
	}
};
CL_ClanApplication app(&Program::main);
