
/** \file MovingObject.cpp
*   \brief Obsahuje definice metod abstraktni tridy MovingObject.
*/

#include "Moving_Object.h"
#include "Math.h"

GLfloat Moving_Object::doubles_translations[NUM_DOUBLES][2]; 

void Moving_Object::Change_Direction(GLfloat angle)
{
	GLfloat speed = Get_Speed();
	vel_x = sin(angle*(Math::PI/180)) * speed;   //horizontalni slozka rychlosti v zavislosti na uhlu pohybu
	vel_y = -cos(angle*(Math::PI/180)) * speed;  //vertikalni slozka rychlosti v zavislosti na uhlu pohybu
}

void Moving_Object::Deactivate_Doubles()
{
	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		doubles_activation[i] = false;
	}
}

GLfloat Moving_Object::Get_Motion_Angle() const
{
	if (vel_x == 0 && vel_y == 0) return -1; //objekt se nepohybuje

	GLfloat angle; //uhel v radianech

	if (vel_x == 0.0f) //horizontalni slozka nulova
	{
		if (vel_y > 0)
			angle = Math::PI;   //pohyb svisle dolu
		else if (vel_y < 0)
			angle = 0.0f; //pohyb svisle nahoru
	}
	else if (vel_y == 0)  //vertikalni slozka nulova
	{
		if (vel_x < 0)
			angle = (3.0f / 2.0f) * Math::PI; //pohyb vodorovne doleva
		else if (vel_x > 0)
			angle = Math::PI / 2.0f;          //pohyb vodorovne doprava
	}
	else //horizontalni ani vertikalni slozka neni nulova
	{
	    float a = atan(fabs(vel_x) / fabs(vel_y)); //ostry uhel, ktery svira vektor rychlosti s osou y

	    if (vel_x > 0 && vel_y > 0) //vektror rychlosti miri doprava a dolu
	    {
		    angle = Math::PI - a;
	    }
	    else if (vel_x < 0 && vel_y > 0) //vektror rychlosti miri doleva a dolu
	    {
		    angle = Math::PI + a;
	    }
	    else if (vel_x < 0 && vel_y < 0) //vektror rychlosti miri doleva a nahoru
	    { 
		    angle = -a;
        }
	    else //vektror rychlosti miri doprava a nahoru
	    {
		    angle = a;
	    }
	}

	if (angle < 0)
		angle += 2*Math::PI;

	return angle * (180/Math::PI); //prevod na stupne
}

void Moving_Object::Get_To_Screen(const SDL_Rect & resolution)
{
	GLfloat x = Get_Center_X();
	GLfloat y = Get_Center_Y();

	if (x < 0)
	{ 
		Set_Center_X(x + resolution.w);
	}
	else if (x >= resolution.w)
	{
		Set_Center_X(x - resolution.w);
	}

	if (y < 0)
	{ 
		Set_Center_Y(y + resolution.h);
	}
	else if (y >= resolution.h)
	{
		Set_Center_Y(y - resolution.h);
	}
}

GLfloat Moving_Object::Get_Speed() const
{ 
	return sqrt(pow(vel_x, 2) + pow(vel_y, 2)); 
}

void Moving_Object::Move(GLfloat distance, GLfloat angle)
{
	Set_Center_X(Get_Center_X() + (sin(angle * (Math::PI/180)) * (distance)));
	Set_Center_Y(Get_Center_Y() + (-cos(angle * (Math::PI/180)) * (distance)));	
}

Moving_Object::Moving_Object(const Point & point, GLfloat ang, GLfloat max_speed, GLfloat rotation_speed, GLfloat resistance) 
	: Object(point), max_speed(max_speed), rotation_speed(rotation_speed), resistance(resistance), vel_x(0.0f), vel_y(0.0f), angle(ang)
{
	doubles_activation[ORIGINAL] =  true;
	Deactivate_Doubles();
}

