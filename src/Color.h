
/** \file Color.h
*   \brief Obsahuje deklaraci tridy Color.
*/

#ifndef COLOR_H_
#define COLOR_H_

#include <SDL_opengl.h>

/** \class Color
*   \brief Trida reprezentujici barvu pro snadnejsi praci s barvami.
*/
class Color
{
public:

	GLfloat red; 
	GLfloat green; 
	GLfloat blue;  
	GLfloat alpha; 
 
	explicit Color(GLfloat red = 0.0f, GLfloat green = 0.0f, GLfloat blue = 0.0f, GLfloat alpha = 1.0f) 
		: red(red), green(green), blue(blue), alpha(alpha) {}

	//! \brief Operator porovnani pro snadnejsi porovnavani dvou barev. 
	bool operator==(const Color & color) const { return (red == color.red && green == color.green && blue == color.blue && alpha == color.alpha); }

	//! \brief Operator porovnani pro snadnejsi porovnavani dvou barev. 
	bool operator!=(const Color & color) const { return !(red == color.red && green == color.green && blue == color.blue && alpha == color.alpha); }

};

#endif