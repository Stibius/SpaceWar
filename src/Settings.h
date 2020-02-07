
/** \file Settings.h
*   \brief Obsahuje deklaraci tridy Settings.
*/

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Color.h"
#include "Events.h"
#include <SDL.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

/** \class Settings
*   \brief Trida pro uchovani hernich nastaveni a praci s nimi.
*/
class Settings
{
public:

	//! \brief Symbolicke konstanty urcujici hrace.
    enum players_t 
	{ 
		FIRST,  //!<prvni hrac
		SECOND, //!<druhy hrac
		NONE,   //!<zadny hrac
		NUM_PLAYERS = NONE, //!<pocet hracu
		NUM_PLAYER_VALUES   //!<pocet hodnot v teto enumeraci
	};

	//! \brief Druhy zbrani.
    enum weapons_t { 
		LASER_WEAPON, //!<laserove strely
		ROCKET_WEAPON, //!<rakety
		NUM_PROJECTILE_WEAPONS, //!<pocet projektilovych zbrani
		LASER_BEAM_WEAPON = NUM_PROJECTILE_WEAPONS, //!<laserovy paprsek
		NUM_WEAPONS //!<pocet druhu zbrani
	};

	//! \brief Symbolicke konstanty urcujici druhy ovladani (mys nebo klavesnice). 
	enum control_types_t 
	{
		KEY, //!<ovladani klavesou     
		MOUSE_BUTTON //!<ovladani tlacitkem mysi
	};

	//! \brief Symbolicke konstanty urcujici jednotliva nastaveni typu, kdy je danemu nastaveni prirazen jeden z hracu. 
	enum player_settings_t 
	{
		MOUSE_TURNING_PLAYER, //!<hrac, ktery je otaceny s pomoci mysi
		NUM_PLAYER_SETTINGS   //!<pocet nastaveni tohoto druhu
	};

	//! \brief Symbolicke konstanty urcujici jednotliva nastaveni typu, kdy je danemu nastaveni prirazena jedna ze zbrani
	enum weapon_settings_t
	{
		DEFAULT_WEAPON, //!<implicitni zbran
		NUM_WEAPON_SETTINGS   //!<pocet nastaveni tohoto druhu
	};

	//! \brief Symbolicke konstanty urcujici jednotliva nastaveni typu bool (ANO/NE).
    enum bool_settings_t 
	{ 
		STAR,                 //!<urcuje, zda ma byt na herni plose hvezda
		FULLSCREEN,             //!<urcuje, zda ma hra bezet ve fullscreenu
		FPS,                    //!<urcuje, zda se ma pri hre zobrazovat fps
		GRID,                   //!<urcuje, zda se ma pri hre vykreslovat mrizka na pozadi
		HEALTH_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Doplneni zivota
		SHIELD_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Doplneni stitu
		ENERGY_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Doplneni energie
		TRIPLE_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Trojite strely
		GUIDED_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Navadene strely
		LASER_POWERUP_ALLOWED,  //!<urcuje, zda je povolen powerup Laserove strely
		ROCKET_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Rakety
		LASER_BEAM_POWERUP_ALLOWED, //!<urcuje, zda je povolen powerup Laserovy paprsek
		NUM_BOOL_SETTINGS       //!<pocet nastaveni typu bool
	};

