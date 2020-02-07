
/** \file Moving_Object.h
*   \brief Obsahuje deklaraci abstraktni tridy Moving_Object.
*/

#ifndef MOVING_OBJECT_H_
#define MOVING_OBJECT_H_

#include "Object.h"
#include "Updatable.h"
#include <SDL_opengl.h>
#include <SDL.h>
#include <math.h>
#include <vector>

using std::vector;

/** \class Moving_Object
*   \brief Abstraktni trida reprezentujici vsechny pohyblive objekty ve hre.
*/
class Moving_Object : public Object, public Updatable
{
public:

	//! \brief Pozice moznych dvojniku (kdyz protina hranice obrazovky a castecne vykukuje na druhe strane).
    enum doubles_positions 
	{ 
	    ORIGINAL, //!<puvodni objekt
	    FIRST_DOUBLE, //!<prvni dvojnik
	    TOP_LEFT_DOUBLE = FIRST_DOUBLE, //!<levy horni dvojnik
	    TOP_DOUBLE, //!<horni dvojnik
		TOP_RIGHT_DOUBLE,  //!<pravy horni dvojnik dvojnik
		LEFT_DOUBLE, //!<levy dvojnik
	    RIGHT_DOUBLE, //!<pravy dvojnik
		BOTTOM_LEFT_DOUBLE, //!<levy dolni dvojnik
	    BOTTOM_DOUBLE, //!<dolni dvojnik
		BOTTOM_RIGHT_DOUBLE, //!<pravy dolni dvojnik
		NUM_DOUBLES //!<pocet moznych dvojniku
    }; 

	//! \brief Symbolicke konstanty reprezentujici horizontalni smery (vpravo, vlevo a zadny smer).
	enum horizontal_direction_t 
	{ 
		LEFT, //!<smer vpravo
		RIGHT, //!<smer vlevo
		NO_DIRECTION //!<zadny smer
	};

	/** \param position Pozice stredu objektu.
	*   \param angle Uhel natoceni ve stupnich.
	*   \param max_speed Maximalni rychlost v px/s.
	*   \param rotation_speed Rychlost otaceni ve °/s.
	*   \param resistance Rychlost samovolneho zpomalovani v px/s/s.
    */
	Moving_Object(const Point & position = Point(), GLfloat angle = 0, GLfloat max_speed = 0, GLfloat rotation_speed = 0, GLfloat resistance = 0.0f);

	//! Virtualni destruktor, nic nedela.
	virtual ~Moving_Object() {};

	//! \brief Zjisti, jestli je dany dvojnik aktivovany. 
	bool Double_Activated(doubles_positions pos) const { return doubles_activation[pos]; }

	/** \brief Zjisti pozici lodi a vsech jejich aktivovanych dvojniku.
	*   \return Vektor pozic lodi a vsech jejich aktivovanych dvojniku.
	*/
	vector<Point> Get_Doubles_Positions() const;

	/** \brief Zjisti pozici daneho dvojnika.
	*   \return Pozice daneho dvojnika.
	*/
	Point Get_Double_Position(doubles_positions pos) const { return Point(Get_Center_X() + doubles_translations[pos][0], Get_Center_Y() + doubles_translations[pos][1]); }

	//! \brief Zjisti posunuti daneho dvojnika v ose x oproti originalu v pixelech. 
	static GLfloat Get_Double_Translation_X(doubles_positions pos) { return doubles_translations[pos][0]; }

	//! \brief Zjisti posunuti daneho dvojnika v ose y oproti originalu v pixelech. 
	static GLfloat Get_Double_Translation_Y(doubles_positions pos) { return doubles_translations[pos][1]; }

	//! \brief Zjisti aktualni uhel natoceni ve stupnich.
	GLfloat Get_Angle() const { return angle; }

	//! \brief Zjisti horizontalni slozku rychlosti v px/s.
	GLfloat Get_Vel_X() const { return vel_x; };

	//! \brief Zjisti vertikalni slozku rychlosti v px/s.
	GLfloat Get_Vel_Y() const { return vel_y; };

	//! \brief Zjisti uhel, pod kterym se momentalne objekt pohybuje, ve stupnich, -1 pokud se nepohybuje.
	GLfloat Get_Motion_Angle() const;

	//! \brief Zjisti celkovou rychlost ve smeru pohybu v px/s.
	GLfloat Get_Speed() const;

