
/** \file Game.cpp
*   \brief Obsahuje definice metod a konstant tridy Engine.
*/

#include "Events.h"
#include <ctime>
#include "Game.h"
#include "Moving_Object.h"
#include "Point.h"
#include "SDL_syswm.h"

const GLfloat Game::STAR_RADIUS = 50.0f;

void Game::Draw_Stars(int num_stars)
{
	vector<Point> stars; //pozice jednotlivych hvezd
	Point star_pos;      //nahodne generovana pozice hvezdy

	//naplneni vektoru pozic hvezd nahodnymi hodnotami v mezich aktualnich rozmeru obrazovky
	for (int i = 0; i < num_stars; i++)
	{
		SDL_Rect resolution = settings.Get_Resolution();

		star_pos.x = GLfloat(rand() % resolution.w);
		star_pos.y = GLfloat(rand() % resolution.h);
	
		//kontrola, aby nebyla dvakrat pouzita stejna pozice
		for (int j = 0; j < i; j++)
		{
			if (stars[j] == star_pos)
			{
				i--;
				break;
			}
		}

		stars.push_back(star_pos);
	}

	//vykresleni do display-listu
	int i = 1;
	while (glIsList(i))
		i++;
	stars_list = i;

	glNewList(stars_list, GL_COMPILE); 
	    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPointSize(2.0f);
	
	    glBegin(GL_POINTS);
	    for (int i = 0; i < num_stars; i++)
		{
			glVertex2f(stars[i].x, stars[i].y);
		} 
		glEnd();
	glEndList();
}

void Game::Draw_Grid() 
{
	int i = 1;
	while (glIsList(i))
		i++;
	grid_list = i;

	SDL_Rect resolution = settings.Get_Resolution();

	glNewList(grid_list, GL_COMPILE); 
	    GLint width = resolution.w;
	    GLint height = resolution.h;
		glLineWidth(1.0f);
		glPushMatrix();
		glLoadIdentity();

	    GLint x = 1; //pocatecni x-ova pozice pro kresleni vertikalnich car
		i = 0;       //pocitadlo car

	    do 
		{
			if (!(i % 4)) //kazda ctvrta cara je vyraznejsi
			    glDisable(GL_BLEND);
			else
				glEnable(GL_BLEND);

				glBegin(GL_LINES);
			        glVertex2i(x, -1);
			        glVertex2i(x, height);
                glEnd();

			    i++;
			    x += width / 60; //posun v x na dalsi caru
		} while (x <= width);

		GLint y = 1; //pocatecni y-ova pozice pro kresleni horizontalnich car
		i = 0;       //pocitadlo car

	    do {
			if (!(i % 4)) //kazda ctvrta cara je vyraznejsi
			    glDisable(GL_BLEND);
			else
				glEnable(GL_BLEND);

			glBegin(GL_LINES);
			    glVertex2i(-1, y);
			    glVertex2i(width, y);
            glEnd();

			i++;
			y += width / 60; //posun v y na dalsi caru
		} while (y <= height);

		glEnable(GL_BLEND);
		glPopMatrix();
	glEndList();
}

void Game::Calculate_FPS()
{
	static Uint32 last_time = SDL_GetTicks(); //doba, kdy se naposledy zjistovalo fps

	Uint32 miliseconds = SDL_GetTicks() - last_time; //doba, ktera uplynula od posledniho zjistovani fps

	fps = 1000.0f / miliseconds; //pocet snimku za sekundu
	last_time = SDL_GetTicks();
}

void Game::Init_GL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //specifikuje barvu, kterou se vymaze pozadi okna 
	glEnable(GL_BLEND);               //povoleni blendingu
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //nastaveni blend funkce
	glEnable(GL_LINE_SMOOTH);          //povoleni antialiasingu usecek
	glEnable(GL_POINT_SMOOTH);         //povoleni antialiasingu usecek
	glEnable(GL_POLYGON_SMOOTH);       //povoleni antialiasingu usecek
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
}

void Game::Draw() const
{
	glClear(GL_COLOR_BUFFER_BIT);     //vymazani obrazovky

	if (state == IN_GAME || state == GAME_PAUSED) //probiha hra
	{
		Draw_Game();
	}

	if (state == GAME_PAUSED) //hra je zapauzovana, vykresli se pres ni cerna pruhledna barva
	{
		SDL_Rect resolution = settings.Get_Resolution();

		glColor4f(0.0f, 0.0f, 0.0f, 0.6f);

		glBegin(GL_QUADS);
		    glVertex2f(0.0f, 0.0f);
			glVertex2f(resolution.w, 0.0f);
			glVertex2f(resolution.w, resolution.h);
			glVertex2f(0.0f, resolution.h);
		glEnd();
	}

	if (state != IN_GAME) //jsme v hernim menu
		menu.Draw();
                       
	SDL_GL_SwapBuffers();
}