    //! \brief Symbolicke konstanty urcujici jednotliva nastaveni typu int.
    enum int_settings_t 
	{ 
		SHIP_GRAVITY,      //!<gravitacni zrychleni pusobici na lode
		PROJECTILE_GRAVITY, //!<gravitacni zrychleni pusobici na projektily
		ACCELERATION, //!<zrychleni lodi vpred
		DECELERATION, //!<zrychleni lodi vzad
		PLAYER_RESISTANCE,     //!<samovolne zpomalovani lodi
		LASER_RESISTANCE, //!<samovolne zpomalovani laserovyh projektilu
		ROCKET_RESISTANCE, //!<samovolne zpomalovani raket
		MAX_SPEED, //!<maximalni rychlost lodi
		PLAYER_ROTATION_SPEED,  //!<rychlost otaceni lodi
		GUIDED_MISSILE_ROTATION_SPEED, //!<rychlost otaceni navadenych strel
		LASER_SPEED,    //!<pocatecni rychlost laserovych projektilu 
		ROCKET_SPEED,   //!<pocatecni rychlost raket
		LASER_DAMAGE,   //!<poskozeni laserovou strelou
		ROCKET_DAMAGE,  //!<poskozeni raketou
		LASER_BEAM_DAMAGE, //!<poskozeni laserovym paprskem
		LASER_RATE,     //!<kadence laserovych strel
		ROCKET_RATE,    //!<kadence raket
		MAX_HEALTH,     //!<maximum zdravi
		MAX_SHIELD,     //!<maximum stitu
		MAX_ENERGY,     //!<maximum energie
		SHIELD_REGENERATION_INTERVAL,   //!<interval regenerace stitu
		SHIELD_REGENERATION_AMOUNT,     //!<mnozstvi regenerovaneho stitu za jeden inerval
		ENERGY_REGENERATION_INTERVAL,   //!<interval regenerace energie
		ENERGY_REGENERATION_AMOUNT,     //!<mnozstvi regenerovane energie za jeden inerval
		POWERUP_DISPLAY_DURATION, //!<doba, po kterou zustane po svem vygenerovani powerup na herni plose
		POWERUP_MIN_INTERVAL, //!<minimalni inerval mezi vygenerovanymi powerupy
		POWERUP_MAX_INTERVAL, //!<maximalni inerval mezi vygenerovanymi powerupy
		HEALTH_POWERUP_AMOUNT, //!<mnozstvi zivotu, ktere doplnuje powerup Doplneni zivota
		SHIELD_POWERUP_AMOUNT, //!<mnozstvi stitu, ktere doplnuje powerup Doplneni stitu
		ENERGY_POWERUP_AMOUNT, //!<mnozstvi energie, ktere doplnuje powerup Doplneni energie
		TRIPLE_POWERUP_DURATION, //!<doba trvani efektu powerupu Trojite strely
		GUIDED_POWERUP_DURATION, //!<doba trvani efektu powerupu Navadene strely
		LASER_ENERGY_USAGE, //!<mnozstvi energie spotrebovane na vystrel jedne laserove strely
		ROCKET_ENERGY_USAGE, //!<mnozstvi energie spotrebovane na vystrel jedne rakety
		LASER_BEAM_ENERGY_USAGE, //!<mnozstvi energie spotrebovane laserovym paprskem za sekundu
		NUM_STARS, //!<pocet hvezd na pozadi
		NUM_INT_SETTINGS, //!<pocet nastaveni typu int
		NO_INT_SETTING //!<zadne z nastaveni typu int
	};

	//! \brief Symbolicke konstanty urcujici ovladaci prvky hracske lode.
	enum controls_t 
	{
		ACCELERATE, //!<pohyb vpred
	    DECELERATE, //!<pohyb vzad
	    TURN_LEFT,  //!<otocit doleva
	    TURN_RIGHT, //!<otocit doprava
	    FIRE,       //!<vystrelit
		NUM_SHIP_CONTROLS //!<pocet ovladacich prvku hracske lode
	};

	//! \brief Symbolicke konstanty urcujici jednotliva nastaveni typu barva.
	enum color_settings_t 
	{
		PLAYER1_COLOR, //!<barva prvniho hrace 
		PLAYER2_COLOR, //!<barva druheho hrace
		STAR_COLOR,  //!<barva hvezdy
		GRID_COLOR,    //!<barva mrizky na pozadi
		NUM_COLOR_SETTINGS //!<pocet nastaveni typu barva
	};

	//! \brief Precte nastaveni z konfiguracniho souboru, pokud se to nepovede, nastavi defaultni nastaveni a vytvori konfiguracni soubor s nimi.
	Settings();	

	//! \brief Zjisti hodnotu zadaneho parametru typu bool.
	bool Get_Setting_Value(bool_settings_t setting) const;

	//! \brief Zjisti hodnotu zadaneho parametru typu int.
	int Get_Setting_Value(int_settings_t setting) const { return int_settings[setting].Get_Value(); }

	//! \brief Zjisti druh nastaveni zadaneho ovladaciho prvku zadaneho hrace (klavesnice nebo mys).
	control_types_t Get_Control_Type(controls_t control, players_t player) const { return key_settings[player][control].Get_Type(); }

	//! \brief Zjisti klavesu prirazenou zadanemu ovladacimu prvku zadaneho hrace.
	SDLKey Get_Key(controls_t control, players_t player) const { return key_settings[player][control].Get_Key_Value(); }

