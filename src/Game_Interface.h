
/** \file Game_Interface.h
*   \brief Obsahuje deklaraci tridy Game_Interface.
*/

#ifndef GAME_INTERFACE_H_
#define GAME_INTERFACE_H_

#include "Player.h"
#include <SDL_opengl.h>
#include "Font.h"

/** \class Game_Interface
*   \brief Trida reprezentujici herni graficke uzivatelske rozhrani, ktere zobrazuje aktualni stav lodi a skore.
*/
class Game_Interface : public Drawable, public Configurable
{
private:

	static const int FPS_RATE = 500; //!<urcuje delku intervalu, po kterych se vypisuje nova hodnota fps, v milisekundach

	static const GLfloat BAR_WIDTH; //!<sirka obdelnikovych ukazatelu hodnot zivota, stitu a energie 
	static const GLfloat BAR_HEIGHT; //!<vyska obdelnikovych ukazatelu hodnot zivota, stitu a energie
	static const GLfloat POWERUP_INDICATOR_RADIUS;  //!<polomer kruhovych ukazatelu zbyvajiciho casu bonusu

	static const GLfloat TOP_MARGIN;          //!<vzdalenost GUI od horniho okraje obrazovky
	static const GLfloat LEFT_RIGHT_MARGIN;   //!<vzdalenost GUI od leveho a praveho okraje obraovky
	static const GLfloat SYMBOL_WIDTH;        //!<sirka symbolu (zivot, stit, energie) 
	static const GLfloat SYMBOL_BAR_GAP;      //!<mezera mezi symbolem (zivot, stit, energie) a prislusnym obdelnikovym ukazatelem
	static const GLfloat BARS_VERTICAL_GAP;   //!<mezera mezi obdelnikovymi ukazateli pod sebou
	static const GLfloat BARS_POWERUP_INDICATOR_GAP; //!<mezera mezi casti GUI zobrazujici ukazatele zivota, stitu a energie a indikatory zbyvajiciho casu bonusu
	static const GLfloat POWERUP_INDICATORS_GAP; //!<vertikalni mezera mezi indikatory zbyvajiciho casu bonusu, pokud je jich vic

	static const GLfloat SYMBOL_BORDER_ALPHA; //!<alfa slozka obrysu znaku zivota, stitu a energie
	static const GLfloat SYMBOL_INSIDE_ALPHA; //!<alfa slozka vnitrku znaku zivota, stitu a energie
	static const GLfloat BAR_BORDER_ALPHA;  //!<alfa slozka obrysu obdelnikovych ukazatelu
	static const GLfloat BAR_INSIDE_ALPHA;  //!<alfa slozka vnitrku obdelnikovych ukazatelu
	static const GLfloat BORDERS_LINE_WIDTH; //!<sirka car tvoricich obrysy
	static const GLfloat POWERUP_INDICATOR_CIRLCE_LINE_WIDTH; //!<tloustka kruznice u indikatoru zbyvajiciho casu bonusu

	static int num_instances; //!<aktualni pocet instanci tridy
	static bool initialized;  //!<urcuje, zda jsou vykreslene display-listy a inicializovany font

	SDL_Rect resolution; //!<rozliseni obrazovky ve slozkach w a h
	bool show_fps;       //!<udava, zda se ma zobrazovat fps

	static Font score_font; //!<font, kterym se vypisuje skore
	static Font fps_font;   //!<font, kterym se vypisuje fps
	static Texture triple_powerup_indicator_text; //!<textura s vypsanym textem, ktery se vypisuje v bonusu trojite strely, pro zobrazeni indikatoru zbyvajiciho casu

	static const Player* players[Settings::NUM_PLAYERS]; //!<odkazy na hrace, jjeichz atributy se zobrazuji
	
	static GLuint health_symbol_list; //!<id display-listu s vykreslenym znakem zivota
	static GLuint shield_symbol_list; //!<id display-listu s vykreslenym znakem stitu
	static GLuint energy_symbol_list; //!<id display-listu s vykreslenym znakem energie
	static GLuint guided_powerup_indicator_list; //!<id display-listu s vykreslenym znakem bonusu navadene strely pro dany indikator zbyvajiciho casu
    static GLuint triple_powerup_indicator_list; //!<id display-listu s vykreslenym textem bonusu trojite strely pro dany indikator zbyvajiciho casu
	static GLuint bar_list; //!<id display-listu s vykreslenym obrysem obdelnikoveho ukazatele pro zobrazeni hodnot zivota, stitu a energie

