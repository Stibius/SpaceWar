
/** \file Player.h
*   \brief Obsahuje deklaraci tridy Player.
*/

#ifndef PLAYER_H_
#define PLAYER_H_

class Projectile;

#include "Moving_Object.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "Settings.h"
#include "Sound.h"
#include "Point.h"
#include "Math.h"
#include "Configurable.h"

/** \class Player
*   \brief Trida reprezentujici hracskou lod.
*/
class Player : public Moving_Object, public Configurable
{
public:

	//! \brief Mozne stavy hrace.
    enum states { 
		NORMAL, //!<normalni stav
		EXPLOSION, //!<hrac prave exploduje
		DESTROYED, //!<exploze hrace skoncila a jeste se nerespawnoval
		NUM_STATES //!<pocet moznych stavu hrace
	};

	//! \brief Oznaceni trech moznych laserovych paprsku.
	enum beams_t
	{
		LEFT_BEAM,
		CENTER_BEAM,
		RIGHT_BEAM,
		NUM_POSSIBLE_BEAMS
	};

	//! \brief Defaultni konstruktor, pouze inkrementuje pocet instanci tridy.
	Player() { num_instances++; }

	/** \brief Konstruktor, inicializuje objekt, inkrementuje ocet instanci, kresli display-listy, pokud uz nejsou nakreslene.
	*   \param settings Nastaveni parametru aplikace.
	*   \param player Urcuje, o ktereho hrace jde.
	*   \param start_position Startovni pozice hrace.
	*   \param start_angle Startovni uhel natoceni ve stupnich.
	*/
	Player(const Settings & settings, Settings::players_t player, Point start_position, GLfloat start_angle = 0);

	//! \brief Inkrementuje pocet instanci a nastavi ukazatel na hrace ve svem objektu weapon na sebe.
	Player(const Player & player);

	//! \brief Destruktor, maze display-listy, pokud je to posledni instance.
	virtual ~Player();

	//! \brief Nastavi ukazatel na hrace ve svem objektu weapon na sebe.
	Player & operator=(const Player & player);

	//! \brief Zjisti barvu hrace.
	const Color & Get_Color() const { return color; }

	//! \brief Zjisti, o ktereho hrace jde.
	Settings::players_t Get_Player_Number() const { return player_number; }

	//! \brief Zjisti aktualni hodnotu zivota.
	int Get_Health() const { return health; }

	//! \brief Zjisti aktualni hodnotu energie.
	int Get_Energy() const { return weapon.Get_Energy(); }

	//! \brief Zjisti aktualni hodnotu stitu.
	int Get_Shield() const { return shield; }

	//! \brief Zjisti maximalni moznou hodnotu zivota.
	static int Get_Max_Health() { return max_health; }

	//! \brief Zjisti maximalni moznou hodnotu stitu.
	static int Get_Max_Shield() { return max_shield; }

	//! \brief Zjisti maximalni moznou hodnotu energie.
	static int Get_Max_Energy() { return Weapon::Get_Max_Energy(); }

	//! \brief Zjisti aktualni skore tohoto hrace.
	int Get_Score() const { return score; };

	//! \brief Zjisti aktualne drzenou zbran.
	Settings::weapons_t Get_Weapon_Type() const { return weapon.Get_Type(); }

	//! \brief Zjisti spotrebu energie aktualni zbrane na jeden vystrel.
	int Get_Weapon_Energy_Usage() const { return weapon.Get_Energy_Usage(); }

	//! \brief Zjisti zbyvajici cas pusobeni powerupu Trojite strely v milisekundach.
	Uint32 Get_Triple_Remaining_Time() const { return weapon.Get_Triple_Remaining_Time(); }

	//! \brief Zjisti zbyvajici cas pusobeni powerupu Navadene strely v milisekundach.
	Uint32 Get_Guided_Remaining_Time() const { return weapon.Get_Guided_Remaining_Time(); }

	//! \brief Zjisti, jestli ma hrac trojite strely.
	bool Triple() const { return weapon.Triple(); }

	//! \brief Zjisti, jestli ma hrac navadene strely.
	bool Guided() const { return weapon.Guided(); }

