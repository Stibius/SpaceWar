
/** \file Menu.h
*   \brief Obsahuje deklaraci tridy Menu.
*/

#ifndef MENU_H_
#define MENU_H_

#include "Color.h"
#include "Events.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "Settings.h"
#include <string>
#include "Font.h"
#include <vector>
#include "Drawable.h"
#include "Configurable.h"

using std::string;
using std::vector;

/** \class Menu
*   \brief Trida reprezentujici herni menu. 
*/
class Menu : public Drawable, public Configurable
{
private:

	//! \brief Symbolicka jmena pro sekce menu.
    enum sections 
	{
		FIRST_SECTION,        //!<prvni sekce menu
		MAIN = FIRST_SECTION, //!<sekce Hlavni menu       
	    PAUSED,               //!<sekce Hra pozastavena
        SETTINGS,             //!<sekce Nastaveni
		VIDEO_SETTINGS,       //!<sekce Zobrazeni
		CONTROLS,             //!<sekce Ovladani
		GAME_SETTINGS,        //!<sekce Herni nastaveni
		POWERUPS_SETTINGS,    //!<sekce Nastaveni powerupu
		QUIT_PROGRAM,         //!<sekce Opravdu ukoncit program? 
		QUIT_GAME,            //!<sekce Opravdu ukoncit hru?
		LAST_MENU_SECTION = QUIT_GAME, //!<posledni sekce menu
		NUM_MENU_SECTIONS,    //!<pocet sekci menu
		NO_SECTION = NUM_MENU_SECTIONS //!<zadna sekce
    };

	static const GLfloat HORIZONTAL_MARGINS; //!<minimalni vzdalenost polozek od leveho a praveho okraje obrazovky
	static const GLfloat VERTICAL_MARGINS;   //!<minimalni vzdalenost polozek od horniho a spodniho okraje obrazovky

	/** \class Screen
    *   \brief Trida reprezentujici jednu obrazovku herniho menu.
    */
	class Screen
	{
	public:

		/** \class Item
        *   \brief Abstraktni trida reprezentujici polozku herniho menu, odvozuji se z ni ruzne typy polozek. 
        */
		class Item
	    {
	    private:
		    string name; //!<text polozky, v pripade polozky typu nastaveni je to jen cast pred hodnotou: "Nazev nastaveni:"
		    Point position; //!<pozice leveho horniho okraje textu polozky
	    public:

	        /** \param name text polozky, v pripade polozky typu nastaveni je to jen cast pred hodnotou: "Nazev nastaveni:"
	        *   \param position Pozice leveho horniho okraje textu polozky.
            */
		    Item(const string & name, const Point & position = Point(0.0f, 0.0f)) 
				: name(name), position(position) {}

	        //! \brief Vrati text polozky, v pripade polozky typu nastaveni je to jen cast pred hodnotou: "Nazev nastaveni:"Text polozky.
		    string Get_Name() const { return name; }

	        //! \brief Vrati pozici leveho horniho okraje textu polozky.
		    const Point & Get_Position() const { return position; }

	        //! \brief Vrati sirku textu polozky v pixelech, v pripade polozky typu nastaveni je to jen cast pred hodnotou: "Nazev nastaveni:"
		    virtual GLfloat Get_Width() const = 0;

	        //! \brief Vrati vysku fontu polozky v pixelech.
		    virtual GLfloat Get_Height() const = 0; 
	
	        //! \brief Nastavi text polozky.
		    void Set_Name(const string & name) { this->name = name; }

	        //! \brief Nastavi pozici leveho horniho okraje textu polozky.
		    void Set_Position(const Point & position) { this->position = position; }

			//! \brief Vykresleni polozky.
		    virtual void Draw() const = 0;
	    };

		/** \class Head_Item
        *   \brief Trida reprezentujici nadpisy sekci a podeskci v hernim menu. 
        */
	    class Head_Item : public Item
	    {
	    public:

			//! \brief Symbolicke konstanty reprezentujici urovne nadpisu.
		    enum levels 
			{ 
				FIRST_LEVEL, //!<prvni uroven nadpisu
				MAIN_HEADING = FIRST_LEVEL, //!<nejvetsi napis s nazvem hry v hlavni obrazovce menu
				HEADING, //!<nadpis sekce
				SUBHEADING,  //!<nadpis podsekce
				LAST_LEVEL = SUBHEADING, 
				NUM_LEVELS   //!<pocet urovni nadpisu
			};

	        /** \param name Text nadpisu.
			*   \param level Uroven nadpisu.
	        *   \param position Pozice leveho horniho okraje textu nadpisu.
            */
		    Head_Item(const string & name, levels level, const Point & position = Point(0.0f, 0.0f)) 
				: Item(name, position), level(level), diff_color(NULL) {}

	        /** \param name Text nadpisu.
			*   \param level Uroven nadpisu.
			*   \param color Barva, pokud ma byt jina nez ta nastavena pro vsechny nadpisy dane urovne.
	        *   \param position Pozice leveho horniho okraje textu nadpisu.
            */
			Head_Item(const string & name, levels level, const Color & color, const Point & position = Point(0.0f, 0.0f)) 
				: Item(name, position), level(level), diff_color(new Color(color)) {}
			
	        //! \brief Vrati uroven nadpisu.
		    levels Get_Level() const { return level; }

			//! \brief Vrati vysku fontu dane urovne nadpisu v pixelech.
		    static GLfloat Get_Level_Height(levels level);

			//! \brief Vrati barvu textu dane urovne nadpisu.
		    static const Color Get_Level_Color(levels level) { return texts[level].Get_Color(); }

	        //! \brief Vrati sirku textu nadpisu v pixelech.
		    virtual GLfloat Get_Width() const;

	        //! \brief Vrati vysku fontu nadpisu v pixelech.
		    virtual GLfloat Get_Height() const;

	        //! \brief Nastavi uroven nadpisu.
		    void Set_Level(levels level) { this->level = level; }

	        //! \brief Nastavi vysku fontu nadpisu dane urovne v pixelech.
		    static void Set_Level_Height(int height, levels level);

			//! \brief Nastavi barvu konkretni polozce. Muze byt jina nez nastavena barva pro vsechny nadpisy dane urovne.
			void Set_Color(const Color & color) { diff_color = new Color(color); }

			//! \brief Nastavi barvu pro danou uroven nadpisu.
		    static void Set_Level_Color(const Color & color, levels level) { texts[level].Set_Color(color); }

			/** \brief Nastavi font pro danou uroven nadpisu.
			*   \param path Cesta k souboru s fontem.
	        *   \param height Vyska fontu v pixelech.
			*   \param level Uroven nadpisu.
            */
		    static void Set_Level_Font(const string & path, int height, levels level);
		
			//! \brief Vykresleni nadpisu.
		    virtual void Draw() const;

	    private:

		    static Font texts[NUM_LEVELS]; //!<fonty pouzivane pro vypisovani nadpisu jednotlivych urovni
			Color* diff_color; //!<barva, pokud ma byt odlisna od standardni barvy dane urovne nadpisu, pokud neni, je NULL
		    levels level; //!<uroven nadpisu
	    };

		/** \class Interactive_Item
        *   \brief Abstraktni trida reprezentujici interaktivni polozky menu.
        */
	    class Interactive_Item : public Item
	    {
	    public:

	        /** \param name Text polozky, v pripade polozky typu nastaveni je to jen cast pred hodnotou: "Nazev nastaveni:"
	        *   \param position Pozice leveho horniho okraje textu polozky.
            */
		    Interactive_Item(const string & name, const Point & position = Point(0.0f, 0.0f)) 
				: Item(name, position) {}

			//! \brief Vrati ukazatel na prave oznacenou polozku.
		    static Interactive_Item* Get_Selected_Item() { return selected_item; }

