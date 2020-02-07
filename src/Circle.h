
/** \file Circle.h
*   \brief Obsahuje deklaraci tridy Circle.  
*/

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <SDL_opengl.h>
#include "Color.h"
#include "Object.h"
#include "Point.h"

/** \class Circle
*   \brief Trida reprezentujici kruhy a kruznice. Umoznuje vykresleni kruznice, vyplneneho kruhu nebo jejich casti.
*/
class Circle : public Object
{
private:

	bool filled;     //!<urcuje, zda jde o vyplneny kruh
	Color color;     

public:

	//! \brief Defaultni konstruktor, nic nedela.
	Circle() {}; 

	/** \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1. 
	*   \param filled Urcuje, zda ma byt kruznice vyplnena barvou.
    */
	Circle(GLfloat radius, const Point & center, const Color & color = Color(), bool filled = false) 
		: Object(center), color(color), filled(filled) { Set_Radius(radius); }

	//! \brief Vrati barvu kruznice/kruhu.
	const Color & Get_Color() const { return color; }

	//! \brief Zjisti, zda je kruznice vyplnena.
	bool Is_Filled() const { return filled; }

	//! \brief Nastavi barvu kruznice/kruhu.
	void Set_Color(const Color & color) { this->color = color; }

	//! \brief Nastavi vyplneni kruznice/kruhu.
	void Set_Fill(bool filled) { this->filled = filled; }

	//! \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	void Set_Radius(GLfloat radius) { Object::Set_Radius(radius); }

	/** \brief Vykresleni cele kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. Bere se v potaz i transformacni matice aktualne nastavena v OpenGL. 
	*   \param center_alpha Hodnota pruhlednosti ve stredu kruhu.
	*   \param edge_alpha Hodnota prihlednosti na obvodu kruhu.
    */
	virtual void Draw() const;

	/** \brief Vykresleni cele kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. Bere se v potaz i transformacni matice aktualne nastavena v OpenGL. 
	*   \param center_alpha Hodnota pruhlednosti ve stredu kruhu.
	*   \param edge_alpha Hodnota prihlednosti na obvodu kruhu.
    */
	void Draw(GLfloat center_alpha, GLfloat edge_alpha) const;

	/** \brief Vykresleni casti kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param angle1 Uhel, od ktereho se ma zacit vykreslovat, ve stupnich.
	*   \param angle2 Uhel, kde se ma skoncit s vykreslovanim, ve stupnich.
    */
	void Draw_Arc(GLfloat angle1, GLfloat angle2) const;

	/** \brief Vykresleni casti kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param angle1 Uhel, od ktereho se ma zacit vykreslovat, ve stupnich.
	*   \param angle2 Uhel, kde se ma skoncit s vykreslovanim, ve stupnich.
	*   \param center_alpha Hodnota pruhlednosti ve stredu kruhu.
	*   \param edge_alpha Hodnota prihlednosti na obvodu kruhu.
    */
	void Draw_Arc(GLfloat angle1, GLfloat angle2, GLfloat center_alpha, GLfloat edge_alpha) const;

	/** \brief Staticka metoda pro vykresleni cele kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param center Pozice stredu kruznice/kruhu (bere se v potaz i transformacni matice aktualne nastavena v OpenGL)
	*   \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	*   \param filled Urcuje, zda ma byt kruznice vyplnena barvou, ktera je aktualne nastavena v OpenGL.
    */
	static void Draw(const Point & center, GLfloat radius, bool filled = false);

	/** \brief Staticka metoda pro vykresleni cele kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param center Pozice stredu kruznice/kruhu (bere se v potaz i transformacni matice aktualne nastavena v OpenGL)
	*   \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	*   \param center_alpha Hodnota pruhlednosti ve stredu kruhu.
	*   \param edge_alpha Hodnota prihlednosti na obvodu kruhu.
    */
	static void Draw(const Point & center, GLfloat radius, const Color & color, GLfloat center_alpha, GLfloat edge_alpha);

	/** \brief Staticka metoda pro vykresleni casti kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param center Pozice stredu kruznice/kruhu (bere se v potaz i transformacni matice aktualne nastavena v OpenGL)
	*   \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	*   \param angle1 Uhel, od ktereho se ma zacit vykreslovat, ve stupnich.
	*   \param angle2 Uhel, kde se ma skoncit s vykreslovanim, ve stupnich.
	*   \param filled Urcuje, zda ma byt kruznice vyplnena barvou, ktera je aktualne nastavena v OpenGL.
    */
	static void Draw_Arc(const Point & center, GLfloat radius, GLfloat angle1, GLfloat angle2, bool filled = false);

	/** \brief Staticka metoda pro vykresleni casti kruznice/kruhu. Je pouzita metoda vykresleni kruznice jako N-uhelniku. 
	*   \param center Pozice stredu kruznice/kruhu (bere se v potaz i transformacni matice aktualne nastavena v OpenGL)
	*   \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	*   \param angle1 Uhel, od ktereho se ma zacit vykreslovat, ve stupnich.
	*   \param angle2 Uhel, kde se ma skoncit s vykreslovanim, ve stupnich.
	*   \param center_alpha Hodnota pruhlednosti ve stredu kruhu.
	*   \param edge_alpha Hodnota prihlednosti na obvodu kruhu.
    */
	static void Draw_Arc(const Point & center, GLfloat radius, GLfloat angle1, GLfloat angle2, const Color & color, GLfloat center_alpha, GLfloat edge_alpha);
};

#endif