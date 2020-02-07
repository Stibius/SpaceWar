
/** \file Star.h
*   \brief Obsahuje deklaraci tridy Star.
*/

#ifndef STAR_H_
#define STAR_H_

#include "Circle.h"
#include "Settings.h"
#include "Point.h"
#include "Player.h"
#include "Projectile.h"

/** \class Star
*   \brief Trida reprezentujici hvezdu s gravitaci.
*/
class Star : public Object, public Configurable, public Updatable
{
private:

	int ship_gravity;          //!<gravitacni zrychleni lodi(v px/s/s)
	int projectile_gravity;    //!<gravitacni zrychleni projektilu(v px/s/s)
	GLuint list_number;   //!<identifikator display-listu, ve kterem je hvezda vykreslena
	int num_vertical_lines; //!<pocet svislych car
	int num_horizontal_lines; //!<pocet vodorovnych car
	Color color;          //!<barva hvezdy
	bool active;          //!<urcuje, zda je hvezda zapnuta
	Player* players[Settings::NUM_PLAYERS]; //!<odkazy na hrace, pres ktere je hvezda ovlivnuje
	vector<Projectile*>* projectiles; //!<odkazy na projektily, pres ktere je hvezda ovlivnuje

    //! \brief vykresli hvezdu s danym polomerem a poctem vodorovnych a svislych car se stredem v [0,0]
	void Draw_Star(GLfloat radius, int num_vertical_lines, int num_horizontal_lines);
	
public:

	//! \brief Nastavi ukazatele na hrace na NULL.
	Star() { players[Settings::FIRST] = NULL; players[Settings::SECOND] = NULL; }

	/** \brief Inicializuje objekt a vykresli display-list.
	*   \param player1 Prvni hrac, objekt si uchova ukazatel na nej, aby ho hvezda mohla ovlivnovat svoji gravitaci.
	*   \param player2 Druhy hrac, objekt si uchova ukazatel na nej, aby ho hvezda mohla ovlivnovat svoji gravitaci.
	*   \param settings Nastaveni, podle kterych se nastavi parametry hvezdy.
	*   \param radius Polomer hvezdy v pixelech.
	*   \param num_vertical_lines Pocet svislych car.
	*   \param num_horizontal_lines Pocet vodorovnych car,
	*   \param position Pozice stredu hvezdy.
	*/
	Star(Player & player1, Player & player2, vector<Projectile*> & projectiles, const Settings & settings, GLfloat radius, int num_vertical_lines, int num_horizontal_lines, const Point & position);

	//! \brief Vykresli novy display_list.
	Star(const Star & obj);

	//! \brief Vykresli novy display_list a smaze stary.
	Star & operator=(const Star & star);

	//! \brief Destruktor, maze display-list.
	~Star() { glDeleteLists(list_number, 1); }

	//! \brief Zjisti, zda je hvezda zapnuta.
	bool Is_Active() const { return active; }

	//! \brief Nastavi parametry objektu podle predanych nastaveni.
	virtual void Set_Settings(const Settings & settings);

	//! \brief Vykresleni display-listu.
	virtual void Draw() const;

	/** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky. Hvezda ovlivni rychlosti hracu a projektilu.
	*   \param fps Aktualni pocet snimku za sekundu.
	*/
	virtual void Update(GLfloat fps);
};

#endif


