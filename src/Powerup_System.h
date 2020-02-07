
/** \file Powerup_System.h
*   \brief Obsahuje deklaraci tridy Powerup_System.
*/

#ifndef POWERUP_H_
#define POWERUP_H_

class Collision_System;

#include "Circle.h"
#include "Player.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "Texture.h"
#include "Font.h"
#include "Sound.h"
#include "Point.h"
#include "Drawable.h"

/** \class Powerup_System
*   \brief Trida reprezentujici system powerupu.
*/
class Powerup_System : public Drawable, public Updatable, public Configurable
{

public:

	//! \brief druhy powerupu
    enum powerups_t 
	{ 
		FIRST_POWERUP, //!<prvni bonus
		HEALTH_POWERUP = FIRST_POWERUP, //!<doplneni zivota
		SHIELD_POWERUP, //!<doplneni stitu
		ENERGY_POWERUP, //!<doplneni energie
		TRIPLE_POWERUP, //!<trojite strely
		GUIDED_POWERUP, //!<navadene strely
		LASER_POWERUP,  //!<laserove strely
		ROCKET_POWERUP, //!<rakety
		LASER_BEAM_POWERUP, //!<laserovy paprsek
		NUM_POWERUPS    //!<pocet druhu powerupu
	};

	/** \class Powerup
    *   \brief Abstraktni trida reprezentujici bonus, odvozuji se od ni tridy pro vsechny druhy powerupu.
    */
	class Powerup : public Object
	{
	private:
		Uint32 time_limit; //!<cas, kdy ma bonus zmizet z obrazovky, pokud do te doby nebude sebran
		static int display_time; //!<cas od vygenerovani, po ktery je bonus zobrazen, pokud neni drive sebran, v sekundach
		static Sound pickup_sound; //!<zvuk sebrani powerupu
	protected:
		//! \brief Prehraje zvuk sebrani powerupu.
		void Play_Pickup_Sound() const;
	public:
		static const GLfloat RADIUS; //!<polomer kruznic ohranicujicich powerupy 

		//! \brief Nastavi cas zmizeni bonusu a polomer.
		Powerup(const Point & position = Point());

		//! \brief Virtualni destruktor, nic nedela.
		virtual ~Powerup() {};

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const = 0;

		//! \brief Vrati kruznici ohranicujici bonus.
	    const Circle Get_Circle() const;

		//! \brief Nastavi cas od vygenerovani, po ktery je bonus zobrazen, pokud neni drive sebran, v sekundach.
		static void Set_Display_Time(int display_t) { display_time = display_t; }

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player) = 0;

		//! \brief Zjisti, jestli uz uplynul cas od vygenerovani, po ktery ma byt bonus zobrazen
		virtual bool Time_Out();