void Moving_Object::Push(GLfloat force, GLfloat angle, GLfloat fps)
{
	vel_x += sin(angle*(Math::PI/180)) * (force / fps);
	vel_y += -cos(angle*(Math::PI/180)) * (force / fps);	

	if (max_speed >= 0 && Get_Speed() > max_speed)
	{
		Set_Velocity(max_speed, Get_Motion_Angle());
	}
}

void Moving_Object::Rotate(horizontal_direction_t direction, GLfloat fps)
{
	if (direction == LEFT)
	{
	    angle -= fabs(rotation_speed) / fps;
	    if (angle < 0) angle = 360 - fabs(angle);
    }
	else if (direction == RIGHT)
	{
		angle += fabs(rotation_speed) / fps;
		if (angle >= 360) angle = angle - 360;
	}
}

void Moving_Object::Set_Doubles_Translations(const SDL_Rect & resolution)
{
	//dvojnici jsou vzdy posunuti o rozmer obrazovky
	GLfloat x_translation = GLfloat(resolution.w);
	GLfloat y_translation = GLfloat(resolution.h);

	doubles_translations[0][0] = 0;
	doubles_translations[0][1] = 0;

	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		switch (i)
		{
		case TOP_LEFT_DOUBLE:     //dvojnik v pravem hornim rohu
			doubles_translations[i][0] = -x_translation;
			doubles_translations[i][1] = -y_translation;
			break;
		case TOP_DOUBLE:          //dvojnik nahore
			doubles_translations[i][0] = 0.0f;
			doubles_translations[i][1] = -y_translation;
			break;
		case TOP_RIGHT_DOUBLE:    //dvojnik v pravem hornim rohu
			doubles_translations[i][0] = +x_translation;
			doubles_translations[i][1] = -y_translation;
			break;
		case LEFT_DOUBLE:         //dvojnik vlevo
			doubles_translations[i][0] = -x_translation;
			doubles_translations[i][1] = 0.0f;
			break;
		case RIGHT_DOUBLE:        //dvojnik vpravo
			doubles_translations[i][0] = +x_translation;
			doubles_translations[i][1] = 0.0f;
			break;
		case BOTTOM_LEFT_DOUBLE:  //dvojnik v levem dolnim rohu
			doubles_translations[i][0] = -x_translation;
			doubles_translations[i][1] = +y_translation;
			break;
		case BOTTOM_DOUBLE:       //dvojnik dole
			doubles_translations[i][0] = 0.0f;
			doubles_translations[i][1] = +y_translation;
			break;
		case BOTTOM_RIGHT_DOUBLE: //dvojnik v pravem dolnim rohu
			doubles_translations[i][0] = +x_translation;
			doubles_translations[i][1] = +y_translation;
			break;
		default:
			break;
		}
	}
}

void Moving_Object::Set_Velocity(GLfloat vel, GLfloat ang)
{
	if (max_speed >= 0 && vel > max_speed)
	{
	    Set_Vel_X(sin(ang*(Math::PI/180)) * max_speed);
	    Set_Vel_Y(-cos(ang*(Math::PI/180)) * max_speed);
	}
	else
	{
		Set_Vel_X(sin(ang*(Math::PI/180)) * vel);
	    Set_Vel_Y(-cos(ang*(Math::PI/180)) * vel);
	}
}

void Moving_Object::Slow_Down(GLfloat fps)
{
	Push(resistance, Get_Motion_Angle()-180.0f, fps);
}

vector<Point> Moving_Object::Get_Doubles_Positions() const
{
	vector<Point> positions;

	const Point & center = Get_Center();

	positions.push_back(center);

	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		doubles_positions pos = doubles_positions(i);

		if (Double_Activated(pos))
		{
			positions.push_back(Point(center.x + Get_Double_Translation_X(pos), center.y + Get_Double_Translation_Y(pos)));
		}
	}

	return positions;
}








