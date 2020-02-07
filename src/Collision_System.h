
/** \file Collision_System.h
*   \brief Obsahuje deklaraci tridy Collision_System.
*/

#ifndef COLLISION_SYSTEM_H_
#define COLLISION_SYSTEM_H_

#include "Player.h"
#include "Powerup_System.h"
#include "Projectile.h"
#include "Star.h"

/** \class Collision_System
*   \brief Trida reprezentujici kolizni system. Obsahuje ukazatele na vsechny herni objekty, se kterymi pracuje. Zjistuje kolize a reaguje na ne.
*/
class Collision_System 
{
private:

	//! \brief Symbolicka jmena pro okraje okna, 12 usecek.
	enum margins 
	{ 
        TOP_LEFT_LEFT_MARGIN,                   //!<pokracovani horniho okraje za levym okrajem           
		TOP_LEFT_TOP_MARGIN,                    //!<pokracovani leveho okraje za hornim okrajem
		TOP_MARGIN,                             //!<horni okraj
		TOP_RIGHT_TOP_MARGIN,                   //!<pokracovani praveho okraje za hornim okrajem
		TOP_RIGHT_RIGHT_MARGIN,                 //!<pokracovani horniho okraje za pravym okrajem
		BOTTOM_LEFT_LEFT_MARGIN,                //!<pokracovani dolniho okraje za levym okrajem
		LEFT_MARGIN,                            //!<levy okraj
		BOTTOM_MARGIN,                          //!<spodni okraj
		RIGHT_MARGIN,                           //!<pravy okraj
		BOTTOM_RIGHT_RIGHT_MARGIN,              //!<pokracovani spodniho okraje za pravym okrajem
		BOTTOM_LEFT_BOTTOM_MARGIN,              //!<pokracovani leveho okraje za spodnim okrajem
		BOTTOM_RIGHT_BOTTOM_MARGIN,             //!<pokracovani praveho okraje za spodnim okrajem
		NUM_MARGINS
	};

    Player* players[Settings::NUM_PLAYERS]; 
	vector<Projectile*>* projectiles; 
	vector<Powerup_System::Powerup*>* powerups; 
	const Star* star; 
	line_t margin_lines[NUM_MARGINS]; //!<usecky tvorici hranice okna
	SDL_Rect resolution;

	//! \return true, pokud koliduji, jinak false.
	bool Collide(const Player & player1, const Player & player2) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Player & player, const Projectile & projectile) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Player & player, const Star & star) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Player & player, const Powerup_System::Powerup & powerup) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Projectile & projectile1, const Projectile & projectile2) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Projectile & projectile, const Star & star) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Powerup_System::Powerup & powerup, const Star & star) const;

	//! \return true, pokud koliduji, jinak false
	bool Collide(const Powerup_System::Powerup & powerup1, const Powerup_System::Powerup & powerup2) const;

	//! \return true, pokud objekt koliduje s hranicemi obrazovky.
	bool Collide(Object & object, margins margin) const;

	//! \brief Provede kontrolu kolizi mezi laserovymi paprsky (pokud jsou) a hernimi objekty. Pri kolizich provede prislusne zmeny paprsku a techto objektu.
	void Manage_Beam_Collisions() const;

public:

	//! \brief Nastavi ukazatele na NULL.
	Collision_System() 
		: projectiles(NULL), powerups(NULL), star(NULL) { players[0] = NULL; players[1] = NULL; }

	/** \param resolution Pouziji se jen slozky w a h jako rozliseni obrazovky. Slouzi pro nastaveni usecek tvoricich hranice okna.
	*   \param player1 Adresa se priradi ukazateli ve vytvarenem objektu.
	*   \param player2 Adresa se priradi ukazateli ve vytvarenem objektu.
	*   \param projectiles Adresa se priradi ukazateli ve vytvarenem objektu.
	*   \param powerups Adresa se priradi ukazateli ve vytvarenem objektu.
	*   \param star Adresa se priradi ukazateli ve vytvarenem objektu.
    */
	Collision_System(const SDL_Rect & resolution, Player & player1, Player & player2, vector<Projectile*> & projectiles, Powerup_System & powerup_system, const Star & star);
	
	//! \brief Provede kontrolu kolizi mezi hernimi objekty. Pri kolizich provede prislusne zmeny techto objektu.
	void Manage_Collisions() const;

	/** \brief Nastavi novemu bonusu nahodnou pozici tak, aby s nicim nekolidoval.
	*   \param area Udava oblast, ve ktere se bonus muze nachazet. Slozky x, y udavaji pozici leveho horniho okraje oblasti. Slozky w, h jeji rozmery.
    */
	void Set_New_Powerup_Position(Powerup_System::Powerup & new_powerup, const SDL_Rect & area) const;

	/** \brief Nastavi dvojniky daneho herniho objektu na zaklade jeho kolizi s okraji obrazovky. Pokud koliduje s hranou obrazovky, musi se jeho cast objevit na druhe strane => dvojnik.
    *   \return true, pokud byl aktivovany nektery dvojnik, jinak false.
    */
	bool Determine_Doubles(Moving_Object & object) const;

	/** \brief Nastavi souradnice usecek tvoricich hranice okna podle aktualniho rozliseni.
	*   \param resolution Pouziji se jen slozky w a h jako rozliseni obrazovky.
    */
	void Set_Margins(const SDL_Rect & resolution);
};

#endif