	//! \brief Zjisti tlacitko mysi prirazene zadanemu ovladacimu prvku zadaneho hrace.
	int Get_Mouse_Button(controls_t control, players_t player) const { return key_settings[player][control].Get_Mouse_Button_Value(); }

	//! \brief Zjisti hodnotu zadaneho parametru typu barva.
	Color Get_Setting_Value(color_settings_t setting) const;

	//! \brief Zjisti hodnotu zadaneho parametru typu hrac.
	players_t Get_Setting_Value(player_settings_t setting) const;

	//! \brief Zjisti hodnotu zadaneho parametru typu zbran.
	weapons_t Get_Setting_Value(weapon_settings_t setting) const;

	/** \brief Zjisti hodnotu aktualne nastaveneho rozliseni.
	*   \return Aktualne nastavene rozliseni ve slozkach w a h.
    */
    SDL_Rect Get_Resolution() const;

	//! \brief Vrati textovou reprezentaci hodnoty zadaneho parametru typu bool.
	string Get_Setting_Value_Text(bool_settings_t setting) const { return Bool_To_String(bool_settings[setting].Get_Value()); }

	//! \brief Vrati textovou reprezentaci klavesy prirazene zadanemu ovladacimu prvku zadaneho hrace.
	string Get_Setting_Value_Text(controls_t control, players_t player) const; 

	//! \brief Vrati textovou reprezentaci hodnoty zadaneho parametru typu barva.
	string Get_Setting_Value_Text(color_settings_t setting) const;

	//! \brief Vrati textovou reprezentaci hodnoty zadaneho parametru typu hrac.
	string Get_Setting_Value_Text(player_settings_t setting) const { return Player_To_String(player_settings[setting].Get_Value()); }

	//! \brief Vrati textovou reprezentaci hodnoty zadaneho parametru typu zbran.
	string Get_Setting_Value_Text(weapon_settings_t setting) const { return Weapon_To_String(weapon_settings[setting].Get_Value()); }

	//! \brief Vrati textovou reprezentaci hodnoty aktualne nastaveneho rozliseni.
	string Get_Resolution_Text() const;

	//! \brief Vrati oznaceni zadaneho parametru typu bool, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(bool_settings_t setting) const { return bool_settings[setting].Get_Label(); }

	//! \brief Vrati oznaceni zadaneho parametru typu int, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(int_settings_t setting) const { return int_settings[setting].Get_Label(); }

	//! \brief Vrati oznaceni zadaneho ovladaciho prvku, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(controls_t control, players_t player) const { return key_settings[player][control].Get_Label(); }

	//! \brief Vrati oznaceni zadaneho parametru typu barva, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(color_settings_t setting) const { return color_settings[setting].Get_Label(); }

	//! \brief Vrati oznaceni zadaneho parametru typu hrac, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(player_settings_t setting) const { return player_settings[setting].Get_Label(); }

	//! \brief Vrati oznaceni zadaneho parametru typu zbran, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Setting_Label(weapon_settings_t setting) const { return weapon_settings[setting].Get_Label(); }

	//! \brief Vrati oznaceni parametru rozliseni, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	string Get_Resolution_Label() const { return resolution.Get_Label(); }

	//! \brief Vrati jednotky zadaneho parametru typu int vypisovane v konfiguracnim souboru a menu za jeho hodnotou.
	string Get_Setting_Units(int_settings_t setting) const { return int_settings[setting].Get_Units(); }

	//! \brief Vrati minimalni nastavitelnou hodnotu zadaneho parametru typu int.
	int Get_Min(int_settings_t setting) const { return int_settings[setting].Get_Min(); } 

	//! \brief Vrati maximalni nastavitelnou hodnotu zadaneho parametru typu int.
    int Get_Max(int_settings_t setting) const { return int_settings[setting].Get_Max(); } 

	//! \brief Nastavi vsechny parametry na defaultni hodnoty.
	void Set_Default_Settings();

	//! \brief Nastavi zadany parametr typu bool.
	void Set_Setting(bool_settings_t setting, bool value);

	//! \brief Nastavi zadany parametr typu int.
	void Set_Setting(int_settings_t setting, int value);

