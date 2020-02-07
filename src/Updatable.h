
/** \file Updatable.h
*   \brief Obsahuje deklaraci interface Updatable.
*/

#pragma once

#ifndef IUPDATING_H_
#define IUPDATING_H_

#include <SDL_opengl.h>

/** \class Updatable
*   \brief Interface pro tridy, jejichz objekty se aktualizuji pred kazdym prekreslenim obrazovky.
*/
class Updatable
{
public:

	//! \brief Virtualni destruktor, co nic nedela.
	virtual ~Updatable() {}

    /** \brief Aktualizacni funkce, vola se pred kazdym prekreslenim obrazovky.
    *   \param fps Aktualni pocet snimku za sekundu.
    */
	virtual void Update(GLfloat fps) = 0;
};

#endif