
/** \file Font.h
*   \brief Obsahuje deklaraci tridy Font.
*/

#ifndef FONT_H_
#define FONT_H_

#include <string>
#include <SDL_ttf.h>
#include <vector>
#include <SDL_opengl.h>
#include "Texture.h"
#include "Color.h"
#include "Point.h"
#include <map>

using std::map;
using std::string;
using std::vector;

/** \class Font
*   \brief Trida pro praci s textem.
*/
class Font 
{
private:

	struct DL_map //!<reprezentuje mapu uchovavajici cisla display-listu asociovana s jednotlivymi znaky
	{
		map<Uint16, GLuint> display_lists; //!<key: znak, value: display-list
		vector<Texture> textures; //!<uchovava textury se znaky, tyto textury jsou vykreslene v display-listech
		int uses; //!<pocet objektu tridy Font pouzivajicich tuto DL_map
	};

	static const string czech_characters; //!<obsahuje vsechny ceske znaky.
    static const char ASCII_FIRST = 32;   //!<hodnota prvniho znaku z ASCII tabulky, ktery je pouzit
    static const char ASCII_LAST = 126;   //!<hodnota posledniho znaku z ASCII tabulky, ktery je pouzit
	static const int NO_MAP = -1;         //!<signalizuje, ze objekt nema prirazenou zadnou mapu

	static int num_instances; //!<pocet instanci tridy

	Color color; //!<barva fontu pri vypisovani textu

	TTF_Font* font;           //!<nahrany font
	string font_path;         //!<cesta k souboru s nahranym fontem
	GLfloat scale;            //!<pomer vyska pisma v pixelech / point size nacteneho fontu
	
	/** \brief Ziska vsechny DL_map vsech instanci tridy Font.
	*   \return key: kazda instance tridy Font s nahranym fontem ma svuj klic, value: DL_map uchovavajici cisla display-listu asociovana s jednotlivymi znaky
    */
	static map<int, DL_map> & Get_Maps(); 

	int map_key; //!<klic do mapy vracene metodou Get_Maps(), s pomoci ktereho se ziska DL_map, ktera patri k teto instanci

	/** \brief Prevede retezec na vektor identifikatoru display-listu pro jeho znaky ve stejnem poradi.
	*   \return Vektor identifikatoru display-listu odpovidajicich znakum v retezci ve stejnem poradi. Prazdny vektor pokud neni nahrany zadny font. Pokud se nektere ze znaku v mape nenachazi, bude na jejich miste 0.
	*/
	vector<GLuint> Characters_To_Lists(const string & characters) const;

	//! \brief Vrati vektor UTF8 kodu znaku v poradi, v jakem jsou v predanem retezci.
	static vector<Uint16> Get_UTF8_Codes(const string & text);

	/** \brief Vykresli zadany znak nahranym fontem do textury, tu pak vykresli do display-listu a vytvori se patricna polozka v predane DL_map.
	*   \return Jestli se to povedlo.
	*/
	bool Create_Texture(Uint16 character, DL_map & map) const;

	/** \brief Vykresli kazdy pouzivany znak nahranym fontem do sve textury, ty pak vykresli do display-listu a vytvori DL_map.
	*   \return Jestli se to povedlo.
	*/
	bool Create_Textures(); 

	//! \brief Vymaze DL_map a uvolni display-listy a font, pokud neexistuje jiny objekt, ktery je pouziva.
	void Close_Font();

public:

	//! \brief Defaultni konstruktor, nastavi defaultni hodnoty.
	Font() : font(NULL), scale(1.0f), map_key(NO_MAP) { num_instances++; }

	/** \brief Nastavi defaultni hodnotu barvy, nahraje zadany font a pripravi ho k pouziti.
	*   \param path Cesta k souboru s fontem.
	*   \param size Pozadovana vyska fontu v pixelech.
	*/
	Font(const string & path, int size);

