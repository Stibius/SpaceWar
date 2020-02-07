
/** \file Circle.cpp
*   \brief Obsahuje definice metod tridy Circle.  
*/

#include <cmath>
#include "Circle.h"
#include "Math.h"

void Circle::Draw() const
{
	glColor4f(color.red, color.green, color.blue, color.alpha);
	Draw(Get_Center(), Get_Radius(), filled);
}

void Circle::Draw(GLfloat center_alpha, GLfloat edge_alpha) const
{
	Draw(Get_Center(), Get_Radius(), color, center_alpha, edge_alpha);
}

void Circle::Draw_Arc(GLfloat angle1, GLfloat angle2) const
{
	glColor4f(color.red, color.green, color.blue, color.alpha);
	Draw_Arc(Get_Center(), Get_Radius(), angle1, angle2, filled);
}

void Circle::Draw_Arc(GLfloat angle1, GLfloat angle2, GLfloat center_alpha, GLfloat edge_alpha) const
{
	Draw_Arc(Get_Center(), Get_Radius(), angle1, angle2, color, center_alpha, edge_alpha);
}

void Circle::Draw(const Point & center, GLfloat radius, bool filled)
{
	if (radius == 0.0f) return;
	if (radius < 0.0f) radius *= -1;

	glPushMatrix(); 
	glTranslatef(center.x, center.y, 0.0f);

    if (filled) //vyplneny
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
	}
	else //nevyplneny
	{
		glBegin(GL_LINE_LOOP);    
	}

	int N = int(radius); //pocet usecek, ze kterych je kruznice slozena
	
	GLfloat angle = 0;
	for (int i = 0; i < N; i++)
	{
		angle += (2*Math::PI)/N;
		glVertex2f(radius * cos(angle), radius * sin(angle));
	}

	glEnd();

	glPopMatrix();
}

void Circle::Draw(const Point & center, GLfloat radius, const Color & color, GLfloat center_alpha, GLfloat edge_alpha)
{
	if (radius == 0.0f) return;
	if (radius < 0.0f) radius *= -1;

	glPushMatrix(); 
	glTranslatef(center.x, center.y, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.red, color.green, color.blue, center_alpha);
	glVertex2f(0.0f, 0.0f);
	glColor4f(color.red, color.green, color.blue, edge_alpha);

	int N = int(radius); //pocet usecek, ze kterych je kruznice slozena
	
	GLfloat angle = 0;
	for (int i = 0; i <= N; i++)
	{
		angle += (2*Math::PI)/N;
		glVertex2f(radius * cos(angle), radius * sin(angle));
	}

	glEnd();

	glPopMatrix();
}

void Circle::Draw_Arc(const Point & center, GLfloat radius, GLfloat angle1, GLfloat angle2, bool filled)
{
	if (radius == 0.0f) return;
	if (radius < 0.0f) radius *= -1;

	if (angle1 == angle2)
		return;

	glPushMatrix();
	glTranslatef(center.x, center.y, 0.0f);

	if (filled) //vyplneny
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
	}
	else //nevyplneny
	{
		glBegin(GL_LINE_STRIP);    
	}
	
	//prevod uhlu do intervalu 0-359
	angle1 = Math::Degrees_To_Range(angle1);
	angle2 = Math::Degrees_To_Range(angle2);

	//vypocita body na kruznici korespondujici s danymi uhly
	Point point1 = Math::Get_Circle_Angle_Point(radius, angle1);
	Point point2 = Math::Get_Circle_Angle_Point(radius, angle2);

	int N = int(radius*4); //pocet usecek, ze kterych je cela kruznice slozena
	GLfloat cosa = cos(2*Math::PI/N); //cosinus uhlu, o ktery se posouvame
    GLfloat sina = sin(2*Math::PI/N); //sinus uhlu, o ktery se posouvame
	GLfloat x1 = 0.0f, y1 = -radius; //pocatecni bod vykreslovani
	GLfloat x2, y2;

	GLfloat angle_diff = (360.0f)/N; //prirustek uhlu, o ktery se posouvame
	GLfloat angle = 0.0f; //pocatecni uhel
	bool in = false; //urcuje, zda jsme prave posunuti do intervalu, ktery se ma vykreslovat
	
	//cyklus pro vykresleni, postupuje pres celou kruznici, ale vykresluje jen casti mezi zadanymi uhly
	for (int i = 0; i < N; i++)
    {
		if ((angle1 > angle2 && (angle > angle1 || angle < angle2)) || (angle1 < angle2 && (angle > angle1 && angle < angle2))) //zjistuje, zda jsme prave posunuti do intervalu, ktery se ma vykreslovat
		{
			if (!in) //prechod z nevykreslovane casti do vykreslovane
			{
				//navazani na krajni bod vykreslovane casti
				if (angle1 > angle2 && point1 != Point(x1, y1) && i != 0) 
				{
					glVertex2f(point1.x, point1.y);
				}
				else if (angle1 < angle2 && point2 != Point(x1, y1)) 
				{
					glVertex2f(point2.x, point2.y);
				}
				
				in = true;
			}

		    glVertex2f(x1, y1);	
		}
		else
		{
			if (in) //prechod z vykreslovane casti do nevykreslovane
			{
				//navazani na krajni bod vykreslovane casti
				if (angle1 > angle2) 
				{
					glVertex2f(point2.x, point2.y);
				}
				else if (angle1 < angle2) 
				{
					glVertex2f(point2.x, point2.y);
				}
				
				in = false;	
			}
		}
		
        x2 = x1*cosa - y1*sina;
        y2 = x1*sina + y1*cosa;

        x1 = x2;
        y1 = y2;


		angle += angle_diff;
    }

	glEnd();

	glPopMatrix();
}