void Game::Draw_Game() const
{
	glLoadIdentity();
	glCallList(stars_list);

	if (show_grid)
	{
	    glColor4f(grid_color.red, grid_color.green, grid_color.blue, 0.5f);
	    glCallList(grid_list);
	}
	
	powerup_system.Draw();

	for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	{
		players[i].Draw();
	}

	for (vector<Projectile*>::const_iterator it = projectiles.begin(); it < projectiles.end(); it++)
	{
		(*it)->Draw();
	}

	star.Draw();

	game_interface.Draw();
	game_interface.Draw_FPS(fps);

	glLoadIdentity();
}

Game::~Game()
{
	SDL_Quit();
}

void Game::Run()
{
	state = GAME_OFF;

	g_screen = NULL;

	srand(unsigned int(time(0)));

	if (SDL_Init(SDL_INIT_VIDEO)) //Inicializace SDL
	{
		return;
	}

	//Atributy rendering kontextu.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	//podporovana rozliseni
	SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

	//zadna rozliseni nejsou podporovana
    if (modes == (SDL_Rect**)0)
		return;

	//nastavi podporovana rozliseni
	settings.Set_Resolution_Range(modes);
	
	//zjisti aktualni rozliseni
	SDL_Rect resolution = settings.Get_Resolution();
	
	//nastavi okno podle aktualniho rozliseni
	if (Set_Video_Mode(resolution, 32, settings.Get_Setting_Value(Settings::FULLSCREEN)) == false)
	{
		return;
	}
	
	SDL_WM_SetCaption("Space War", NULL); //Titulek okna

	SDL_ShowCursor(SDL_ENABLE);           //povoli kurzor
	SDL_EnableKeyRepeat(200, 200);        //pri drzeni klavesy v menu se posle znovu zprava za 200ms

	menu = Menu(settings); //inicializace menu
	collision_system.Set_Margins(resolution); //nastaveni okraju obrazovky koliznimu systemu

	grid_color = settings.Get_Setting_Value(Settings::GRID_COLOR);
	show_grid = settings.Get_Setting_Value(Settings::GRID);
	
	Init_GL(); //inicializace parametru OpenGL

	Main_Loop(); //hlavni smycka aplikace
	
	return;
}

bool Game::Set_Video_Mode(const SDL_Rect & resolution, int bpp, bool fullscreen)
{
	//http://www.bytehazard.com/code/sdlres.html

	Uint32 flags;
	if (fullscreen)
		flags = SDL_FULLSCREEN | SDL_OPENGL;
	else
		flags = SDL_OPENGL;

	if (!SDL_VideoModeOK(resolution.w, resolution.h, bpp, flags)) 
		return false; //dany mod neni podporovan

	if (g_screen) //je vytvorene okno
	{
		//Get the current video hardware information
        const SDL_VideoInfo* original_settings = SDL_GetVideoInfo();
		bool original_fullscreen = (g_screen->flags & SDL_FULLSCREEN) != 0;
	    if (resolution.w == original_settings->current_w && resolution.h == original_settings->current_h && original_settings->vfmt->BitsPerPixel == bpp && ((fullscreen && original_fullscreen) || (!fullscreen && !original_fullscreen)))
	    {
		    return true; //no change
	    }

		#ifdef _WIN32 //jsme ve Windows
	        SDL_SysWMinfo info;
 
            //get window handle from SDL
            SDL_VERSION(&info.version);
            if (SDL_GetWMInfo(&info) == -1) return false;
 
            //get device context handle
            HDC tempDC = GetDC( info.window );
 
            //create temporary context
            HGLRC tempRC = wglCreateContext( tempDC );
            if (tempRC == NULL) return false;
 
            //share resources to temporary context
            SetLastError(0);
            if (!wglShareLists(info.hglrc, tempRC)) return false;
        #endif

		//Vytvori okno s definovanymi vlastnostmi
	    g_screen = SDL_SetVideoMode(resolution.w, resolution.h, bpp, flags);

		#ifdef _WIN32 //jsme ve Windows

	        //previously used structure may possibly be invalid, to be sure we get it again
            SDL_VERSION(&info.version);
            if (SDL_GetWMInfo(&info) == -1) return false;
 
            //share resources to new SDL-created context
            if (!wglShareLists(tempRC, info.hglrc)) return false;
 
            //we no longer need our temporary context
            if (!wglDeleteContext(tempRC)) return false;

        #endif

		Init_GL();
	    Resize_GL(resolution); //Nastavi perspektivu
	    menu.Set_Resolution(resolution);
   
	}
	else //neni vytvorene okno
	{
		//Vytvori okno s definovanymi vlastnostmi
	    g_screen = SDL_SetVideoMode(resolution.w, resolution.h, bpp, flags);
		Init_GL();
	    Resize_GL(resolution); //Nastavi perspektivu
	    menu.Set_Resolution(resolution);
	}
	
	//this->resolution = resolution;

	//Init_GL();
	//Resize_GL(resolution); //Nastavi perspektivu

	return true;
}