			//! \brief Zjisti, zda je tato polozka prave oznacena.
		    bool Is_Selected() const { return (this == selected_item); }

	        //! \brief Vrati ukazatel na prave editovanou polozku, NULL pokud neni zrovna zadna editovana.
		    static Interactive_Item* Get_Edited_Item() { if (edited) return selected_item; else return NULL; }

	        //! \brief Zjisti, zda je tato polozka prave editovana.
		    bool Is_Edited() const { return (this == selected_item) && edited; }

			//! \brief Oznaci tuto polozku.
		    void Select() { selected_item = this; edited = false; }

			//! \brief Vykresleni polozky.
		    virtual void Draw() const = 0;
	
	    protected:

			//! \brief Oznaci tuto polozku a prepne ji do stavu editace.
            void Edit_H() { Select(); edited = true; }

			//! \brief Prepnuti polozky zpet ze stavu editace.
		    void Stop_Editing_H() { edited = false; }

	    private:

	 	    static Interactive_Item* selected_item; //!<ukazatel na prave oznacenou polozku
		    static bool edited; //!<urcuje, zda je nejaka polozka prave ve stavu editace
	    };

		/** \class Link_Item
        *   \brief Trida reprezentujici polozky v menu, ktere odkazuji do jeho jinych sekci.
        */
	    class Link_Item : public Interactive_Item
	    {
	    public:

			//! \brief Symbolicke konstanty reprezentujici mozne stavy Link_Itemu.
		    enum states 
			{ 
				NORMAL, //!<polozka neni oznacena
				SELECTED, //!<polozka je oznacena
				NUM_STATES //!<pocet moznych stavu Link_Itemu
			};

	        /** \param name Text polozky.
			*   \param linked_section Sekce menu, na kterou polozka odkazuje.
			*   \param evt Event, ktery se ma poslat, pokud je na polozku kliknuto.
	        *   \param position Pozice leveho horniho okraje textu polozky.
            */
		    Link_Item(const string & name, sections linked_section, events evt, const Point & position = Point(0.0f, 0.0f)) 
				: Interactive_Item(name, position), linked_section(linked_section), evt(evt) {}

			//! \brief Vrati barvu polozek v danem stavu.
		    static const Color & Get_State_Color(states state) { return colors[state]; }

			//! \brief Vrati vysku fontu polozek v pixelech
		    static GLfloat Get_Height_Static();

			//! \brief Vrati sirku textu polozky v pixelech.
		    virtual GLfloat Get_Width() const;

			//! \brief Vrati vysku fontu polozky v pixelech.
		    virtual GLfloat Get_Height() const;

			//! \brief Vrati sekci menu, na kterou polozka odkazuje.
		    sections Get_Linked_Section() const { return linked_section; }

			//! \brief Vrati event, ktery se ma poslat, pokud je na polozku kliknuto.
		    events Get_Event() const { return evt; }
		
			//! \brief Nastavi barvu polozek v danem stavu.
		    static void Set_State_Color(const Color & color, states state) { colors[state] = color; }

			//! \brief Nastavi vysku fontu polozek v pixelech.
		    static void Set_Height(int height);

			/** \brief Nastavi font polozek.
			*   \param path Cesta k souboru s fontem.
	        *   \param height Vyska fontu v pixelech.
            */
		    static void Set_Font(const string & path, int height);

			//! \brief Nastavi sekci, na kterou polozka odkazuje.
		    void Set_Linked_Section(sections section) { linked_section = section; }

			//! \brief Vykresleni polozky.
		    virtual void Draw() const;

	    private:

		    static Font text; //!<font polozek 
		    static Color colors[NUM_STATES]; //!<barvy polozek v jednotlivych stavech
		    sections linked_section; //!<sekce menu, na kterou polozka odkazuje
		    events evt; //!<Event, ktery se ma poslat, kdyz se na polozku klikne mysi
	    };