	//! \brief Zjisti, jestli je spustena zbran laserovy paprsek.
	bool Beam_Active() const { return weapon.Beam_Active(); } 

	//! \brief Vrati usecku predstavujici dany laserovy paprsek.
	line_t Get_Beam_Line(beams_t beam) const;

	//! \brief Vrati poskozeni jednim laserovym paprskem za sekundu.
	static int Get_Beam_Damage() { return Weapon::Get_Beam_Damage(); }

	//! \brief Zjisti netransformovanou (pri stredu lodi v [0,0] a natoceni 0°) pozici spicky lodi.
	const Point & Get_Tip() const { return border[0]; }

	/** \brief Zjisti presnou hranici lodi nebo nektereho z jejich dvojniku.
	*   \param double_pos Dvojnik, jehoz hranice se ma zjistit. Pri hodnote ORIGINAL se zjisti hranice originalu.
	*   \return Polygon urcujici presnou hranici lodi, prazdny polygon pokud byla pozadovana hranice dvojnika, ktery neni aktivovany.
	*/
	polygon_t Get_Border(doubles_positions double_pos = ORIGINAL) const;

	/** \brief Zjisti presne hranice lodi a vsech jejich aktivovanych dvojniku.
	*   \return Vektor polygonu urcujicich presne hranice lodi a vsech jejich aktivovanych dvojniku.
	*/
	vector<polygon_t> Get_Borders() const;

	/** \brief Zjisti aktualni stav lodi.
	*   \return Aktualni stav lodi (NORMAL, EXPLOSION nebo DESTROYED).
	*/
	states Get_Mode() const { return state; }

	/** \brief Zmeni skore.
    *   \param change Mnozstvi, ktere se ma pricist ke skore.
    */
	void Change_Score(int change) { score += change; }

	//! \brief Prida dane mnozstvi zdravi.
	void Add_Health(int health);
	
	//! \brief Prida dane mnozstvi stitu.
	void Add_Shield(int shield);

	//! \brief Prida dane mnozstvi energie.
	void Add_Energy(int energy);

	//! \brief Prepne zbran.
	void Set_Weapon(Settings::weapons_t type);

	//! \brief Zapne trojite strely na danou dobu v milisekundach.
	void Triple_On(Uint32 duration) { weapon.Triple_On(duration); }

	//! \brief Zapne navadeni strel na danou dobu v milisekundach.
	void Guided_On(Uint32 duration) { weapon.Guided_On(duration); }

	//! \brief Nastavi delku daneho laseroveho paprsku, vola se v koliznim systemu, ktery delku nastavuje.
	void Set_Beam_Length(beams_t beam, GLfloat beam_length);

	//! \brief Nastavi delku daneho laseroveho paprsku na maximum, vola se v koliznim systemu, ktery delku nastavuje.
	void Set_Max_Beam_Length(beams_t beam);

	//! \brief Ustedri lodi dane poskozeni.
	void Damage(int damage);

	//! \brief Okamzite znici hrace.
	void Destroy();

	//! \brief Nastavi hrace do vychoziho stavu, ve kterem je pri spusteni hry.
	void Reset();

	//! \brief Metoda pro vykresleni.
	virtual void Draw() const;

	/** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky.
    *   \param fps Aktualni pocet snimku za sekundu.
    */
	virtual void Update(GLfloat fps);

	//! \brief Nastavi parametry objektu podle predanych nastaveni, pokud jsou nejake vlastnosti objektu nastavitelne.
	virtual void Set_Settings(const Settings & settings);

private:

	/** \class Weapon
    *   \brief Trida reprezentujici lodni zbran.
    */
	class Weapon : public Configurable, public Updatable
	{
	public:

		Weapon() : player(NULL), list_drawn(false) {}

	    /** \param settings Objekt s hernimi nastavenimi podle kterych se nastavi nektere parametry vytvareneho objektu.
	    *   \param player Pouzije se k ziskani adresy hrace, ke kteremu zbran patri.
	    *   \param type Druh zbrane.
	    */
		Weapon(const Settings & settings, Player & player, Settings::weapons_t type = Settings::LASER_WEAPON);