void Game::Init_Game()
{
	num_stars = settings.Get_Setting_Value(Settings::NUM_STARS);
	Draw_Stars(num_stars);
	Draw_Grid();
	show_grid = settings.Get_Setting_Value(Settings::GRID);
	grid_color = settings.Get_Setting_Value(Settings::GRID_COLOR);

	SDL_Rect resolution = settings.Get_Resolution();

	star = Star(players[Settings::FIRST], players[Settings::SECOND], projectiles, settings, STAR_RADIUS, 3, 3, Point(resolution.w / 2.0f, resolution.h / 2.0f));

	Projectile::Draw_Lists();

	players[Settings::FIRST] = Player(settings, Settings::FIRST, Point(50.0f, 150.0f), 135.0f);
	players[Settings::SECOND] = Player(settings, Settings::SECOND, Point(resolution.w - 50.0f, resolution.h - 50.0f), 315.0f);

	collision_system = Collision_System(settings.Get_Resolution(), players[Settings::FIRST], players[Settings::SECOND], projectiles, powerup_system, star);

	game_interface = Game_Interface(players[Settings::FIRST], players[Settings::SECOND], settings);

	powerup_system = Powerup_System(settings, collision_system, players[Settings::FIRST], players[Settings::SECOND]);
}

void Game::Cancel_Game()
{
	glDeleteLists(grid_list, 1);
	glDeleteLists(stars_list, 1);
	Projectile::Delete_Lists();
	for (vector<Projectile*>::iterator it = projectiles.begin(); it < projectiles.end(); it++)
	{
		delete *it;
	}
	projectiles.clear();
}

void Game::Main_Loop()
{
	bool done = false;

	while (!done)
	{
		done = !Process_Events(); //Osetri udalosti
		
		if (state == IN_GAME)
		{
		    Calculate_FPS();
			Update(fps);
			collision_system.Manage_Collisions();
		}

		Draw(); 		 
	}
}

void Game::Set_Settings(const Settings & settings)
{
	if (state == GAME_PAUSED) //je spustena hra, updatnou se herni objekty
	{
		star.Set_Settings(settings);

	    for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	    {      
		    players[i].Set_Settings(settings);
	    }

		if (num_stars != settings.Get_Setting_Value(Settings::NUM_STARS)) //pocet hvezd se zmenil
		{
			num_stars = settings.Get_Setting_Value(Settings::NUM_STARS);
			Draw_Stars(num_stars);
		}

		grid_color = settings.Get_Setting_Value(Settings::GRID_COLOR);

		show_grid = settings.Get_Setting_Value(Settings::GRID);

		powerup_system.Set_Settings(settings);
		
		for (vector<Projectile*>::iterator it = projectiles.begin(); it < projectiles.end(); it++)
		{
			(*it)->Set_Settings(settings);
		}
	
		game_interface.Set_Settings(settings);
	}

	menu.Set_Settings(settings);
}

