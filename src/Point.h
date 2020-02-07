
/** \file Point.h
*   \brief Obsahuje deklaraci tridy Point.
*/

#ifndef POINT_H_
#define POINT_H_

#include <SDL_opengl.h>

/** \class Point
*   \brief Trida reprezentujici bod v rovine pro snadnejsi praci s body.
*/
class Point
{
public:

	GLfloat x;
	GLfloat y;

	explicit Point(GLfloat x = 0.0f, GLfloat y = 0.0f) : x(x), y(y) {};
	bool operator!=(const Point & point2) const { return !(x == point2.x && y == point2.y); }
	bool operator==(const Point & point2) const { return (x == point2.x && y == point2.y); }
};

#endif