	//! \brief Priradi zadanou klavesu zadanemu ovladacimu prvku zadaneho hrace.
	void Set_Setting(controls_t control, players_t player, SDLKey key);

	//! \brief Priradi zadane tlacitko mysi zadanemu ovladacimu prvku zadaneho hrace.
	void Set_Setting(controls_t control, players_t player, int mouse_button);

	//! \brief Nastavi zadany parametr typu barva.
	void Set_Setting(color_settings_t setting, const Color & value);

	//! \brief Nastavi zadany parametr typu hrac.
	void Set_Setting(player_settings_t setting, players_t player); 

	//! \brief Nastavi zadany parametr typu zbran.
	void Set_Setting(weapon_settings_t setting, weapons_t weapon); 

	/** \brief Nastavi hodnotu rozliseni.
	*   \param resolution Hodnota rozliseni v prvcich w a h.
    */
	void Set_Resolution(const SDL_Rect & resolution);

	/** \brief Nastavi seznam moznych rozliseni. Pokud aktualne nastavene rozliseni neni v novem seznamu, nastavi se posledni ze seznamu.  
	*   \param range Vektor moznych rozliseni k nastaveni. Nesmi byt prazdny.
    */
	void Set_Resolution_Range(const vector<SDL_Rect> & range);

	/** \brief Nastavi seznam moznych rozliseni. Pokud aktualne nastavene rozliseni neni v novem seznamu, nastavi se posledni ze seznamu.  
	*   \param range Pole ukazatelu na SDL_Rect.
    */
	void Set_Resolution_Range(SDL_Rect** resolutions);

	//! \brief Nastavi nasledujici rozliseni ze seznamu moznych rozliseni. Pokud je prave nastaveno posledni, nastavi se prvni.
	void Resolution_Up();

	//! \brief Nastavi predchozi rozliseni ze seznamu moznych rozliseni. Pokud je prave nastaveno prvni, nastavi se posledni.
	void Resolution_Down();

	//! \brief Nastavi nasledujici barvu ze seznamu moznych barev pro dane nastaveni typu barva. Pokud je prave nastavena posledni, nastavi se prvni.
	void Color_Up(color_settings_t setting);

	//! \brief Nastavi predchozi bravu ze seznamu moznych barev pro dane nastaveni typu barva. Pokud je prave nastavena prvni, nastavi se posledni.
	void Color_Down(color_settings_t setting);

	//! \brief Nastavi opacneho hrace pro dane nastaveni typu hrac.
	void Player_Setting_Up(player_settings_t setting);

	//! \brief Nastavi opacneho hrace pro dane nastaveni typu hrac.
	void Player_Setting_Down(player_settings_t setting);

	//! \brief Nastavi opacneho hrace pro dane nastaveni typu hrac.
	void Weapon_Setting_Up(weapon_settings_t setting);

	//! \brief Nastavi opacneho hrace pro dane nastaveni typu hrac.
	void Weapon_Setting_Down(weapon_settings_t setting);

	//! \brief Vytvori konfiguracni soubor s aktualnimi nastavenimi.
	void Write_Config() const;

private:

	static const int MAX_STARS = 2000; //!<maximalni pocet hvezd na pozadi

	//! \brief Defaultni hodnoty nastaveni typu bool.
    static const bool DEFAULT_BOOL_SETTINGS[NUM_BOOL_SETTINGS];

    //! \brief Defaultni hodnoty nastaveni typu int.
    static const int DEFAULT_INT_SETTINGS[NUM_INT_SETTINGS];

    //! \brief Defaultni hodnoty nastaveni ovladacich prvku obou hracu.
    static const SDLKey DEFAULT_CONTROLS[NUM_PLAYERS][NUM_SHIP_CONTROLS]; 

	//! \brief Minimalni mozne rozliseni.
	static const SDL_Rect MIN_RESOLUTION;

	//! \brief Defaultni hodnoty nastaveni typu barva.
	static const Color DEFAULT_COLOR_SETTINGS[NUM_COLOR_SETTINGS];

	//! \brief Defaultni hodnoty nastaveni typu hrac.
	static const players_t DEFAULT_PLAYER_SETTINGS[NUM_PLAYER_SETTINGS];

	//! \brief Defaultni hodnoty nastaveni typu zbran.
	static const weapons_t DEFAULT_WEAPON_SETTINGS[NUM_WEAPON_SETTINGS];
	