		//! \brief Kopirovaci konstruktor, objekt si vykresli vlastni display-list se svetelnou kouli na konci laseroveho paprsku
		Weapon(const Weapon & weapon);

		//! \brief Pretizeny operator prirazeni, objekt si vykresli vlastni display-list se svetelnou kouli na konci laseroveho paprsku
		Weapon & operator=(const Weapon & weapon);

		//! \brief Vymaze display-list se svetelnou kouli na konci laseroveho paprsku a stopne zvuky
		~Weapon();
		
		//! \brief Vrati druh zbrane.
		Settings::weapons_t Get_Type() const { return type; }

		//! \brief Vrati aktualni hodnotu energie.
		int Get_Energy() const { return energy; }

		//! \brief Vrati spotrebu energie na vystrel.
		int Get_Energy_Usage() const { return energy_usage[type]; }

		//! \brief Vrati maximalni mozne mnozstvi energie.
		static int Get_Max_Energy() { return max_energy; }

		//! \brief Zjisti, jestli jsou zapnuty trojite strely.
		bool Triple() const { return triple; }

		//! \brief Zjisti, jestli jsou zapnuty navadene projektily.
		bool Guided() const { return guided; }

		//! \brief Vrati zbyvajici cas trojitych strel v milisekundach.
		Uint32 Get_Triple_Remaining_Time() const { return triple_remaining_time; }

		//! \brief Vrati zbyvajici cas navadenych strel v milisekundach.
		Uint32 Get_Guided_Remaining_Time() const { return guided_remaining_time; }

		//! \brief Zjisti delku daneho laseroveho paprsku, delka se nastavuje v koliznim systemu.
		GLfloat Get_Beam_Length(beams_t beam) const { return beam_lengths[beam]; }

		//! \brief Zjisti, jestli je spustena zbran laserovy paprsek
		bool Beam_Active() const { return beam_active; } 

		//! \brief Vrati usecku predstavujici dany laserovy paprsek.
		line_t Get_Beam_Line(beams_t beam) const;

		//! \brief Vrati poskozeni jednim laserovym paprskem za sekundu
		static int Get_Beam_Damage() { return beam_damage; }

		//! \brief Prida dane mnozstvi energie.
		void Add_Energy(int energy);

		//! \brief Ubere dane mnozstvi energie.
		void Drain_Energy(int energy);

		//! \brief Zapne trojite strely na danou dobu v milisekundach.
		void Triple_On(Uint32 duration) { triple = true; triple_remaining_time = duration; };

		//! \brief Zapne navadene strely na danou dobu v milisekundach.
		void Guided_On(Uint32 duration) { if (type != Settings::LASER_BEAM_WEAPON) { guided = true; guided_remaining_time = duration; } };

		//! \brief Nastavi, jestli je spustena strelba.
		void Fire(bool fire) { firing = fire; }

		//! \brief Asociuje zbran s danym hracem.
		void Set_Player(Player & player) { this->player = &player; }

		//! \brief Nastavi druh zbrane.
		void Set_Type(Settings::weapons_t type);

		//! \brief Nastavi delku daneho laseroveho paprsku, vola se z kolizniho systemu, ktery delku nastavuje.
		void Set_Beam_Length(beams_t beam, GLfloat beam_length);

		//! \brief Nastavi delku daneho laseroveho paprsku na maximum, vola se z kolizniho systemu, ktery delku nastavuje.
		void Set_Max_Beam_Length(beams_t beam) { Set_Beam_Length(beam, MAX_BEAM_LENGTH); }

		//! \brief Nastavi zbran na defaultni. (Pri zniceni hrace).
		void Reset();

		//! \brief Vykresli laserovy paprsek ci paprsky, pokud jsou aktivovany.
		void Draw_Beams() const;

		/** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky.
        *   \param fps Aktualni pocet snimku za sekundu.
        */
		virtual void Update(GLfloat fps);

		//! \brief Aktualizuje parametry podle aktualnich nastaveni aplikace.
		virtual void Set_Settings(const Settings & settings);

	private:

		enum 
		{ 
			MAX_BEAM_LENGTH = 7000 //!<maximalni delka laseroveho paprsku, dost velka na to, aby byl konec mimo obrazovku
		};

		static const string laser_shot_sound_file;  //!<cesta k souboru se zvukem vystrelu laseroveho projektilu
		static const string rocket_shot_sound_file; //!<cesta k souboru se zvukem vystrelu rakety
		static const string beam_sound_file;        //!<cesta k souboru se zvukem laseroveho paprsku, kdyz nic nezasahuje
		static const string beam_hit_sound_file;    //!<cesta k souboru se zvukem laseroveho paprsku pri zasahu
		GLuint beam_end_list;                       //!<identifikator display-listu se svetelnou kouli na konci laseroveho paprsku
		bool list_drawn;                            //!<udava, zda je vykreslen display-list se svetelnou kouli na konci laseroveho paprsku
		int energy; //!<aktualni hodnota energie zbrane
		static Settings::weapons_t default_weapon; //!<implicitni typ zbrane
		static int max_energy; //!<maximalni energie zbrani (spolecna pro vsechny zbrane)
		static int energy_regeneration_rate; //!<interval regenerace energie zbrani (spolecna pro vsechny zbrane)
	    static int energy_regeneration_amount; //!<mnozstvi doplnene energie behem jednoho intervalu doplneni (spolecne pro vsechny zbrane)
		static int energy_usage[Settings::NUM_WEAPONS]; //!<spotreby energie jednotlivych zbrani na vystrel (u laseroveho paprsku spotreba za jednu sekundu)
		static int rate[Settings::NUM_PROJECTILE_WEAPONS]; //!<kadence projektilovych zbrani (minimalni interval mezi vystrely)
		static int beam_damage; //!<poskozeni laserovym paprskem za jednu sekundu
		Settings::weapons_t type; //!<druh zbrane
		bool beam_active; //!<udava, zda je prave spusten laserovy paprsek ci paprsky
		GLfloat beam_lengths[NUM_POSSIBLE_BEAMS]; //!<udava delky laserovych paprsku, nastavuji se v koliznim systemu
		unsigned int shots_counter; //!<pocita vystrely (trojity vystrel se pocita jako jeden)
		bool triple; //!<udava, zda maji byt strely trojite
		bool guided; //!<udava, zda maji byt strely navadene.
		bool firing; //!<udava, jestli se ze zbrane prave strili
		static const int TRIPLE_PROJECTILE_GAP = 10; //!<mezera mezi projektily, pokud jsou vytvoreny tri najednou (vedle sebe)
		static const int TRIPLE_BEAM_GAP = 20; //!<mezera mezi laserovymi paprsky, pokud jsou vytvoreny tri najednou (vedle sebe) 
		static const int CENTER_BEAM_GAP = 5; //!<mezera mezi spickou lodi a pozici, kde zacina prostredni paprsek
		static const int SIDE_BEAM_GAP = 20; //!<vzdalenost, o kterou zacinaji postranni paprsky pred spickou lodi
		Uint32 triple_remaining_time; //!<zbyvajici cas trojitych strel v milisekundach
	    Uint32 guided_remaining_time; //!<zbyvajici cas navadenych strel v milisekundach
		Uint32 last_shot; //!<cas, kdy bylo naposledy vystreleno
		Player* player; //!<ukazatel na hrace, ke kteremu zbran nalezi
		const Settings* settings; //!<ukazatel na objekt s hernimi nastavenimi kvuli vytvareni projektilu
		Sound laser_shot_sound; //!<zvuk vystrelu z laseru
		Sound rocket_shot_sound; //!<zvuk vystrelu rakety
		Sound beam_sound; //!<zvuk laseroveho paprsku, kdyz nic nezasahuje
		Sound beam_hit_sound; //!<zvuk laseroveho paprsku pri zasahu

		//! \brief Zjisti bod, kde se ma objevit vystreleny projektil v zavislosti na aktualni poloze a natoceni lodi.
		Point Get_Projectile_Spawn_Point() const;

		//! \brief Zjisti bod, kde ma zacinat dany paprsek v zavislosti na aktualni poloze a natoceni lodi.
		Point Get_Beam_Spawn_Point(beams_t beam) const;