	//! \brief Aktivuje dvojnika na dane pozici.
	void Activate_Double(doubles_positions pos) { doubles_activation[pos] = true; }

	//! Deaktivuje vsechny dvojniky.
	void Deactivate_Doubles();

	/** \brief Nastavi posunuti dvojniku vuci originalu na zaklade rozliseni obrazovky. 
	*   \param resolution Rozliseni obrazovky ve slozkacj w a h.
    */
	static void Set_Doubles_Translations(const SDL_Rect & resolution);

	/** \brief Okamzita zmena smeru pohybu.
	*   \param angle Novy uhel pohybu ve stupnich.
    */
	void Change_Direction(GLfloat angle);

	/** \brief Vrati objekt na obrazovku, pokud z ni vyjel (na druhou stranu).
	*   \param resolution Rozliseni obrazovky ve slozkacj w a h.
    */
	void Get_To_Screen(const SDL_Rect & resolution);

	//! \brief Nastavi uhel natoceni ve stupnich.
	void Set_Angle(GLfloat angle) { this->angle = angle; };

	//! \brief Nastavi horizontalni slozku rychlosti v px/s. 
	void Set_Vel_X(GLfloat vel_x) { this->vel_x = vel_x; };

	//! \brief Nastavi vertikalni slozku rychlosti v px/s. 
	void Set_Vel_Y(GLfloat vel_y) { this->vel_y = vel_y; };

	/** \brief Nastavi rychlost a uhel pohybu.
    *   \param velocity Pozadovana rychlost v px/s.
	*   \param angle Pozadovany uhel pohybu ve stupnich.
    */
	void Set_Velocity(GLfloat velocity, GLfloat angle);

	//! \brief Nastavi rychlost samovolneho zpomalovani v px/s/s.
	void Set_Resistance(GLfloat resistance) { this->resistance = resistance; }

	//! \brief Nastavi hodnotu maximalni rychlosti v px/s. 
	void Set_Max_Speed_Value(GLfloat speed) { max_speed = speed; }

	//! \brief Nastavi rychlost otaceni ve °/s. 
	void Set_Rotation_Speed(GLfloat speed) { rotation_speed = speed; }

	/** \brief Presune objekt o urcitou vzdalenost urcitym smerem.
	*   \param distance Vzdalenost, o kterou se posouva, v pixelech.
	*   \param angle Uhel ve stupnich udavajici smer, kterym se posouva.
    */
	void Move(GLfloat distance, GLfloat angle);

	/** \brief Zapusobi na objekt silou v urcitem uhlu.
	*   \param force Zrychleni, ktere sila zpusobuje v px/s/s.
	*   \param angle Uhel ve stupnich udavajici smer, ve kterem sila pusobi.
	*   \param fps Aktualni pocet snimku za sekundu.
    */
	void Push(GLfloat force, GLfloat angle, GLfloat fps);

	/** \brief Orotuje objekt v urcitem smeru.
	*   \param direction Smer, kterym se rotuje.
	*   \param fps Aktualni pocet snimku za sekundu.
    */
	void Rotate(horizontal_direction_t direction, GLfloat fps);

	/** \brief Zpomali pohyb objektu na zaklade hodnoty samovolneho zpomalovani a uplynuleho casu od posledniho volani teto metody.
	*   \param fps Aktualni pocet snimku za sekundu. (Metoda se vola pri kazdem prekresleni.)
    */
	void Slow_Down(GLfloat fps);

	/** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky.
    *   \param fps Aktualni pocet snimku za sekundu.
    */
	virtual void Update(GLfloat fps) = 0;

	//! Vykresleni.
	virtual void Draw() const = 0;

private:

	GLfloat angle;               //!<aktualni uhel natoceni ve stupnich
	bool doubles_activation[NUM_DOUBLES];                //!<signalizuje aktivaci jednotlivych dvojniku
	static GLfloat doubles_translations[NUM_DOUBLES][2]; //!<posunuti dvojniku vzhledem k originalu
	GLfloat max_speed;           //!<maximalni mozna rychlost v px/s
	GLfloat rotation_speed;      //!<rychlost rotace ve °/s
	GLfloat vel_x;               //!<aktualni horizontalni slozka rychlosti v px/s
	GLfloat vel_y;               //!<aktualni vertikalni slozka rychlosti v px/s
	GLfloat resistance;          //!<Rychlost samovolneho zpomalovani v px/s/s	
};

#endif