	//! \brief Pocet pouzitelnych klaves.
	static const int NUM_KEYS = 232; 

	//! \brief Seznam pouzitelnych klaves.
    static const SDLKey SDL_KEYS[NUM_KEYS]; 

	//! \brief Jmena pouzitelnych klaves pro zobrazeni v menu.
    static const string KEY_NAMES[NUM_KEYS]; 

	//! \brief Pocet pouzitelnych barev.
	static const int NUM_COLORS = 10; 

	//! \brief Seznam pouzitelnych barev.
	static const Color COLORS[NUM_COLORS];       

	//! \brief Jmena pouzitelnych barev pro zobrazeni v menu.
	static const string COLOR_NAMES[NUM_COLORS]; 

	//! \brief Mozne hodnoty parametru typu hrac.
	static const players_t PLAYER_VALUES[NUM_PLAYER_VALUES]; 

	//! \brief Jmena moznych hodnot parametru typu hrac pro zobrazeni v menu.
	static const string PLAYER_NAMES[NUM_PLAYER_VALUES]; 

	//! \brief Mozne hodnoty parametru typu hrac.
	static const weapons_t WEAPON_VALUES[NUM_WEAPONS];

	//! \brief Jmena moznych hodnot parametru typu zbran pro zobrazeni v menu.
	static const string WEAPON_NAMES[NUM_WEAPONS];

	//! \brief Pocet pouzitelnych tlacitek mysi.
	static const int NUM_MOUSE_BUTTONS = 7; 

	//! \brief Seznam kodu pouzitelnych tlacitek mysi podle SDL.
	static const int MOUSE_BUTTONS[NUM_MOUSE_BUTTONS];         

	//! \brief Jmena pouzitelnych tlacitek mysi pro zobrazeni v menu.
	static const string MOUSE_BUTTON_NAMES[NUM_MOUSE_BUTTONS]; 

	//! \brief Pocet moznych hodnot parametru typu bool.
	static const int NUM_BOOL_VALUES = 2; 

	//! \brief Mozne hodnoty parametru typu bool.
	static const bool BOOL_VALUES[NUM_BOOL_VALUES];     

	//! \brief Jmena moznych hodnot parametru typu bool pro zobrazeni v menu.
	static const string BOOL_NAMES[NUM_BOOL_VALUES];    

	/** \class Setting
    *   \brief Abstraktni trida, ze ktere se odvozuji tridy reprezentujici jednotlive typy hernich parametru.
    */
	class Setting
	{
	private:
		string label; //!<Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
	public:
		//! \brief Defaultni konstruktor, nic nedela.
		Setting() {}

		/** \brief Nastavuje label.
	    *   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
        */
		explicit Setting(string label) : label(label) {}

		/** \brief Vrati hodnotu label.
		*   \return Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
        */
		string Get_Label() const { return label; }

		/** \brief Najde podle hodnoty label v konfiguracnim souboru hodnotu tohoto parametru a vrati ji ve forme retezce.
	    *   \param config_content Retezec s celym obsahem konfiguracniho souboru.
		*   \return Hodnota tohoto parametru z konfiguracniho souboru jako string, prazdny retezec, pokud se nepodarilo ziskat.
        */
		string Find_Config_Value(string config_content) const;

	};

	/** \class Choice_Setting
    *   \brief Sablona tridy, odvozena od abstraktni tridy Setting, reprezentujici typy nastaveni (bool, barva, hrac, rozliseni), kdy se vybira z nekolika predem definovanych hodnot daneho typu.
	*   \tparam <Type> Typ hodnot nastaveni. 
    */
	template <typename Type>
	class Choice_Setting : public Setting
	{
	private:

		vector<Type> choices; //!<Seznam moznych hodnot.
		size_t index; //!<index aktualne nastavene hodnoty ve vektoru moznych hodnot

	public:

		//! \brief Defaultni konstruktor, prazdny seznam hodnot.
		Choice_Setting() {}

		/** \brief Nastavi pouze label, seznam hodnot je prazdny.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
        */
		explicit Choice_Setting(string label) : Setting(label) {}

		/** \brief Nastavi label a seznam hodnot.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
		*   \param choices Ukazatel na pole hodnot, ktere se maji pouzit jako seznam hodnot.
		*   \param length Pocet predavanych hodnot.
        */
		Choice_Setting(string label, const Type * choices, int length);