		/** \class Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni.
        */
	    class Setting_Item : public Interactive_Item
	    {
	    public:

			//! \brief Symbolicke konstanty reprezentujici mozne stavy Setting_Itemu.
		    enum states 
			{ 
				NORMAL, //!<polozka neni oznacena
				SELECTED, //!<polozka je oznacena
				EDITING, //!<polozka je editovana
				NUM_STATES //!<pocet moznych stavu Link_Itemu
			};

	        //! param position Pozice leveho horniho okraje textu polozky.
		    Setting_Item(const Point & position = Point(0.0f, 0.0f)) 
				: Interactive_Item("", position) {}

	        /** \param name Text polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
	        *   \param position Pozice leveho horniho okraje textu polozky.
            */
			Setting_Item(string name, const Point & position = Point(0.0f, 0.0f)) 
				: Interactive_Item(name, position) {}

			//! \brief Vrati barvu polozek v danem stavu
		    static const Color & Get_State_Color(states state) { return colors[state]; }

			//! \brief Vrati vysku fontu polozek v pixelech.
		    static GLfloat Get_Height_Static();

			//! \brief Vrati sirku textu polozky v pixelech, je to jen cast pred hodnotou: "Nazev nastaveni:"
		    virtual GLfloat Get_Width() const;

			//! \brief Vrati vysku fontu polozky v pixelech.
		    virtual GLfloat Get_Height() const;

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const = 0;

			//! \brief Nastavi barvu polozek v danem stavu.
		    static void Set_State_Color(const Color & color, states state) { colors[state] = color; }

			//! \brief Nastavi vysku fontu polozek v pixelech.
		    static void Set_Height(int height);

			/** \brief Nastavi font polozky.
			*   \param path Cesta k souboru s fontem.
	        *   \param height Vyska textu fontu v pixelech.
            */
			static void Set_Font(const string & path, int height);

			//! Priradi Setting_Itemum ukazatel na objekt s nastavenimi, ze ktereho se budou brat hodnoty pro zobrazeni.
		    static void Set_Settings(Settings & setts) { settings = &setts; };
		    
			//! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key) = 0;

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button) = 0;

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const = 0;

		protected:

			//! \brief Vrati ukazatel na objekt s nastavenimi, ze ktereho se berou hodnoty pro zobrazeni.
			static Settings* Get_Settings() { return settings; } 

			//! Vrati font pouzity pro vypisovani polozek.
			static const Font & Get_Text() { return text; } 

			//! \brief Vrati Barvu polozek.
		    const Color & Get_Color() const; 
	
	    private:

		    static Font text; //!<font polozek 
		    static Color colors[NUM_STATES]; //!<barvy polozek v jednotlivych stavech
		    static Settings* settings; //!<ukazatel na objekt s nastavenimi, ze ktereho se berou hodnoty pro zobrazeni.
	    };

		/** \class Bool_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu bool. 
        */
	    class Bool_Setting_Item : public Setting_Item
	    {
	    public:

	        /** \param setting Nastaveni, ktere znazornuje tato polozka.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Bool_Setting_Item(Settings::bool_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Bool_Setting_Item(Settings::bool_settings_t setting, const string & name, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

			//! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;
	    private:

		    Settings::bool_settings_t setting; //!<Nastaveni, ktere znazornuje tato polozka.
	    };


		/** \class Player_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu hrac.
        */
		class Player_Setting_Item : public Setting_Item
	    {
	    public:

	        /** \param setting Nastaveni, ktere znazornuje tato polozka.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Player_Setting_Item(Settings::player_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Player_Setting_Item(Settings::player_settings_t setting, const string & name, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

			//! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;

	    private:

		    Settings::player_settings_t setting; //!<Nastaveni, ktere znazornuje tato polozka.

	    };

		/** \class Weapon_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu zbran.
        */
		class Weapon_Setting_Item : public Setting_Item
	    {
	    public:

	        /** \param setting Nastaveni, ktere znazornuje tato polozka.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Weapon_Setting_Item(Settings::weapon_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Weapon_Setting_Item(Settings::weapon_settings_t setting, const string & name, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

			//! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;

	    private:

		    Settings::weapon_settings_t setting; //!<Nastaveni, ktere znazornuje tato polozka.

	    };

		/** \class Key_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu ovladaci prvek.
        */
	    class Key_Setting_Item : public Setting_Item
	    {
	    public:

	        /** \param setting Ovladaci prvek, ktery znazornuje tato polozka.
			*   \param player Hrac, ktereho je to ovladaci prvek.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Key_Setting_Item(Settings::controls_t setting, Settings::players_t player, const Point & position = Point(0.0f, 0.0f)) 
			   : Setting_Item(position), setting(setting), player(player) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting, player)); }
			
	        /** \param setting Ovladaci prvek, ktery znazornuje tato polozka.
			*   \param player Hrac, ktereho je to ovladaci prvek.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Key_Setting_Item(Settings::controls_t setting, Settings::players_t player, const string & name, const Point & position = Point(0.0f, 0.0f)) 
				: Setting_Item(name, position), setting(setting), player(player) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

		    //! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;

	    private:

		    Settings::controls_t setting; //!<Ovladaci prvek, ktery znazornuje tato polozka.
			Settings::players_t player;   //!<Hrac, ktereho je to ovladaci prvek.

			void Edit() { Edit_H(); }                  //!<Oznaci tuto polozku a prepne ji do stavu editace.
			void Stop_Editing() { Stop_Editing_H(); }  //!<Prepnuti zpet ze stavu editace.

	    };

		/** \class Int_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu int.
        */
	    class Int_Setting_Item : public Setting_Item
	    {
	    public:

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Int_Setting_Item(Settings::int_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting), show_max(false), value_empty(false) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }

	        /** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param show_max Urcuje, zda se ma za hodnotou zobrazovat: "/ maximalni mozna hodnota"
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Int_Setting_Item(Settings::int_settings_t setting, bool show_max, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting), show_max(show_max), value_empty(false) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }
			
			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Int_Setting_Item(const string & name, Settings::int_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting), show_max(false), value_empty(false) {}

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
			*   \param show_max Urcuje, zda se ma za hodnotou zobrazovat: "/ maximalni mozna hodnota"
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Int_Setting_Item(const string & name, Settings::int_settings_t setting, bool show_max, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting), show_max(show_max), value_empty(false) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

		    //! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;
	    private:

		    Settings::int_settings_t setting; //!<Nastaveni, ktere znazornuje tato polozka.
		    bool value_empty; //!<Udava, zda se misto hodnoty zobrazuje prazdne misto.
		    int current_int_value; //!<Aktualne zobrazovana hodnota nastaveni (kvuli prepisovani pri editaci).
			bool show_max; //!<Urcuje, zda se ma za hodnotou zobrazovat: "/ maximalni mozna hodnota"

		    void Edit(); //!<Oznaci tuto polozku a prepne ji do stavu editace.
		    void Stop_Editing(); //!<Prepnuti zpet ze stavu editace.
	    };

		/** \class Resolution_Item
        *   \brief Trida reprezentujici polozku nastaveni rozliseni. 
        */
	    class Resolution_Item : public Setting_Item
	    {
	    public:

	        //! \param position Pozice leveho horniho okraje textu polozky.
		    Resolution_Item(const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Resolution_Label()); }

			/** \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Resolution_Item(const string & name, const Point & position = Point(0.0f, 0.0f)) 
				: Setting_Item(name, position) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

		    //! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;
	    };

		/** \class Color_Setting_Item
        *   \brief Trida reprezentujici polozky hernich nastaveni typu barva.
        */
		class Color_Setting_Item : public Setting_Item
	    {
	    public:

	        /** \param setting Nastaveni, ktere znazornuje tato polozka.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
		    Color_Setting_Item(Settings::color_settings_t setting, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(position), setting(setting) { if (const Settings* settings = Get_Settings()) Set_Name(settings->Get_Setting_Label(setting)); }

			/** \param setting Nastaveni, ktere znazornuje tato polozka.
			*   \param name Text polozky.
	        *   \param position Pozice leveho horniho okraje textu polozky, je to jen cast pred hodnotou: "Nazev nastaveni:"
            */
			Color_Setting_Item(Settings::color_settings_t setting, const string & name, const Point & position = Point(0.0f, 0.0f)) 
			    : Setting_Item(name, position), setting(setting) {}

			//! \brief Vrati textovou podobu aktualni hodnoty tohoto nastaveni.
		    virtual string Get_Value_Text() const;

		    //! \brief Reakce polozky na stisknuti klavesy pri jejim oznaceni.
		    virtual void Update(SDLKey key);

			/** \brief Reakce polozky na stisknuti tlacitka mysi pri jejim oznaceni.
			*   \param mouse_button Kod stisknuteho tlacitko mysi podle SDL.
            */
			virtual void Update(int mouse_button);

		    //! \brief Vykresleni polozky.
		    virtual void Draw() const;
	    private:

		    Settings::color_settings_t setting; //!<Nastaveni, ktere znazornuje tato polozka.

	    };

		//! \brief Defaultni konstruktor, nic nedela.
		Screen() {}

		/** \param items Vektor polozek, ktere maji byt na teto obrazovce zobrazeny.
		*   \param escape_screen Obrazovka, na kterou se ma prejit pri stisku klavesy ESC.
		*   \param escape_evt Udalost, ktera se ma poslat pri stisku klavesy ESC.
        */
		Screen(vector<Item*> & items, sections escape_screen = NO_SECTION, events escape_evt = NO_EVENT) 
			: items(items), escape_screen(escape_screen), escape_evt(escape_evt) {}

		//! \brief Kopiruje data, na ktera ukazuji ukazatele na jednotlive polozky.
		Screen(const Screen & screen);

		//! \brief Kopiruje data, na ktera ukazuji ukazatele na jednotlive polozky.
		Screen & operator=(const Screen & screen);

		/** \brief Indexovaci operator pro pristup k jednotlivym polozkam obrazovky.
		*   \param i Index, urcuje poradi polozky na obrazovce, kterou chceme ziskat.
		*   \return Ukazatel na prislusnou polozku na obrazovce.
        */
		Item* operator[](unsigned int i);

		//! \brief Destruktor, uvolni alokovane polozky.
		~Screen();

		//! \brief Vrati celkovou vysku vsech itemu teto obrazovky v pixelech tak, jak budou vykreslene.
		GLfloat Get_Items_Height() const;

		//! \brief Vrati rozliseni obrazovky ve slozkach w a h.
		static SDL_Rect Get_Resolution() { return resolution; }

		//! \brief Vrati pocet polozek na teto obrazovce.
		int Get_Items_Count() const { return int(items.size()); }

		/** \brief Nastavi rozmery obrazovky.
		*   \param res Rozmery obrazovky ve slozkach w a h.
        */
		static void Set_Resolution(const SDL_Rect & res) { resolution = res; }

		//! \brief Nastavi pozice jednotlivych polozek na teto obrazovce na zaklade vysky obrazovky a celkove vysky vypisovanych polozek.
		void Set_Items_Positions();

		/** \brief Vlozi SDL udalost do fronty udalosti.
        *   \param code Identifikator udalosti.
        */
		void Send_Event(int code) const;

		//! \brief Aktualizuje obrazovku v zavislosti na predane vertikalni poloze mysi.
		void Update(GLfloat mouse_y);

