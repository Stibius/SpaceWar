
/** \file Texture.h
*   \brief Obsahuje deklaraci tridy Texture.
*/

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL_opengl.h>
#include <map>
#include "Point.h"

using std::map;

/** \class Texture
*   \brief Trida reprezentujici OpenGL texturu.
*/
class Texture
{
private:

	GLuint id;      //!<OpenGL identifikator textury
	GLsizei width;  //!<sirka textury v texelech
	GLsizei height; //!<vyska textury v texelech
	GLenum type;    //!<datovy typ pixelovych dat

	static map<int, int> & Get_Num_Instances(); //!<klic: id OpenGL textury, hodnota: pocet instanci tridy Texture pracujicich s touto texturou

public:
	
	//! \brief Defaultni konstruktor, vytvori prazdnou texturu.
	Texture();

	/** \brief Vytvori texturu na zaklade vstupnich parametru.
	*   \param width Sirka textury v texelech.
	*   \param height Vyska textury v texelech.
	*   \param type Datovy typ pixelovych dat.
	*   \param pixels Ukazatel na data obrazku.
	*   \param wrap_s Nastaveni OpenGL parametru GL_TEXTURE_WRAP_S
	*   \param wrap_t Nastaveni OpenGL parametru GL_TEXTURE_WRAP_T
	*/
	Texture(GLsizei width, GLsizei height, GLenum type, const GLvoid *pixels, GLint wrap_s, GLint wrap_t);

	//! \brief Kopirovaci konstruktor, inkrementuje hodnotu poctu instanci tridy pracujicich s danou texturou, nekopiruje samotnou texturu.
	Texture(const Texture & texture);

	//! \brief Destruktor, dekrementuje hodnotu poctu instanci tridy pracujicich s danou texturou, maze texturu, pokud klesl na 0.
	~Texture();

	//! \brief Pretizeny prirazovaci operator, aktualizuje hodnoty poctu instanci tridy pracujicich s danymi texturami, nekopiruje samotnou texturu.
	Texture & operator=(const Texture & texture);

	//! \brief Zjisti sirku textury v texelech.
	GLsizei Get_Width() const { return width; }

	//! \brief Zjisti vysku textury v texelech.
	GLsizei Get_Height() const { return height; }

	/** \brief Vykresli texturu.
	*   \param position Pozice leveho horniho okraje textury.
	*/
	void Draw(const Point & position) const;

};

#endif