		//! \brief Zpusobi vystrel z projektilove zbrane, pokud to dovoluje kadence a mnozstvi energie, v tom pripade vrati true, jinak false.
		bool Fire_Projectile();

		//! \brief Vola se pri stisknutem tlacitku strelby, kdyz je zbran laserovy paprsek. Vrati true, pokud je dostatek energie na jeho pouziti.
		bool Fire_Beams(Uint32 elapsed_time);

		//! \brief Posle SDL zpravu, ze byl vystrelen projektil, a vytvoreny projektil v parametru zpravy
		void Send_Event(Projectile* projectile) const;

		//! \brief Vykresli display-list se svetelnou kouli na konci laseroveho paprsku.
		void Draw_Beam_End_List();
	};

	static const int EXPLOSION_DURATION = 1000;  //!<delka trvani exploze
	static const GLfloat EXPLOSION_RADIUS; //!<polomer exploze lodi
	static const GLfloat RADIUS; //!<polomer kruznice opsane lodi v pixelech, pouziva se pro kolize
	Uint32 destroy_time;                 //!<cas, kdy ma skoncit exploze

	static bool initialized;  //!<udava, zda jsou vykreslene display-listy
	static int num_instances; //!<pocet instanci teto tridy, ktery je aktualne vytvoren

	static GLfloat acceleration;  //!<zrychleni pri pohybu vpred v px/s/s
	static GLfloat deceleration;  //!<zrychleni pri couvani v px/s/s
	bool accelerating;  //!<udava, zda jsou zazehnuty zadni trysky
	bool decelerating;  //!<udava, zda jsou zazehnuty predni trysky
	bool mouse_turning; //!<zda je tento hrac otaceny s pomoci mysi

	polygon_t border;  //!<souradnice bodu tvoricich hranici lodi, pokud je v bode [0,0] a ma uhel natoceni 0 

	static SDLKey key_controls[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS];    //!<nastaveni klaves pro oba hrace
	static int mouse_controls[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS]; //!<nastaveni mysi pro oba hrace
	static bool mouse[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS];         //!<udava, ktere ovladaci prvky hracu jsou nastavene na mys
	
	int health;                 //!<aktualni hodnota zivota
	static int max_health;      //!<maximalni hodnota zivota
	int shield;                 //!<aktualni hodnota stitu
	static int max_shield;      //!<maximalni hodnota stitu
	static int shield_regeneration_rate;    //!<interval regenrace stitu v milisekundach
	static int shield_regeneration_amount;; //!<mnozstvi regenrovaneho stitu behem jednoho intervalu regenrace
	int score;              //!<aktualni hodnota skore

	static GLuint acceleration_list;     //!<identifikator display-listu, ve kterem jsou vykresleny zadni trysky
	static GLuint ship_list;             //!<identifikator display-listu, ve kterem je vykreslena lod
	static GLuint deceleration_list;     //!<identifikator display-listu, ve kterem jsou vykresleny prednii trysky
	
	Settings::players_t player_number;  //!<urcuje, ktery hrac to je
	states state;  //!<aktualni stav 
	Weapon weapon; //!<lodni zbran     
	Color color;   //!<barva lodi

	Point start_position;   //!<startovni pozice lodi
	GLfloat start_angle;    //!<startovni uhel natoceni lodi ve stupnich
	
	Sound jet_sound;       //!<zvuk trysek
	Sound explosion_sound; //!<zvuk exploze lodi

	static const string jet_sound_file;  //!<cesta k souboru se zvukem trysek
	static const string explosion_sound_file;  //!<cesta k souboru se zvukem exploze lodi

	//! \brief Vykresli lod. 
	void Draw_Ship() const;

	//! \brief Vykresli explozi lode.
	void Draw_Explosion() const;

	//! \brief Vykresli zadni trysku
	void Draw_Acceleration_Jet() const;

	//! \brief Vykresli predni trysku.
	void Draw_Deceleration_Jet() const;

	//! \brief Vykresli display-listy lodi a trysek.
	void Draw_Lists();		
};

#endif