	//! \brief Kopirovaci konstruktor, nove vytovreny objekt bude pouzivat stejnou DL_map jako puvodni, aby se nemusely vytvaret nove display-listy delajici to same.
	Font(const Font & text);

	//! \brief Zavola metodu Close_Font().
	~Font();

	//! \brief Objekt, do ktereho je prirazovano, bude pouzivat stejnou DL_map jako puvodni, aby se nemusely vytvaret nove display-listy delajici to same.
	Font & operator=(const Font & text);

	//! \brief Vrati barvu fontu.
	const Color & Get_Color() const { return color; }

	//! \brief Vrati cestu k souboru s nahranym fontem. Prazdny retezec, pokud neni nahrany zadny font.
    string Get_Font_Path() const { return font_path; }
    
	//! \brief Zjisti maximalni vysku vykreslovanych znaku v pixelech. -1 pri chybe.
	GLfloat Get_Font_Height() const;

	//! \brief Zjisti sirku zadaneho textu pri aktualnim nastaveni v pixelech. -1 pri chybe.
	GLfloat Get_Text_Width(const string & text) const;

	/** \brief Vrati sirku zadaneho textu v pixelech pri zadane velikosti zadaneho fontu v pixelech, -1 pri chybe.
	*   \param path Cesta k souboru s fontem.
	*   \param size Vyska fontu v pixelech.
	*   \param text Text, jehoz sirka se zjistuje.
	*/
	static GLfloat Get_Text_Width(const string & path, int size, const string & text);

	/** \brief Zjisti vysku zadaneho textu pri aktualnim nastaveni v pixelech. -1 pri chybe.
	*   \return Vyska zadaneho textu pri aktualnim nastaveni v pixelech. -1 pri chybe.
	*/
	GLfloat Get_Text_Height(const string & text) const;

	/** \brief Nahraje zadany font a vytvori z nej textury jednotlivych znaku, display-listy vykreslujici tyto textury a prislusnou DL_map.
	*   \param path Cesta k souboru s fontem.
	*   \param size Pozadovana vyska fontu v pixelech.
	*   \return Zda se to povedlo.
	*/
	bool Load_Font(const string & path, int size);

	//! \brief Nastavi barvu fontu.
	void Set_Color(const Color & color) { this->color = color; };

	//! \brief Zmeni velikost fontu na zadanou velikost v pixelech, vsechny znaky se znovu nactou ze souboru s novou velikosti a vykresli.
	void Change_Height(int font_height);

	//! \brief Zmeni velikost fontu na zadanou velikost v pixelech, pouze se bude pri vykreslovani textur se znaky menit meritko.
	void Scale_Font(GLfloat fh) { scale *= fh / Get_Font_Height(); }

	/** \brief Vykresli zadany text zadanym fontem do zadane textury.
	*   \param path Cesta k souboru s fontem.
	*   \param size Pozadovana vyska fontu v pixelech.
	*   \param text Vykreslovany text.
	*   \param texture Textura, kam se ma text vykreslit.
	*   \return true pri uspechu, jinak false.
	*/
	static bool Create_Texture(const string & path, int size, string text, Texture & texture);

	/** \brief Vykresli zadany text na obrazovku na zadane misto. Bere se v potaz i transformacni matice aktualne nastavena v OpenGL. Pouzije se prave nastavena barva.
	*   \param position Pozice leveho horniho rohu vykreslovaneho textu. Bere se v potaz i transformacni matice aktualne nastavena v OpenGL.
	*/
	void Draw_Text(const string & text, const Point & position) const;

	/** \brief Vykresli zadany text na obrazovku na zadane misto (bere se v potaz i transformacni matice aktualne nastavena v OpenGL) zadanou barvou, na kterou se nastavi i objekt.
	*   \param position Pozice leveho horniho rohu vykreslovaneho textu. Bere se v potaz i transformacni matice aktualne nastavena v OpenGL.
	*/
	void Draw_Text(const string & text, const Point & position, const Color & color);

};

#endif