bool Game::Process_Events()
{
	SDL_Event event;

	//Smycka na zpracovani udalosti
	while (SDL_PollEvent(&event)) //dokud jsou ve fronte nejake udalosti
	{
		//Vetvi podle typu udalosti
		switch (event.type)
		{
		case SDL_USEREVENT: //uzivatelska udalost
			switch(event.user.code)
            {
			case USR_EVT_RETURN_TO_GAME: //navrat do pozastavene hry z menu
				state = IN_GAME;
				if (settings.Get_Setting_Value(Settings::MOUSE_TURNING_PLAYER) != Settings::NONE) 
					SDL_ShowCursor(SDL_ENABLE); //nektery hrac je otaceny s pomoci mysi, zobrazi se kurzor
				else
					SDL_ShowCursor(SDL_DISABLE);
		        Calculate_FPS(); //aby se nepocitalo s poslednim snimkem z doby pred pozastavenim
				break;
			case USR_EVT_START_GAME: //spusteni nove hry
				state = IN_GAME;
				if (settings.Get_Setting_Value(Settings::MOUSE_TURNING_PLAYER) != Settings::NONE) 
					SDL_ShowCursor(SDL_ENABLE); //nektery hrac je otaceny s pomoci mysi, zobrazi se kurzor
				else
					SDL_ShowCursor(SDL_DISABLE);
				Init_Game();
				Calculate_FPS(); //aby se nepocitalo s poslednim snimkem z doby pred pozastavenim
				break;
			case USR_EVT_QUIT_GAME: //ukonceni pozastavene hry
				state = GAME_OFF;
		        Cancel_Game();
				break;
			case USR_EVT_FIRE: //byl vystrelen novy projektil, zaradi se do vektoru
			    {
					Projectile* p = (Projectile*)event.user.data1;
					if (p->Is_Guided())
					{
						switch (p->Get_Player())
						{
						case Settings::FIRST:
							p->Set_Target(players[Settings::SECOND]);
							break;
						case Settings::SECOND:
							p->Set_Target(players[Settings::FIRST]);
							break;
						default:
							break;
						}
					}
					projectiles.push_back(p);
				}
				break;
			case USR_EVT_SETTINGS_CHANGED: //bylo zmeneno nastaveni
				{
				    Set_Settings(settings);
				}
				break;
			case USR_EVT_VIDEO_SETTINGS_CHANGED: //bylo zmeneno nastaveni obrazovky (rozliseni a/nebo fullscreen)
				{
				    SDL_Rect resolution = settings.Get_Resolution();
				    Set_Video_Mode(resolution, 32, settings.Get_Setting_Value(Settings::FULLSCREEN));
				    menu.Set_Resolution(settings.Get_Resolution());
				    collision_system.Set_Margins(resolution);
				}
				break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN: //byla stisknuta klavesa
			//vetvi podle klavesy
			switch (event.key.keysym.sym)
			    {
				    case SDLK_ESCAPE:
						if (state == IN_GAME) //pozastaveni probihajici hry, skok do menu
					    {
							state = GAME_PAUSED;
							SDL_ShowCursor(SDL_ENABLE);
					    }
					    else //jsme v menu a to bude reagovat na klavesu
					    {
						    menu.Update(SDLK_ESCAPE);
					    }
					    break;
				    default:
						if (state != IN_GAME) //na klavesu bude reagovat menu
					    {
					    	menu.Update(event.key.keysym.sym);
					    }
					    break;
			    }
				break;
		case SDL_MOUSEMOTION: //mys se pohnula
			if (state != IN_GAME) //reaguje menu
			    menu.Update(float(event.motion.y));
            break;
		case SDL_MOUSEBUTTONDOWN: //bylo stisknuto tlacitko mysi
			if (state != IN_GAME) //reaguje menu
            {
				menu.Update(event.button.button);
			}
			break;
		case SDL_QUIT: //ma se ukoncit program
		    return false;
			break;
		default:
			break;
		}
	}

	return true;
}

void Game::Resize_GL(const SDL_Rect & resolution) const
{
	glViewport(0, 0, resolution.w, resolution.h); //viditelna oblast pres cele okno (x, y: levy spodni roh Viewportu, w, h: rozmery Viewportu
    glMatrixMode(GL_PROJECTION);                 
    glLoadIdentity();                                 
    glOrtho(0, resolution.w, resolution.h, 0, 0, 1);                                                  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::Update(GLfloat fps)
{
	SDL_Rect resolution = settings.Get_Resolution();

	star.Update(fps);

	for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	{
		if (players[i].Get_Mode() == Player::NORMAL) //hrac neni prave zniceny nebo explodujici
		{
		    players[i].Update(fps);

		    if (!collision_system.Determine_Doubles(players[i])) //nema dvojniky, mohl by byt uplne mimo obrazovku
		    {
			    players[i].Get_To_Screen(resolution);
		    }
		}
		else
		{
			players[i].Update(fps);
		}
	}

	vector<Projectile*>::iterator it = projectiles.begin();
	while (it < projectiles.end())
	{
		if ((*it)->Get_Mode() == Projectile::NORMAL) //projektil neni prave zniceny nebo explodujici
		{
			(*it)->Update(fps);

	        //pokud projektil opustil obrazovku, zrusi se
		    GLfloat x = (*it)->Get_Center_X();
			GLfloat y = (*it)->Get_Center_Y();
			if (x < 0 || x > resolution.w || y < 0 || y > resolution.h)
		    {
			    delete (*it);
		        it = projectiles.erase(it);
				continue;
		    } 
		}
		else if ((*it)->Get_Mode() == Projectile::DESTROYED) //pokud byl projektil znicen, zrusi se
		{
			delete (*it);
		    it = projectiles.erase(it);
			continue;
		}
		else //projektil prave exploduje
		{
			(*it)->Update(fps);
		}

		it++;
	}	

	powerup_system.Update(fps);
}

