		//! \brief Vrati aktualne nastavenou hodnotu. Pokud je seznam moznych hodnot prazdny, vrati defaultni hodnotu daneho typu.
		Type Get_Value() const;

		//! \brief Vrati seznam moznych hodnot.
		const vector<Type> Get_Choices() const { return choices; }

		/** \brief Nastavi parametr na zadanou hodnotu, pokud tato je v seznamu moznych hodnot, jinak se nic nestane.
		*   \param value Hodnota, ktera se ma nastavit.
		*   \return true, pokud se hodnotu podarilo nastavit (je v seznamu moznych hodnot), jinak false.
        */
		bool Set_Value(const Type & value);
	
		//! \brief Nastavi seznam moznych hodnot.
		void Set_Choices(const vector<Type> & new_choices);

		//! \brief Nastavi seznam moznych hodnot na jedinou hodnotu.
		void Set_Choices(const Type & value);

		/** \brief Nastavi seznam moznych hodnot.
		*   \param choices Ukazatel na pole hodnot, ktere se maji pouzit jako seznam hodnot.
		*   \param length Pocet predavanych hodnot.
        */
		void Set_Choices(const Type * choices, int length);

		//! \brief Nastavi jako aktualni hodnotu nasledujici hodnotu ze seznamu hodnot. Pokud je aktualni hodnota posledni v seznamu, nastavi se prvni v seznamu.
		void Up();

		//! \brief Nastavi jako aktualni hodnotu predchazejici hodnotu ze seznamu hodnot. Pokud je aktualni hodnota prvnii v seznamu, nastavi se posledni v seznamu.
		void Down();
	};

	/** \class Control_Setting
    *   \brief Trida reprezentujici nastaveni ovladaciho prvku.
    */
	class Control_Setting : public Setting
	{
	private:

		SDLKey key;  //!<aktualne nastavena klavesa
		int mouse_button; //!<aktualne nastavene tlacitko mysi
		control_types_t type; //!<typ ovladani prirazeny tomuto ovladacimu prvku (klavesa nebo tlacitko mysi)

	public:

		//! \brief Defaultni konstruktor, nic nedela.
		Control_Setting() {}

		/** \brief Nastavi label a klavesu.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
		*   \param key Klavesa, na kterou se ma ovladaci prvek nastavit.
        */
		Control_Setting(string label, SDLKey key) : Setting(label), key(key), type(KEY) {}

		/** \brief Nastavi label a tlacitko mysi.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
		*   \param mouse_button Tlacitko mysi, na ktere se ma ovladaci prvek nastavit.
        */
		Control_Setting(string label, int mouse_button) : Setting(label), mouse_button(mouse_button), type(MOUSE_BUTTON) {}

		//! \brief Vrati typ ovladani prirazeny tomuto ovladacimu prvku (klavesa nebo tlacitko mysi).
		control_types_t Get_Type() const { return type; }

		//! \brief Vrati klavesu, na kterou je tento ovladaci prvek prave nastaven.
		SDLKey Get_Key_Value() const { return key; }

		//! \brief Vrati kod tlacitka mysi (podle SDL), na ktere je tento ovladaci prvek prave nastaven.
		int Get_Mouse_Button_Value() const { return mouse_button; }

		//! \brief Nastavi ovladaci prvek na zadanou klavesu.
		void Set_Key_Value(SDLKey key) { this->key = key; type = KEY; }

		/** \brief Nastavi ovladaci prvek na zadane tlacitko mysi.
		*   \param mouse_button Kod tlacitka mysi, ktere se ma nastavit, podle SDL.
        */
		void Set_Mouse_Button_Value(int mouse_button) { this->mouse_button = mouse_button; type = MOUSE_BUTTON; }
	};

	/** \class Int_Setting
    *   \brief Trida reprezentujici nastaveni typu int.
    */
	class Int_Setting : public Setting
	{
	private:

		int value; //!<aktualne nastavena hodnota
		string units; //!<jednotky (pokud nastaveni nejake ma, jinak je to prazdny retezec) pro vypis do souboru a v hernim menu
		int min; //!<maximalni povolena hodnota nastaveni
		int max; //!<minimalni povolena hodnota nastaveni
		
	public:

		//! \brief Defaultni konstruktor, nic nedela.
		Int_Setting() {}