	//! \brief Vytvori display-list se znakem zivota a priradi jeho id do prislusne tridni promenne.
	void Draw_Health_Symbol_List();

	//! \brief Vytvori display-list se znakem stitu a priradi jeho id do prislusne tridni promenne.
	void Draw_Shield_Symbol_List();

	//! \brief Vytvori display-list se znakem energie a priradi jeho id do prislusne tridni promenne.
	void Draw_Energy_Symbol_List();

	//! \brief Vytvori display-list se znakem bonusu navadene strely pro indikator zbyvajiciho casu a priradi jeho id do prislusne tridni promenne.
	void Draw_Guided_Powerup_Indicator_List();

	//! \brief Vytvori display-list s textem bonusu trojite strely pro indikator zbyvajiciho casu a priradi jeho id do prislusne tridni promenne.
	void Draw_Triple_Powerup_Indicator_List();

	//! \brief Vytvori display-list s obrysem obdelnikoveho ukazatele (zivot, stit, energie) a priradi jeho id do prislusne tridni promenne.
	void Draw_Bar_List();

	/** \brief Vykresli obdelnikovy ukazatel (zivota, stitu nebo energie) s danou vyplnenou casti.
	*   \param point Pozice leveho horniho rohu ukazatele.
	*   \param color Barva ukazatele, alfa slozka se ignoruje, funkce pouziva pri vykresleni predem dane hodnoty alfa slozky.
	*   \param portion Cast ukazatele, ktera ma byt vyplnena, mela by byt v rozsahu 0.0 - 1.0. Jinak se do tohoto rozsahu prevede.
	*/
	void Draw_Filled_Bar(const Point & point, const Color & color, GLfloat portion) const;

	/** \brief Vykresli kruhovy ukazatel zbyvajiciho casu bonusu navadene strely.
	*   \param point pozice stredu ukazatele.
	*   \param color Barva ukazatele, alfa slozka se nepouziva.
	*   \param portion Cast casu, ktery jeste zbyva, podle toho se vykresli adekvatni cast kruznice.
	*/
	void Draw_Guided_Powerup_Indicator(const Point & point, const Color & color, GLfloat portion) const;

	/** \brief Vykresli kruhovy ukazatel zbyvajiciho casu bonusu trojite strely.
	*   \param point pozice stredu ukazatele.
	*   \param color Barva ukazatele, alfa slozka se nepouziva.
	*   \param portion Cast casu, ktery jeste zbyva, podle toho se vykresli adekvatni cast kruznice.
	*/
	void Draw_Triple_Powerup_Indicator(const Point & point, const Color & color, GLfloat portion) const;

	//! \brief Vykresli cast GUI zobrazujici kruhove ukazatele zbyvajiciho casu bonusu pro daneho hrace.
	void Draw_Powerup_Indicators(const Player & player) const;

	//! \brief Vykresli cast GUI zobrazujici aktualni skore.
	void Draw_Score() const;

	//! \brief Vykresli cast GUI zobrazujici obdelnikove ukazatele (zivot, stit, energie) pro daneho hrace.
	void Draw_Bars(const Player & player) const;
		
public:

	//! \brief Defaultni konstruktor, inkrementuje pocet instanci a nastavi odkazy na hrace na NULL.
	Game_Interface() { num_instances++; }

	/** \brief Inicializuje objekt a inkrementuje pocet instanci, vytvori display-listy, pokud jeste nejsou vytvorene.
	*   \param player1 Prvni hrac, objekt si uchova ukazatel na nej, aby mohl vykreslovat jeho atributy.
	*   \param player2 Druhy hrac, objekt si uchova ukazatel na nej, aby mohl vykreslovat jeho atributy.
	*   \param settings Objekt s hernim nastavenim, podle ktereho se nastavi nektere parametry.
	*/
	Game_Interface(const Player & player1, const Player & player2, const Settings & settings);

	//! \brief Kopirovaci konstruktor, inkrementuje pocet instanci.
	Game_Interface(const Game_Interface & game_interface) : resolution(game_interface.resolution), show_fps(game_interface.show_fps) { num_instances++; }

	//! \brief Dekrementuje pocet instanci a maze display-listy, pokud slo o posledni instanci.
	~Game_Interface();	

	//! \brief Vykresleni GUI.
	virtual void Draw() const;

	//! \brief Vykresli cast GUI zobrazujici fps, pokud je nastaveno, ze se ma zobrazovat.
	void Draw_FPS(GLfloat fps) const;

	//! \brief Aktualizuje nastavitelne parametry.
	virtual void Set_Settings(const Settings & settings);
};

#endif