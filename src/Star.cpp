
/** \file Star.cpp
*   \brief Obsahuje definice metod tridy Star.
*/


#include "Star.h"
#include "Math.h"

using std::make_pair;

void Star::Set_Settings(const Settings & settings)
{
	ship_gravity = settings.Get_Setting_Value(Settings::SHIP_GRAVITY);
	projectile_gravity = settings.Get_Setting_Value(Settings::PROJECTILE_GRAVITY);
	color = settings.Get_Setting_Value(Settings::STAR_COLOR);
	active = settings.Get_Setting_Value(Settings::STAR);
}

Star::Star(Player & player1, Player & player2, vector<Projectile*> & projectiles, const Settings & settings, GLfloat radius, int num_vertical_lines, int num_horizontal_lines, const Point & position) 
	: Object(position), num_horizontal_lines(num_horizontal_lines), num_vertical_lines(num_vertical_lines), active(true), projectiles(&projectiles)
{
    Set_Settings(settings);

	Set_Radius(radius);

	players[Settings::FIRST] = &player1;
	players[Settings::SECOND] = &player2;
	
	int i = 1;
	while (glIsList(i))
		i++;

	list_number = i;
	glNewList(list_number, GL_COMPILE); 
	    Draw_Star(radius, num_vertical_lines, num_horizontal_lines);
	glEndList();	
}

void Star::Draw() const 
{ 
	if (!active) 
		return;

	glColor4f(color.red, color.green, color.blue, 0.2f);
	const Point & center = Get_Center();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(center.x, center.y, 0.0f);
	glCallList(list_number); 
	glPopMatrix();
};

void Star::Draw_Star(GLfloat radius, int num_vertical_lines, int num_horizontal_lines)
{
    Point center = Point(0.0f, 0.0f);

	glLineWidth(2.0f);

	//vykresleni pruhledneho vnitrku
	glEnable(GL_BLEND);
	Circle::Draw(center, radius, true);

	//vykresleni obvodu
	glDisable(GL_BLEND);
	Circle::Draw(center, radius, false);
	
	//vykresleni vnitrnich car
	GLfloat x_spacing = (radius*2) / (num_vertical_lines + 1); //velikost mezer mezi pruseciky svislych car s vodorovnou carou prochazejic stredem hvezdy
	Point top(center.x, center.y + radius); //nejvyssi bod kruznice hvezdy
	Point arc_max(center.x - radius + x_spacing, center.y); //prusecik aktualne vykreslovane svisle cary s vodorovnou carou prochazejic stredem hvezdy

	//vykresleni svislych car
	while (arc_max.x < center.x + radius)
	{
		Point center2; //stred kruznice jejiz prunik s kruhem hvezdy tvori svislou caru
		if (arc_max.x < center.x) //stred hledane kruznice bude vlevo od stredu hvezdy
		    center2 = Point(arc_max.x + (Math::Points_Distance(arc_max, center)/2.0f) + (pow(radius, 2)/(2*Math::Points_Distance(arc_max, center))), center.y);
		else if (arc_max.x > center.x) //stred hledane kruznice bude vpravo od stredu hvezdy
			center2 = Point(arc_max.x - (Math::Points_Distance(arc_max, center)/2.0f) - (pow(radius, 2)/(2*Math::Points_Distance(arc_max, center))), center.y);
		else //jde o rovnou svislou caru uprostred
		{
			glBegin(GL_LINES);
			    glVertex2f(center.x, center.y + radius);
				glVertex2f(center.x, center.y - radius);
			glEnd();
			arc_max.x += x_spacing;
			
			continue;
		}

		GLfloat radius2 = Math::Points_Distance(arc_max, center2); //polomer hledane kruznice
		GLfloat diff_x = fabs(top.x - center2.x);
		GLfloat diff_y = fabs(top.y - center2.y);
		GLfloat angle = Math::Radians_To_Degrees(atan(diff_x / diff_y));

		if (center.x < center2.x)
		{
			Circle::Draw_Arc(center2, radius2, 180.0f + angle, 360.0f - angle, false);
		}
		else
		{
			Circle::Draw_Arc(center2, radius2, angle, 180.0f - angle, false);
		}
		
		arc_max.x += x_spacing;
	}

	//vykresleni vodorovnych car
	GLfloat y_spacing = (radius*2) / (num_horizontal_lines + 1); //velikost mezer mezi vodorovnymi carami
	GLfloat y = center.y-radius+y_spacing; //pozice prvni vodorovne cart
	vector<Point> boundaries; //souradnice pruseciku vodorvne cary s kruznici hvezdy
	glBegin(GL_LINES);
	while (y < center.y + radius)
	{
		Math::Line_Circle_Intersections(make_pair(Point(-1.0f, y), Point(1.0f, y)), Circle(radius, center), false, &boundaries);
		if (boundaries.size() != 2) continue;
		glVertex2f(boundaries[0].x, boundaries[0].y);
		glVertex2f(boundaries[1].x, boundaries[1].y);
		y += y_spacing;
	}
	glEnd();

	glEnable(GL_BLEND);
}

void Star::Update(GLfloat fps)
{
	for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	{
		if (players[i]->Get_Mode() == Player::NORMAL) //hrac neni prave zniceny nebo explodujici
		{
		    if (Is_Active()) //hvezda je aktivni
		    {
			    GLfloat angle = Math::Get_Direction(players[i]->Get_Center(), Get_Center());
	            players[i]->Push(GLfloat(ship_gravity), angle, fps);  
		    }
		}
	}

	vector<Projectile*>::iterator it = projectiles->begin();
	while (it < projectiles->end())
	{
		if ((*it)->Get_Mode() == Projectile::NORMAL) //projektil neni prave zniceny nebo explodujici
		{
			if (Is_Active()) //efekt gravitace hvezdy
		    {
			    GLfloat angle = Math::Get_Direction((*it)->Get_Center(), Get_Center());
			    (*it)->Push(GLfloat(projectile_gravity), angle, fps);
		    }
		}

		it++;
	}
}

Star::Star(const Star & obj) 
	: Object(obj), color(obj.color), ship_gravity(obj.ship_gravity), projectile_gravity(obj.projectile_gravity), active(obj.active), 
	num_horizontal_lines(obj.num_horizontal_lines), num_vertical_lines(obj.num_vertical_lines), projectiles(obj.projectiles)
{  
	players[Settings::FIRST] = obj.players[Settings::FIRST];
	players[Settings::SECOND] = obj.players[Settings::SECOND];

	int i = 1;
	while (glIsList(i))
		i++;

	list_number = i;
	glNewList(list_number, GL_COMPILE); 
	    Draw_Star(Get_Radius(), num_vertical_lines, num_horizontal_lines);
	glEndList();	
}

Star & Star::operator=(const Star & star)
{
	Object::operator=(star);

	if (this == &star) 
        return *this;  

	players[Settings::FIRST] = star.players[Settings::FIRST];
	players[Settings::SECOND] = star.players[Settings::SECOND];
	projectiles = star.projectiles;
	color = star.color;
	ship_gravity = star.ship_gravity;
	projectile_gravity = star.projectile_gravity;
	active = star.active;
	num_horizontal_lines = star.num_horizontal_lines;
	num_vertical_lines = star.num_vertical_lines;

	glDeleteLists(list_number, 1);

	int i = 1;
	while (glIsList(i))
		i++;

	list_number = i;
	glNewList(list_number, GL_COMPILE); 
	    Draw_Star(Get_Radius(), num_vertical_lines, num_horizontal_lines);
	glEndList();

	return *this;
}


