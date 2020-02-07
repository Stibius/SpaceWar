
/** \file Configurable.h
*   \brief Obsahuje deklaraci interface Configurable.
*/

#pragma once

#ifndef ICONFIGURABLE_H_
#define ICONFIGURABLE_H_

#include "Settings.h"

/** \class Configurable
*   \brief Interface pro tridy, ktere maji parametry nastavitelne uzivatelem.
*/
class Configurable
{
public:

	//! \brief Virtualni destruktor, co nic nedela.
	virtual ~Configurable() {}

    //! \brief Nastavi parametry objektu podle predanych nastaveni.
	virtual void Set_Settings(const Settings & settings) = 0;
};

#endif