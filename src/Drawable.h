
/** \file Drawable.h
*   \brief Obsahuje deklaraci interface Drawable.
*/

#pragma once

#ifndef IDRAWING_H_
#define IDRAWING_H_

/** \class Drawable
*   \brief Interface pro tridy, ktere neco vykresluji.
*/
class Drawable
{
public:

	//! \brief Virtualni destruktor, co nic nedela.
	virtual ~Drawable() {}

    //! \brief Vykreslovaci metoda.
	virtual void Draw() const = 0;
};

#endif