		/** \brief Reakce na stisk klavesy.
		*   \return Sekce menu, na kterou se ma prepnout, pokud stisk klavesy vyvolal tuto zmenu, NO_SECTION pri zadne zmene.
        */
		sections Update(SDLKey key);

		/** \brief Reakce na stisk tlacitka mysi.
		*   \param mouse_button Kod stisknuteho tlacitka mysi podle SDL.
		*   \return Sekce menu, na kterou se ma prepnout, pokud stisk klavesy vyvolal tuto zmenu, NO_SECTION pri zadne zmene.
        */
		sections Update(int mouse_button);

		//! \brief Vykresleni obrazovky.
		void Draw() const;

	private:

		vector<Item*> items; //!<vektor vsech polozek na teto obrazovce
		sections escape_screen; //!<Obrazovka, na kterou se ma prejit pri stisku klavesy ESC.
		events escape_evt; //!<Udalost, ktera se ma poslat pri stisku klavesy ESC.

		static SDL_Rect resolution;      //!<Rozliseni obrazovky ve slozkach w a h.
		static GLfloat space_ratio;      //!<Pomer vysky mezery pod polozkou a vysky samotne polozky.

		//! \brief Oznaci polozku nad prave oznacenou polozkou. Pokud je prave oznacena prvni polozka, oznaci posledni. Pokud je prave oznacena polozka editovana, neudela nic.
		void Selected_Item_Up();

		//! \brief Oznaci polozku pod prave oznacenou polozkou. Pokud je prave oznacena posledni polozka, oznaci prvni. Pokud je prave oznacena polozka editovana, neudela nic.
		void Selected_Item_Down();

	};

	sections section;     //!<Aktualni sekce menu.
	bool paused;          //!<urcuje, zda je prave hra zapauzovana                       

	Screen screens[NUM_MENU_SECTIONS]; //!<seznam obrazovek menu ve stavu, kdyz neni spustena hra
	Screen screens_paused[NUM_MENU_SECTIONS]; //!<seznam obrazovek menu ve stavu, kdyz je hra spustena a zapauzovana

	//! \brief Vrati referenci na aktualni obrazovku menu.
	Screen & Get_Current_Screen();

	//! \brief Zjisti celkovou vysku vsech polozek te obrazovky, ktera ji ma nejvyssi, v pixelech.
	GLfloat Get_Max_Height() const;

	//! \brief Zjisti sirku nejsirsiho zaznamu v menu na leve strane obrazovky, v pixelech.
	GLfloat Get_Max_Left_Width();

	//! \brief Prejde na danou sekci v menu.
	void Set_Section(sections section);

	//! \brief Nastavi velikosti textu jednotlivych typu polozek tak, aby se vsechny obrazovky menu vesly na obrazovku.
	void Set_Items_Sizes();

public:

	//! \brief Defaultni konstruktor, nic nedela.
	Menu() {}

	//! \param settings Objekt s hernim nastavenim, podle ktereho se nastavi nektere parametry.
	Menu(Settings & settings);

	/** \brief Nastavi pozice polozek menu podle aktualniho rozliseni.
	*   \param resolution Rozliseni obrazovky ve slozkach w a h.
	*/
	void Set_Resolution(const SDL_Rect & resolution);

	//! \brief Aktualizuje menu na zaklade aktualnich nastaveni (barvy lodi).
	virtual void Set_Settings(const Settings & settings);

	//! \brief Vykresli menu.
	virtual void Draw() const;

	//! \brief Aktualizuje menu na zaklade stisknute klavesy.
	void Update(SDLKey key);

	/** \brief Aktualizuje menu na zaklade kodu stisknuteho tlacitka mysi podle SDL.
	*   \param mouse_button Kod stisknuteho tlacitka mysi podle SDL.
    */
	void Update(int mouse_button); 

	//! \brief Aktualizuje menu na zaklade aktualni vertikalni polohy mysi (oznaceni polozky, kam mys ukazuje).
	void Update(GLfloat mouse_y);
};

#endif