void Circle::Draw_Arc(const Point & center, GLfloat radius, GLfloat angle1, GLfloat angle2, const Color & color, GLfloat center_alpha, GLfloat edge_alpha)
{
	if (radius == 0.0f) return;
	if (radius < 0.0f) radius *= -1;

	if (angle1 == angle2)
		return;

	glPushMatrix();
	glTranslatef(center.x, center.y, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.red, color.green, color.blue, center_alpha);
	glVertex2f(0.0f, 0.0f);
	glColor4f(color.red, color.green, color.blue, edge_alpha);
	
	//prevod uhlu do intervalu 0-359
	angle1 = Math::Degrees_To_Range(angle1);
	angle2 = Math::Degrees_To_Range(angle2);

	//vypocita body na kruznici korespondujici s danymi uhly
	Point point1 = Math::Get_Circle_Angle_Point(radius, angle1);
	Point point2 = Math::Get_Circle_Angle_Point(radius, angle2);

	int N = int(radius*4); //pocet usecek, ze kterych je cela kruznice slozena
	GLfloat cosa = cos(2*Math::PI/N); //cosinus uhlu, o ktery se posouvame
    GLfloat sina = sin(2*Math::PI/N); //sinus uhlu, o ktery se posouvame
	GLfloat x1 = 0.0f, y1 = -radius; //pocatecni bod vykreslovani
	GLfloat x2, y2;

	GLfloat angle_diff = (360.0f)/N; //prirustek uhlu, o ktery se posouvame
	GLfloat angle = 0.0f; //pocatecni uhel
	bool in = false; //urcuje, zda jsme prave posunuti do intervalu, ktery se ma vykreslovat
	
	//cyklus pro vykresleni, postupuje pres celou kruznici, ale vykresluje jen casti mezi zadanymi uhly
	for (int i = 0; i <= N; i++)
    {
		if ((angle1 > angle2 && (angle > angle1 || angle < angle2)) || (angle1 < angle2 && (angle > angle1 && angle < angle2))) //zjistuje, zda jsme prave posunuti do intervalu, ktery se ma vykreslovat
		{
			if (!in) //prechod z nevykreslovane casti do vykreslovane
			{
				//navazani na krajni bod vykreslovane casti
				if (angle1 > angle2 && point1 != Point(x1, y1) && i != 0) 
				{
					glVertex2f(point1.x, point1.y);
				}
				else if (angle1 < angle2 && point2 != Point(x1, y1)) 
				{
					glVertex2f(point2.x, point2.y);
				}
				
				in = true;
			}

		    glVertex2f(x1, y1);	
		}
		else
		{
			if (in) //prechod z vykreslovane casti do nevykreslovane
			{
				//navazani na krajni bod vykreslovane casti
				if (angle1 > angle2) 
				{
					glVertex2f(point2.x, point2.y);
				}
				else if (angle1 < angle2) 
				{
					glVertex2f(point2.x, point2.y);
				}
				
				in = false;	
			}
		}
		
        x2 = x1*cosa - y1*sina;
        y2 = x1*sina + y1*cosa;

        x1 = x2;
        y1 = y2;


		angle += angle_diff;
    }

	glEnd();

	glPopMatrix();
}









