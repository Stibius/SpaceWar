
/** \file Game.h
*   \brief Obsahuje deklaraci tridy Game.
*/

#ifndef GAME_H_
#define GAME_H_

#include "Collision_System.h"
#include "Game_Interface.h"
#include "Menu.h"
#include "Player.h"
#include "Powerup_System.h"
#include "Projectile.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "Star.h"

/** \class Game
*   \brief Hlavni trida programu. Pracuje se vsemi ostatnimi tridami, vytvari herni okno a obsahuje hlavni smycku programu.  
*/
class Game : public Drawable, public Configurable, public Updatable
{
private:


	enum states //!<mozne stavy hry
	{
		IN_GAME,     //!<jsme ve hre
		GAME_PAUSED, //!<jsme v menu, prave probihajici hra je zapauzovana
		GAME_OFF     //!<jsme v menu, hra neni spustena 
	};

	static const GLfloat STAR_RADIUS;                 //!<polomer hvezdy
	static const int NUM_RESOLUTIONS = 29;              //!<pocet vsech hrou podporovanych rozliseni
	static const SDL_Rect RESOLUTIONS[NUM_RESOLUTIONS]; //!<seznam vsech hrou podporovanych rozliseni

	SDL_Surface* g_screen; //!<surface obrazovky
	GLfloat fps;           //!<aktualni pocet snimku za sekundu
	
	states state;          //!<aktualni stav hry  

	GLuint grid_list;    //!<id display-listu s mrizkou na pozadi
	Color grid_color;    //!<barva mrizky na pozadi
	bool show_grid;      //!<udava, zda se ma zobrazovat mrizka na pozadi
	GLuint stars_list;   //!<id display-listu s hvezdami na pozadi
	int num_stars;       //!<pocet hvezd na pozadi
	
	Menu menu;           
	Player players[Settings::NUM_PLAYERS]; 
	Powerup_System powerup_system;         
	Collision_System collision_system;     
	vector<Projectile*> projectiles;       //!<vektor ukazatelu na projektily, ktere se prave nachazi na herni plose
	Settings settings;   
	Star star;           
	Game_Interface game_interface;           

	/** \brief Vytvori herni okno s definovanymi vlastnostmi. Pokud uz okno existuje, vytvori se misto nej nove, pokud nema stejne parametry.
	*   \param resolution Pozadovane rozliseni ve slozkach w a h.
	*   \param Barevna hloubka v bitech.
	*   \return true, pokud se podarilo vytvorit okno, jinak false.
    */
	bool Set_Video_Mode(const SDL_Rect & resolution, int bpp, bool fullscreen);

	//! \brief Nastavi parametry OpenGL.
	void Init_GL();

	/** \brief Nastavi perspektivu OpenGL na zaklade rozliseni.
	*   \param resolution Pozadovane rozliseni ve slozkach w a h.
	*/
	void Resize_GL(const SDL_Rect & resolution) const;

	//! \brief Vypocita aktualni pocet snimku za sekundu, pokud se zavola pri kazdem prekresleni obrazovky.
	void Calculate_FPS();

	//! \brief Inicializuje herni objekty a vykresli display-listy mrizky a hvezd.
	void Init_Game();

	//! \brief Vymaze display-listy mrizky a hvezd a smaze existujici projektily.
	void Cancel_Game();

	/** \brief Zpracovava SDL udalosti cekajici ve fronte.
	*   \return false pokud byla obdrzena zprava SDL_QUIT a program se ma ukoncit, jinak true.
    */
	bool Process_Events();

	//! \brief Spusti hlavni nekonecnou smycku aplikace.
	void Main_Loop();

	/** \brief Aktualizuje stav hry, vola se pred kazdym prekreslenim obrazovky.
	*   \param fps Aktualni pocet snimku za sekundu.
    */
	virtual void Update(GLfloat fps);

	//! \brief Vykresleni mrizky, ktera se ma zobrazovat na pozadi, do display listu a ulozeni jeho id do prislusne tridni promenne.
	void Draw_Grid();

	//! \brief Vykresleni daneho poctu hvezd, ktere se maji zobrazovat na pozadi, do display listu a ulozeni jeho id do prislusne tridni promenne.
	void Draw_Stars(int num_stars);

	//! \brief Vykresleni herni plochy.
	void Draw_Game() const;
	
	//! \brief Vykresleni vseho potrebneho na obrazovku (herni plocha a/nebo menu podle aktualniho stavu aplikace).
	virtual void Draw() const;

	//! \brief Podle aktualnich hodnot v objektu settings aktualizuje parametry ostatnich objektu.
	virtual void Set_Settings(const Settings & settings);
	
public:

	//! \brief Deinicializuje SDL.
	~Game();

	//! \brief Inicializuje OpenGL a SDL, vytvori okno a spusti hlavni smycku programu.
	void Run();
};

#endif