		//! Vykresleni.
	    virtual void Draw() const = 0;
	};

	/** \class Health_Powerup
    *   \brief Trida reprezentujici bonus doplneni zivotu.
    */
	class Health_Powerup : public Powerup
	{
	private:
		static int bonus_value; //!<mnozstvi pridaneho zivota
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Health_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return HEALTH_POWERUP; }

		//!<nastavi mnozstvi pridaneho zivota
		static void Set_Bonus_Value(int bonus_val) { bonus_value = bonus_val; } 

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Shield_Powerup
    *   \brief Trida reprezentujici bonus doplneni stitu.
    */
	class Shield_Powerup : public Powerup
	{
	private:
		static int bonus_value; //!<mnozstvi pridaneho stitu
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Shield_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return SHIELD_POWERUP; }

		//!<nastavi mnozstvi pridaneho stitu
		static void Set_Bonus_Value(int bonus_val) { bonus_value = bonus_val; } 

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Energy_Powerup
    *   \brief Trida reprezentujici bonus doplneni energie.
    */
	class Energy_Powerup : public Powerup
	{
	private:
		static int bonus_value; //!<mnozstvi pridane energie
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Energy_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return ENERGY_POWERUP; }

		//!<nastavi mnozstvi pridane energie
		static void Set_Bonus_Value(int bonus_val) { bonus_value = bonus_val; } 

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Triple_Powerup
    *   \brief Trida reprezentujici bonus trojite strely.
    */
	class Triple_Powerup : public Powerup
	{
	private:
		static int effect_duration; //!<doba trvani efektu powerupu v sekundach
		static GLuint list;         //!<identifikator display_listu s vykreslenym vnitrkem powerupu
		static Texture text;        //!<textura s vypsanym textem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Triple_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return TRIPLE_POWERUP; }

		//! \brief Vrati dobu trvani efektu powerupu v sekundach
		static int Get_Effect_Duration() { return effect_duration; }

		//! \brief nastavi dobu trvani efektu powerupu v sekundach
		static void Set_Effect_Duration(int effect_dur) { effect_duration = effect_dur; } 

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Guided_Powerup
    *   \brief Trida reprezentujici bonus navadeni strel.
    */
	class Guided_Powerup : public Powerup
	{
	private:
		static int effect_duration; //!<doba trvani efektu powerupu v sekundach
		static GLuint list;         //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Guided_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return GUIDED_POWERUP; }

		//! \brief Vrati dobu trvani efektu powerupu v sekundach
		static int Get_Effect_Duration() { return effect_duration; }

		//! \brief nastavi dobu trvani efektu powerupu v sekundach
		static void Set_Effect_Duration(int effect_dur) { effect_duration = effect_dur; } 

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Rocket_Powerup
    *   \brief Trida reprezentujici bonus rakety.
    */
	class Rocket_Powerup : public Powerup
	{
	private:
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Rocket_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return ROCKET_POWERUP; }

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Laser_Powerup
    *   \brief Trida reprezentujici bonus laserove strely.
    */
	class Laser_Powerup : public Powerup
	{
	private:
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Laser_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return LASER_POWERUP; }

		//! \brief Aplikuje bonus na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	/** \class Laser_Beam
    *   \brief Trida reprezentujici bonus laserovy paprsek.
    */
	class Laser_Beam_Powerup : public Powerup
	{
	private:
		static GLuint list;     //!<identifikator display_listu s vykreslenym vnitrkem powerupu
	public:
		static const Color COLOR; //!<barva tohoto druhu bonusu

		Laser_Beam_Powerup(const Point & position = Point()) : Powerup(position) {}

		//! \brief Zjisti typ bonusu.
		virtual powerups_t Get_Type() const { return LASER_BEAM_POWERUP; }

		//! \brief Aplikuje powerup na daneho hrace.
		virtual void Apply(Player & player);

		//! \brief Vykresleni display-listu s powerupem.
		virtual void Draw() const;

		//! \brief Vytvoreni display-listu.
		static void Draw_List();

		//! \brief Smazani display-listu s powerupem.
		static void Delete_List() { glDeleteLists(list, 1); }

	};

	//! \brief Defaultni konstruktor, jen inkrementuje pocet instanci.
	Powerup_System() : player1(NULL), player2(NULL), collision_system(NULL) { num_instances++; }

	/** \brief Inicializuje objekt, vykresli display-listy powerupu, pokud jeste nejsou vykreslene, inkrementuje pocet instanci.
	*   \param settings Herni nastaveni, podle kterych se nastavi nektere parametry systemu.
	*   \param collision_system Objekt si uchova jeho adresu kvuli umistovani nove vygenerovanych powerupu.
	*   \param player1 Objekt si uchova jeho adresu kvuli urcovani uzitecnosti potencialne generovanych powerupu.
	*   \param player2 Objekt si uchova jeho adresu kvuli urcovani uzitecnosti potencialne generovanych powerupu.
	*/
	Powerup_System(const Settings & settings, const Collision_System & collision_system, const Player & player1, const Player & player2);

	//! \brief Inkrementuje pocet instanci.
	Powerup_System(const Powerup_System & powerup_system);

	//!  \brief Dekrementuje pocet instanci, pokud je 0, smaze display-listy powerupu, pokud existuji.
	~Powerup_System();

	//! \brief Vykresli vsechny aktualne vytvorene powerupy na herni plochu.
	virtual void Draw() const;

	//! \brief Zkontroluje, zda je cas vytvorit novy powerup nebo zrusit existujici, a udela to.
	virtual void Update(GLfloat fps);

	//! \brief Nastavi parametry objektu podle predanych nastaveni.
	virtual void Set_Settings(const Settings & settings);

private:

	friend class Collision_System; //!<aby mohla trida Collision_System pristupovat primo k jednoltivym powerupum

	static int num_instances; //!<aktualni pocet instanci tridy
	static bool initialized;  //!<zda jsou vytvorene display-listy powerupu
	int min_interval;         //!<minimalni interval mezi generovanim powerupu v milisekundach
	int max_interval;         //!<maximalni interval mezi generovanim powerupu v milisekundach
	Uint32 next_powerup_time; //!<cas, kdy se ma vygenerovat dalsi bonus
	SDL_Rect area;            //!<oblast na obrazovce, kde se muze vygenerovat bonus, w a h rozmery, x a y pozice leveho horniho rohu

	const Player* player1; //!< slouzi na urcovani uzitecnosti potencialne generovanych powerupu
	const Player* player2; //!< slouzi na urcovani uzitecnosti potencialne generovanych powerupu
	
	bool allowed[NUM_POWERUPS]; //!<udava u kazdeho druhu powerupu, jestli je ho povoleno generovat


	vector<Powerup*> powerups; //!<powerupy, ktere se prave nachazi na herni plose
	const Collision_System* collision_system; //!<slouzi na umistovani nove vygenerovanych powerupu.

	//! \brief Zjisti, zda je aspon jeden druh powerupu povoleny.
	bool Some_Allowed() const;

	//! \brief Zjisti, zda je aspon jeden druh powerupu povoleny a nektery hrac ho muze aktualne vyuzit.
	bool Some_Useful();

	//! \brief Vygeneruje novy bonus.
	void Put_Powerup();

	//! \brief Zjisti, zda nektery z hracu muze aktualne vyuzit dany typ powerupu.
	bool Is_Useful(powerups_t type);

	//! \brief Zjisti, zda dany hrac muze aktualne vyuzit dany typ powerupu.
	static bool Is_Useful(powerups_t type, const Player & player);

};


#endif