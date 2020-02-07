
/** \file Projectile.h
*   \brief Obsahuje deklaraci tridy Projectile.
*/

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Moving_Object.h"
#include "Settings.h"
#include "Player.h"
#include "Configurable.h"

/** \class Projectile
*   \brief Trida reprezentujici projektily.
*/
class Projectile : public Moving_Object, public Configurable
{
public:

	//! \brief Mozne stavy projektilu.
    enum states 
	{ 
		NORMAL, //!<normalni stav
		EXPLOSION, //!<projektil prave exploduje
		DESTROYED, //!<exploze projektilu skoncila 
		NUM_STATES //!<pocet moznych stavu projektilu
	};

	/** \brief Vytvori navadeny projektil.
	*   \param settings Nastaveni parametru aplikace, podle ktereho se nastavi parametry projektilu.
	*   \param player Urcuje, ktery hrac tento projektil vystrelil.
	*   \param type Druh projektilu podle zbrane.
	*   \param guided Urcuje, zda ma byt tento projektil navadeny.
	*   \param triple Urcuje, zda byl tento projektil vystrelen ve trojici.
	*   \param shot_identifier Jednoznacne identifikuje vystrel, do ktereho projektil patri.
	*   \param position Startovni pozice projektilu.
	*   \param ang Startovni uhel letu projektilu, ve stupnich.
	*/
	Projectile(const Settings & settings, Settings::players_t player, Settings::weapons_t type, bool guided, bool triple, unsigned int shot_identifier, const Point & position, GLfloat ang);

	/** \brief Zjisti presnou hranici projektilu.
	*   \return Polygon urcujici presnou hranici polygonu, prazdny polygon pokud byla pozadovana hranice dvojnika, ktery neni aktivovany.
	*/
	polygon_t Get_Border() const;

	//! \brief Zjisti miru poskozeni, kterou projektil zpusobuje.
	int Get_Damage() const { return damage[type]; }

	//! \brief Zjisti delku projektilu ve smeru vystrelu v pixelech.
	GLfloat Get_Length() const { return LENGTHS[type]; }

	//! \brief Zjisti sirku projektilu ve smeru kolmem na smer vystrelu v pixelech.
	GLfloat Get_Width() const { return WIDTHS[type]; }

	//! \brief Zjisti aktualni stav projektilu.
	states Get_Mode() const { return state; }

	//! \brief Vrati jednoznacny indentifikator vystrelu, jehoz soucasti byl tento projektil.
	unsigned int Get_Shot_Identifier() const { return shot_identifier; }

	//! \brief Zjisti, ktery hrac tento projektil vystrelil.
	Settings::players_t Get_Player() const { return player; }

	//! \brief Zjisti druh projektilu podle zbrane.
	Settings::weapons_t Get_Type() const { return type; };

	//! \brief Vykresli kazdy druh projektilu do sveho display-listu.
	static void Draw_Lists();

	//! \brief Zjisti, zda byl tento projektil vystrelen ve trojici.
	bool Is_Triple() const { return triple; }

	//! \brief Zjisti, zda je tento projektil navadeny.
	bool Is_Guided() const { return guided; }

	//! \brief Nastavi hrace, na ktereho bude projektil navadeny.
	void Set_Target(const Player & target) { this->target = &target; guided = true; }

	//! \brief Vymaze display-listy vsech druhu projektilu.
	static void Delete_Lists();

	//! \brief Okamzite znici projektil.
	void Destroy();

	/** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky.
    *   \param fps Aktualni pocet snimku za sekundu.
    */
	virtual void Update(GLfloat fps);

	//! \brief Nastavi parametry objektu podle predanych nastaveni.
	virtual void Set_Settings(const Settings & settings);

	//! \brief Vykresli bud projektil z display-listu nebo jeho vybuch.
	virtual void Draw() const;

private:

	static polygon_t borders[Settings::NUM_PROJECTILE_WEAPONS]; //!<souradnice bodu tvoricich hranice obou projektilu, pokud jsou v bode [0,0] a maji uhel natoceni 0 
	static int damage[Settings::NUM_PROJECTILE_WEAPONS]; //!<poskozeni, ktere projektily zpusobuji
	Uint32 destroy_time;   //!<cas, kdy ma skoncit exploze
	enum { EXPLOSION_DURATION = 1000 }; //!<doba trvani exploze v milisekundach
	static const GLfloat EXPLOSION_RADIUSES[Settings::NUM_PROJECTILE_WEAPONS]; //!<polomery explozi obou druhu projektilu v pixelech
	static const Color EXPLOSION_COLOR; //!<barva explozi projektilu
	const Player* target;           //!<ukazatel na hrace, na ktereho je projektil navaden, NULL pokud neni navaden.
	bool guided; //!<true, pokud je projektil navadeny
	static const GLfloat LENGTHS[Settings::NUM_PROJECTILE_WEAPONS];        //!<delka obou projektilu ve smeru vystrelu v pixelech
	static const GLfloat WIDTHS[Settings::NUM_PROJECTILE_WEAPONS];         //!<sirka obou projektilu ve smeru kolmem na smer vystrelu v pixelech
	static GLuint lists_numbers[Settings::NUM_PROJECTILE_WEAPONS]; //!<identifikatory display-listu nalezejicich jednotlivym druhum projektilu
	static Color player_colors[Settings::NUM_PROJECTILE_WEAPONS];
	Settings::players_t player;  //!<urcuje, ktery hrac tento projektil vystrelil
	states state;        //!<aktualni stav projektilu
	bool triple;         //!<urcuje, zda byl tento projektil vystrelen ve trojici
	unsigned int shot_identifier; //!<Jednoznacne identifikuje vystrel, do ktereho projektil patri.
	static bool initialized; //!<urcuje, jestli jsou vykreslene display-listy

	Settings::weapons_t type;        //!<urcuje druh projektilu podle zbrane
	static int starting_speed[Settings::NUM_WEAPONS]; //!<pocatecni rychlosti jednotlivych druhu projektilu v px/s/s
	static int guided_rotation_speed; //!<rychlost nataceni navadenych strel ve stupnich/s
	static Sound explosion_sound; //!<zvuk exploze projektilu

	
	//! \brief Vykresli explozi projektilu.
	void Draw_Explosion() const;

	//! \brief Vykresli laserovou strelu do display-listu.
	static void Draw_Laser_List();

	//! \brief Vykresli raketu do display-listu.
	static void Draw_Rocket_List();
};

#endif