		/** \brief Nastavi label, jednotky a hodnotu.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
		*   \param units Jednotky (pokud nastaveni nejake ma, jinak je to prazdny retezec) pro vypis do souboru a v hernim menu.
		*   \param value Hodnota, ktera se ma nastavit.
        */
		Int_Setting(string label, string units, int value = 0) : Setting(label), units(units), min(INT_MIN), max(INT_MAX), value(value) {}

		/** \brief Nastavi label, jednotky, minimalni a maximalni hodnotu a  aktualni hodnotu.
		*   \param label Oznaceni daneho parametru, ktere ho identifikuje v konfiguracnim souboru (tvar "Oznaceni:").
		*   \param units Jednotky (pokud nastaveni nejake ma, jinak je to prazdny retezec) pro vypis do souboru a v hernim menu.
		*   \param min Minimalni povolena hodnota nastaveni.
		*   \param max Maximalni povolena hodnota nastaveni.
		*   \param value Hodnota, ktera se ma nastavit. Musi byt mezi min. a max., jinak se nastavi min. nebo max.
        */
		Int_Setting(string label, string units, int min, int max, int value = 0) : Setting(label), units(units), min(min), max(max) { Set_Value(value); }

		//! \brief Vrati aktualne nastavenou hodnotu.
		int Get_Value() const { return value; }

		//! \brief Vrati jednotky (pokud nastaveni nejake ma, jinak je to prazdny retezec)
		string Get_Units() const { return units; }

		//! \brief Vrati minimalni moznou hodnotu.
		int Get_Min() const { return min; }

		//! \brief Vrati maximalni moznou hodnotu.
		int Get_Max() const { return max; }

		/** \brief Nastavi hodnotu.
		*   \param value Hodnota, ktera se ma nastavit. Musi byt mezi min. a max., jinak se nastavi min. nebo max.
		*   \return Urcuje, zda se podarilo nastavit pozadovanou hodnotu.
        */
		bool Set_Value(int value);

		//! \brief Urci minimalni moznou hodnotu. Pokud je aktualni hodnota mensi, nastavi se na ni.
		void Set_Min(int min) { this->min = min; Set_Value(value); }

		//! \brief Urci maximalni moznou hodnotu. Pokud je aktualni hodnota vetsi, nastavi se na ni.
		void Set_Max(int max) { this->max = max; Set_Value(value); }
	
	};

	Choice_Setting<bool> bool_settings[NUM_BOOL_SETTINGS];          //!<nastveni typu bool
	Control_Setting key_settings[NUM_PLAYERS][NUM_SHIP_CONTROLS];   //!<nastaveni ovladacich prvku
	Int_Setting int_settings[NUM_INT_SETTINGS];                     //!<nastveni typu int
	Choice_Setting<Color> color_settings[NUM_COLOR_SETTINGS];       //!<nastveni typu barva
	Choice_Setting<SDL_Rect> resolution;                            //!<nastveni rozliseni
	Choice_Setting<players_t> player_settings[NUM_PLAYER_SETTINGS]; //!<nastveni typu hrac
	Choice_Setting<weapons_t> weapon_settings[NUM_WEAPON_SETTINGS]; //!<nastveni typu zbran

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Parametr typu bool.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(bool_settings_t setting) const;

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Parametr typu int.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(int_settings_t setting) const;

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Ovladaci prvek.
	*   \param player Hrac, ktereho je to ovladaci prvek.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(controls_t setting, players_t player) const;

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Parametr typu barva.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(color_settings_t setting) const;

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Parametr typu hrac.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(player_settings_t setting) const; 

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro dany parametr.
	*   \param setting Parametr typu zbran.
	*   \return Radek do konfiguracniho souboru pro dany parametr.
    */
	string Get_Config_Entry(weapon_settings_t setting) const; 

	/** \brief Vrati retezec s radkem do konfiguracniho souboru pro rozliseni.
	*   \return Radek do konfiguracniho souboru pro rozliseni.
    */
	string Get_Resolution_Config_Entry() const;

	/** \brief Prevede textovou reprezentaci hodnoty parametru typu bool na bool.
	*   \param s_value Textova reprezentace hodnoty parametru typu bool
	*   \param value Adresa, kam se ma ulozit vysledna hodnota bool.
	*   \return true, pokud se prevod podaril, jinak false.
    */
	bool String_To_Bool(const string & s_value, bool* value) const;

