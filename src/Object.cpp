
/** \file Object.cpp
*   \brief Obsahuje definice metod abstraktni tridy Object.
*/

#include "Object.h"

void Object::Set_Radius(GLfloat radius)
{
	this->radius = radius;
	if (this->radius < 0.0f) this->radius *= -1;
}



