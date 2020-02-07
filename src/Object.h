
/** \file Object.h
*   \brief Obsahuje deklaraci abstraktni tridy Object.
*/

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Point.h"
#include <SDL_opengl.h>
#include "Drawable.h"

/** \class Object
*   \brief Abstraktni trida, ze ktere jsou odvozeny tridy vsech hernich objektu.
*/
class Object : public Drawable
{
private:

	Point center; //!<pozice stredu objektu
	GLfloat radius; //!<polomer kruznice opsane objektu (pouziva se pri detekci kolizi).

public:

	//! \brief Defaultni konstruktor, nic nedela.
	Object() {};

	//! \param center Pozice stredu objektu.
	Object(const Point & center) { this->center = center; }

	//! \brief Virtualni destruktor, nic nedela.
	virtual ~Object() {};

	//! \brief Zjisti aktualni pozici stredu objektu.
	const Point & Get_Center() const { return center; }

	//! \brief Zjisti aktualni x-ovou souradnici stredu objektu.
	GLfloat Get_Center_X() const { return center.x; }

	//! \brief Zjisti aktualni y-ovou souradnici stredu objektu.
	GLfloat Get_Center_Y() const { return center.y; }

	//! \brief Vrati polomer kruznice opsane objektu v pixelech (pouziva se pri detekci kolizi).
	GLfloat Get_Radius() const { return radius; }

	//! \brief Nastavi x-ovou souradnici stredu objektu.
	void Set_Center_X(GLfloat x) { center.x = x; }

	//! \brief Nastavi y-ovou souradnici stredu objektu.
	void Set_Center_Y(GLfloat y) { center.y = y; }

	//! \brief Nastavi pozici stredu objektu.
	void Set_Center(const Point & point) { center = point; }	

	/** \brief Nastavi polomer kruznice opsane objektu (pouziva se pri detekci kolizi).
	*  \param radius Polomer v pixelech. Pokud je zaporny, je vynasoben -1.
	*/
	void Set_Radius(GLfloat radius);

	//! \brief Vykresleni.
	virtual void Draw() const = 0;
};

#endif