	//! \brief Prevede bool hodnotu na textovou reprezentaci (pro zapis do souboru a zobrazeni v menu).
	string Bool_To_String(bool value) const { if (value) return "ANO"; else return "NE"; }

	/** \brief Prevede retezec na rozliseni.
	*   \param s_value Textova reprezentace rozliseni.
	*   \param resolution Sem se ulozi vysledne rozliseni do slozek w a h.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Resolution(const string & s_value, SDL_Rect & resolution) const;

	/** \brief Prevede rozliseni na retezec.
	*   \param resolution Rozliseni, pouzivaji se jen slozky w a h.
    */
	string Resolution_To_String(SDL_Rect resolution) const;

	/** \brief Prevede retezec na klavesu.
	*   \param s_value Textova reprezentace klavesy.
	*   \param key Sem se ulozi vysledna klavesa.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Key(const string & s_value, SDLKey* key) const;

	/** \brief Prevede klavesu na retezec.
    *   \param key Klavesa.
    *   \return Textova reprezentace predane klavesy. Prazdny retezec, pokud se prevod nepovede.
    */
	string Key_To_String(SDLKey key) const;

	/** \brief Prevede retezec na barvu.
	*   \param s_value Textova reprezentace barvy.
	*   \param color Sem se ulozi vysledna barva.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Color(const string & s_value, Color & color) const;

	/** \brief Prevede barvu na retezec.
	*   \return Textova reprezentace predane barvy. Prazdny retezec, pokud se prevod nepovede.
    */
	string Color_To_String(const Color & color) const;

	/** \brief Prevede retezec na kod tlacitka mysi podle SDL.
	*   \param s_value Textova reprezentace tlacitka mysi.
	*   \param mouse_button Sem se ulozi vysledny kod tlacitka mysi podle SDL.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Mouse_Button(const string & s_value, int & mouse_button) const; 

	/** \brief Prevede kod tlacitka mysi (podle SDL) na retezec.
	*   \param mouse_button Kod tlacitka mysi (podle SDL).
	*   \return Textova reprezentace predaneho tlacitka mysi. Prazdny retezec, pokud se prevod nepovede.
    */
	string Mouse_Button_To_String(const int & mouse_button) const; 

	/** \brief Prevede retezec na hodnotu z enumerace players_t urcujici hrace.
	*   \param s_value Textova reprezentace hrace.
	*   \param player Sem se ulozi vysledna hodnota urcujici hrace.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Player(const string & s_value, players_t & player) const;

	/** \brief Prevede konstantu z enumerace players_t urcujici hrace na retezec.
	*   \param player Konstanta z enumerace players_t urcujici hrace
	*   \return Textova reprezentace predaneho hrace.
    */
	string Player_To_String(const players_t & player) const { return PLAYER_NAMES[player]; }

	/** \brief Prevede retezec na hodnotu z enumerace weapons_t urcujici zbran.
	*   \param s_value Textova reprezentace zbrane.
	*   \param weapon Sem se ulozi vysledna hodnota urcujici zbran.
	*   \return true, pokud se prevod povedl, jinak false.
    */
	bool String_To_Weapon(const string & s_value, weapons_t & weapon) const;

	/** \brief Prevede konstantu z enumerace weapons_t urcujici zbran na retezec.
	*   \param weapon Konstanta z enumerace weapons_t urcujici zbran.
	*   \return Textova reprezentace predane zbrane.
    */
	string Weapon_To_String(const weapons_t & weapon) const { return WEAPON_NAMES[weapon]; }

	//! \brief Urci minima a maxima parametrum typu int, u nichz jsou tyto hodnoty zavisle na hodnotach jinych parametru.
	void Set_Int_Ranges();

	/** \brief Nastavi parametry v objektu podle udaju v konfiguracnim souboru.
	*   \return Jestli vse probehlo v poradku (nechybi konfiguracni soubor nebo v nem nejsou spatna data).
	*/
	bool Read_Config();

	/** \brief Posle SDL zpravu hlavni smycce aplikace. Pouziva se pro upozorneni, ze se zmenilo nejake nastaveni.
    *   \param code Identifikator zpravy.
    */
	void Send_Event(int code) const; 
};

#endif