
/** \file Powerup_System.cpp
*   \brief Obsahuje definice metod tridy Powerup_System.
*/

#include "Powerup_System.h"
#include "Projectile.h"
#include "Star.h"
#include "Collision_System.h"
#include <time.h>

////////////////////////////////POWERUPS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Powerup_System::num_instances = 0;
bool Powerup_System::initialized = false;
int Powerup_System::Powerup::display_time;
const GLfloat Powerup_System::Powerup::RADIUS = 36.0f; 

int Powerup_System::Health_Powerup::bonus_value;
GLuint Powerup_System::Health_Powerup::list;
const Color Powerup_System::Health_Powerup::COLOR = Color(1.0f, 0.0f, 0.0f, 0.2f);

int Powerup_System::Shield_Powerup::bonus_value;
GLuint Powerup_System::Shield_Powerup::list;
const Color Powerup_System::Shield_Powerup::COLOR = Color(1.0f, 1.0f, 0.0f, 0.2f);

int Powerup_System::Energy_Powerup::bonus_value;
GLuint Powerup_System::Energy_Powerup::list;
const Color Powerup_System::Energy_Powerup::COLOR = Color(0.0f, 1.0f, 1.0f, 0.2f);

int Powerup_System::Triple_Powerup::effect_duration;
GLuint Powerup_System::Triple_Powerup::list;
Texture Powerup_System::Triple_Powerup::text;
const Color Powerup_System::Triple_Powerup::COLOR = Color(1.0f, 0.65f, 0.0f, 1.0f);

int Powerup_System::Guided_Powerup::effect_duration;
GLuint Powerup_System::Guided_Powerup::list;
const Color Powerup_System::Guided_Powerup::COLOR = Color(1.0f, 1.0f, 1.0f, 0.2f);

GLuint Powerup_System::Laser_Powerup::list;
const Color Powerup_System::Laser_Powerup::COLOR = Color(0.0f, 0.0f, 1.0f, 0.2f);

GLuint Powerup_System::Rocket_Powerup::list;
const Color Powerup_System::Rocket_Powerup::COLOR = Color(1.0f, 0.0f, 1.0f, 0.2f);

GLuint Powerup_System::Laser_Beam_Powerup::list;
const Color Powerup_System::Laser_Beam_Powerup::COLOR = Color(0.0f, 1.0f, 0.0f, 0.2f);

Sound Powerup_System::Powerup::pickup_sound = Sound("powerup_pickup.wav");

Powerup_System::Powerup::Powerup(const Point & position) : Object(position)
{
	time_limit = SDL_GetTicks() + (display_time * 1000); //prevod na milisekundy
	Set_Radius(RADIUS);
}

void Powerup_System::Powerup::Play_Pickup_Sound() const 
{ 
	pickup_sound.Play(0); 
}

const Circle Powerup_System::Powerup::Get_Circle() const 
{ 
	return Circle(GLfloat(RADIUS), Get_Center()); 
}

bool Powerup_System::Powerup::Time_Out()
{
	if (SDL_GetTicks() >= time_limit)
	{
		return true;
	}
	return false;
}

void Powerup_System::Health_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "doplneni zivotu" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    glLineWidth(2.0f);

		//vykresli obrys krize
	    glDisable(GL_BLEND);

		Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);

		glBegin(GL_LINE_LOOP);
		    glVertex2f(-8.0f, -28.0f);
		    glVertex2f(8.0f, -28.0f);
		    glVertex2f(8.0f, -8.0f);
		    glVertex2f(28.0f, -8.0f);
		    glVertex2f(28.0f, 8.0f);
		    glVertex2f(8.0f, 8.0f);
		    glVertex2f(8.0f, 28.0f);
		    glVertex2f(-8.0f, 28.0f);
		    glVertex2f(-8.0f, 8.0f);
		    glVertex2f(-28.0f, 8.0f);
		    glVertex2f(-28.0f, -8.0f);
		    glVertex2f(-8.0f, -8.0f);
	    glEnd();
		
		//vykresli vnitrek krize
	    glEnable(GL_BLEND);
	    glBegin(GL_QUADS);
		    //svisla cast krize
	        glVertex2f(-10.0f, -30.0f);
		    glVertex2f(10.0f, -30.0f);
		    glVertex2f(10.0f, 30.0f);
		    glVertex2f(-10.0f, 30.0f);

			//leva cast vodorovne casti krize
		    glVertex2f(-30.0f, -10.0f);
		    glVertex2f(-10.0f, -10.0f);
		    glVertex2f(-10.0f, 10.0f);
		    glVertex2f(-30.0f, 10.0f);

			//prava cast vodorovne casti krize
			glVertex2f(10.0f, -10.0f);
		    glVertex2f(30.0f, -10.0f);
		    glVertex2f(30.0f, 10.0f);
		    glVertex2f(10.0f, 10.0f);
	    glEnd();

	glEndList();
}

void Powerup_System::Health_Powerup::Apply(Player & player)
{
	player.Add_Health(bonus_value);
	Play_Pickup_Sound();
}

void Powerup_System::Health_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();

	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);

	glPopMatrix();
}

void Powerup_System::Shield_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "doplneni stitu" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    glLineWidth(2.0f);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				//vykresli se obrys
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
				Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
			}
			else
			{
	            //vykresli se vnitrek
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
			}

			//vykresli polygon stitu
		    glBegin(GL_POLYGON);
	            glVertex2f(-17.0f, -22.0f);
		        glVertex2f(-17.0f, 11.0f);
		        glVertex2f(0.0f, 28.0f);
		        glVertex2f(17.0f, 11.0f);
		        glVertex2f(17.0f, -22.0f);
	        glEnd();
		}

	glEndList();
}

void Powerup_System::Shield_Powerup::Apply(Player & player)
{
	{
		player.Add_Shield(bonus_value);
		Play_Pickup_Sound();
	}
}

void Powerup_System::Shield_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

void Powerup_System::Energy_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "doplneni energie" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glLineWidth(2.0f);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
				Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
				glBegin(GL_LINE_LOOP);
				    glVertex2f(0.0f, -20.0f);
				    glVertex2f(-10.0f, 0.0f);
				    glVertex2f(0.0f, 0.0f);
				    glVertex2f(-10.0f, 20.0f);
				    glVertex2f(14.0f, -7.5f);
				    glVertex2f(4.0f, -7.5f);
	                glVertex2f(10.0f, -20.0f);
	            glEnd();
			}
			else
			{
	            //vykresli se vnitrek
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
				glBegin(GL_QUADS);
				    glVertex2f(0.0f, -20.0f);
				    glVertex2f(-10.0f, 0.0f);
				    glVertex2f(0.0f, 0.0f);
	                glVertex2f(10.0f, -20.0f);
	            glEnd();

				glBegin(GL_TRIANGLES);
				    glVertex2f(-10.0f, 20.0f);
				    glVertex2f(14.0f, -7.5f);
				    glVertex2f(4.0f, -7.5f);
	            glEnd();
			}
		}

	glEndList();
}

void Powerup_System::Energy_Powerup::Apply(Player & player)
{
	player.Add_Energy(bonus_value);
	Play_Pickup_Sound();
}

void Powerup_System::Energy_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

void Powerup_System::Triple_Powerup::Draw_List()
{
	Font::Create_Texture("zektonbo.ttf", 40, "3x", text);
	GLfloat w = Font::Get_Text_Width("zektonbo.ttf", 40, "3x");
	GLfloat h = 40;

	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "trojite strely" (bez opsane kruznice).
	glNewList(list, GL_COMPILE); 
	        glLineWidth(2.0f);
			glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
			Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
			text.Draw(Point(-(w/2.0f), -(h/2.0f)));
	glEndList();
}

void Powerup_System::Triple_Powerup::Apply(Player & player)
{
	player.Triple_On(effect_duration*1000);
	Play_Pickup_Sound();
}

void Powerup_System::Triple_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);

	//ulozit puvodni hodnoty blendfunc
	GLint dst, src;
	glGetIntegerv(GL_BLEND_DST, &dst);
	glGetIntegerv(GL_BLEND_SRC, &src);

	glBlendFunc(GL_ONE, GL_ONE); //Required if you want alpha-blended textures (for our fonts) 

	glCallList(list);
	glBlendFunc(src, dst); 
	glPopMatrix();
}


void Powerup_System::Guided_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "navadene strely" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glLineWidth(2.0f);
	    glDisable(GL_BLEND);

		Circle::Draw(Point(0.0f, 0.0f), 20.0f, false);

	    glBegin(GL_LINES);
		    //horni vodorovna carka
	        glVertex2f(-5.0f, -30.0f);
		    glVertex2f(5.0f, -30.0f);

			//prava svisla carka
		    glVertex2f(30.0f, -5.0f);
		    glVertex2f(30.0f, 5.0f);

			//dolni vodorovna carka
		    glVertex2f(-5.0f, 30.0f);
		    glVertex2f(5.0f, 30.0f);

			//leva svisla carka
	        glVertex2f(-30.0f, -5.0f);
		    glVertex2f(-30.0f, 5.0f);

			//hlavni vodorovna cara
		    glVertex2f(-30.0f, 0.0f);
		    glVertex2f(30.0f, 0.0f);

			//hlavni svisla cara
		    glVertex2f(0.0f, -30.0f);
		    glVertex2f(0.0f, 30.0f);
	    glEnd();

		Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
	   
	    glEnable(GL_BLEND);
	
	glEndList();
}

void Powerup_System::Guided_Powerup::Apply(Player & player)
{
	player.Guided_On(effect_duration*1000);
	Play_Pickup_Sound();
}

void Powerup_System::Guided_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

void Powerup_System::Rocket_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "rakety" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glLineWidth(2.0f);
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				//vykresli se obrys
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
				Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
			}
			else
			{
				//vykresli se vnitrek
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
			}
			
			//trup rakety
			glBegin(GL_POLYGON);
	            glVertex2f(0.0f, -20.0f);
		        glVertex2f(6.0f, -14.0f);
		        glVertex2f(6.0f, 20.0f);
		        glVertex2f(-6.0f, 20.0f);
				glVertex2f(-6.0f, -14.0f);
	        glEnd();

			//leve kridlo rakety
			glBegin(GL_POLYGON);
	            glVertex2f(-6.0f, 5.0f);
		        glVertex2f(-12.0f, 5.0f);
		        glVertex2f(-12.0f, 20.0f);
		        glVertex2f(-6.0f, 20.0f);
	        glEnd();

			//prave kridlo rakety
			glBegin(GL_POLYGON);
	            glVertex2f(6.0f, 5.0f);
		        glVertex2f(12.0f, 5.0f);
		        glVertex2f(12.0f, 20.0f);
		        glVertex2f(6.0f, 20.0f);
	        glEnd();
		}

		glLoadIdentity();

	glEndList();
}

void Powerup_System::Rocket_Powerup::Apply(Player & player)
{
	player.Set_Weapon(Settings::ROCKET_WEAPON);
	Play_Pickup_Sound();
}

void Powerup_System::Rocket_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

void Powerup_System::Laser_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "laserove strely" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glLineWidth(2.0f);

		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		//vykresli se zvlast obrys a vnitrek
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) //obrys
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
			}
			else //vnitrek
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
			}

			//tvar laserove strely
	        glBegin(GL_POLYGON);
		        glVertex2f(-2.0f, -25.0f);
			    glVertex2f(2.0f, -25.0f);
			    glVertex2f(2.0f, -5.0f);
			    glVertex2f(-2.0f, -5.0f);
		    glEnd();

			//tvar laserove strely
	        glBegin(GL_POLYGON);
		        glVertex2f(-2.0f, 5.0f);
			    glVertex2f(2.0f, 5.0f);
			    glVertex2f(2.0f, 25.0f);
			    glVertex2f(-2.0f, 25.0f);
		    glEnd();
		}

		glDisable(GL_BLEND);
		Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
		glEnable(GL_BLEND);
		glLoadIdentity();	

	glEndList();
}

void Powerup_System::Laser_Powerup::Apply(Player & player)
{
	player.Set_Weapon(Settings::LASER_WEAPON);
	Play_Pickup_Sound();
}

void Powerup_System::Laser_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

void Powerup_System::Laser_Beam_Powerup::Draw_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	list = i;

	//vytvori display-list a vykresli do nej vnitrek powerupu "nlaserovy paprsek" (bez opsane kruznice).
	//rozmery jsou v pixelech pri rozliseni 1440x900
	glNewList(list, GL_COMPILE); 
	    glColor4f(COLOR.red, COLOR.green, COLOR.blue, COLOR.alpha);
	    glLineWidth(2.0f);

		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		//vykresli se zvlast obrys a vnitrek
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) //obrys
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
			}
			else //vnitrek
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
			}

			//tvar laseroveho paprsku
	        glBegin(GL_POLYGON);
		        glVertex2f(-2.0f, -25.0f);
			    glVertex2f(2.0f, -25.0f);
			    glVertex2f(2.0f, 25.0f);
			    glVertex2f(-2.0f, 25.0f);
		    glEnd();
		}

		glDisable(GL_BLEND);
		Circle::Draw(Point(0.0f, 0.0f), RADIUS, false);
		glEnable(GL_BLEND);
		glLoadIdentity();	

	glEndList();
}

void Powerup_System::Laser_Beam_Powerup::Apply(Player & player)
{
	player.Set_Weapon(Settings::LASER_BEAM_WEAPON);
	Play_Pickup_Sound();
}

void Powerup_System::Laser_Beam_Powerup::Draw() const
{
	glPushMatrix();
	glLoadIdentity();
	const Point & center = Get_Center();
	
	glTranslatef(center.x, center.y, 0.0f);
	
	glCallList(list);
	glPopMatrix();
}

////////////////////////////////POWERUP_SYSTEM///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Powerup_System::Powerup_System(const Settings & settings, const Collision_System & collision_system, const Player & player1, const Player & player2)
	: player1(&player1), player2(&player2), collision_system(&collision_system) 
{
	Set_Settings(settings);

	if (!initialized)
	{
	    Health_Powerup::Draw_List();
	    Shield_Powerup::Draw_List();
	    Energy_Powerup::Draw_List();
	    Triple_Powerup::Draw_List();
	    Guided_Powerup::Draw_List();
	    Laser_Powerup::Draw_List();
	    Rocket_Powerup::Draw_List();
		Laser_Beam_Powerup::Draw_List();
		initialized = true;
	}

	next_powerup_time = SDL_GetTicks() + (rand() % (max_interval - min_interval + 1)) + min_interval;

	num_instances++;
}

void Powerup_System::Set_Settings(const Settings & settings)
{
	min_interval = settings.Get_Setting_Value(Settings::POWERUP_MIN_INTERVAL) * 1000;
	max_interval = settings.Get_Setting_Value(Settings::POWERUP_MAX_INTERVAL) * 1000;

	area = settings.Get_Resolution();
	area.w -= Uint16(Powerup::RADIUS * 2);
	area.h -= Uint16(Powerup::RADIUS * 2) + 90;
	area.x += Uint16(Powerup::RADIUS);
	area.y += Uint16(Powerup::RADIUS) + 90;

	allowed[HEALTH_POWERUP] = settings.Get_Setting_Value(Settings::HEALTH_POWERUP_ALLOWED);
	allowed[SHIELD_POWERUP] = settings.Get_Setting_Value(Settings::SHIELD_POWERUP_ALLOWED);
	allowed[ENERGY_POWERUP] = settings.Get_Setting_Value(Settings::ENERGY_POWERUP_ALLOWED);
	allowed[GUIDED_POWERUP] = settings.Get_Setting_Value(Settings::GUIDED_POWERUP_ALLOWED);
	allowed[TRIPLE_POWERUP] = settings.Get_Setting_Value(Settings::TRIPLE_POWERUP_ALLOWED);
	allowed[LASER_POWERUP] = settings.Get_Setting_Value(Settings::LASER_POWERUP_ALLOWED);
	allowed[ROCKET_POWERUP] = settings.Get_Setting_Value(Settings::ROCKET_POWERUP_ALLOWED);
	allowed[LASER_BEAM_POWERUP] = settings.Get_Setting_Value(Settings::LASER_BEAM_POWERUP_ALLOWED);

	Powerup::Set_Display_Time(settings.Get_Setting_Value(Settings::POWERUP_DISPLAY_DURATION));

	Health_Powerup::Set_Bonus_Value(settings.Get_Setting_Value(Settings::HEALTH_POWERUP_AMOUNT));
	Shield_Powerup::Set_Bonus_Value(settings.Get_Setting_Value(Settings::SHIELD_POWERUP_AMOUNT));
	Energy_Powerup::Set_Bonus_Value(settings.Get_Setting_Value(Settings::ENERGY_POWERUP_AMOUNT));

	Triple_Powerup::Set_Effect_Duration(settings.Get_Setting_Value(Settings::TRIPLE_POWERUP_DURATION));
	Guided_Powerup::Set_Effect_Duration(settings.Get_Setting_Value(Settings::GUIDED_POWERUP_DURATION));
}

void Powerup_System::Draw() const
{
	for (vector<Powerup*>::const_iterator it = powerups.begin(); it < powerups.end(); it++)
	{
		(*it)->Draw();
	}
}

bool Powerup_System::Some_Allowed() const
{
	for (int i = 0; i < NUM_POWERUPS; i++)
	{
		if (allowed[i])
			return true;
	}
	return false;
}

bool Powerup_System::Some_Useful()
{
	for (int i = 0; i < NUM_POWERUPS; i++)
	{
		if (allowed[i] && Is_Useful(powerups_t(i)))
			return true;
	}
	return false;
}

bool Powerup_System::Is_Useful(powerups_t type, const Player & player)
{
	switch (type)
	{
	case HEALTH_POWERUP:
		if (player.Get_Health() < Player::Get_Max_Health())
			return true;
		break;
	case SHIELD_POWERUP:
		if (player.Get_Shield() < Player::Get_Max_Shield())
			return true;
		break;
	case ENERGY_POWERUP:
		if (player.Get_Energy() < Player::Get_Max_Energy())
			return true;
		break;
	case GUIDED_POWERUP:
		if (player.Get_Weapon_Type() != Settings::LASER_BEAM_WEAPON)
		    return true;
		break;
	case TRIPLE_POWERUP:
		//if (player.Get_Weapon_Type() != Settings::LASER_BEAM_WEAPON)
		    return true;
		break;
	case ROCKET_POWERUP:
		if (player.Get_Weapon_Type() != Settings::ROCKET_WEAPON)
			return true;
		break;
	case LASER_POWERUP:
		if (player.Get_Weapon_Type() != Settings::LASER_WEAPON)
			return true;
		break;
	case LASER_BEAM_POWERUP:
		if (player.Get_Weapon_Type() != Settings::LASER_BEAM_WEAPON)
			return true;
		break;
	default:
		break;
	}

	return false;
}

bool Powerup_System::Is_Useful(powerups_t type)
{
	if (Is_Useful(type, *player1) || Is_Useful(type, *player2))
	{
		return true;
	}
	else
	{
	    return false;
	}
}

void Powerup_System::Put_Powerup()
{
	if (Some_Allowed())
	{
		//srand(unsigned int(time(NULL)));
		int type;

		if (Some_Useful())
		{
			do
		    {
			    type = rand() % NUM_POWERUPS;
		    } while (!allowed[type] || !Is_Useful(powerups_t(type)));
		}
		else
		{
			do
		    {
			    type = rand() % NUM_POWERUPS;
		    } while (!allowed[type]);
		}

		switch (type)
	    {
		case HEALTH_POWERUP:
			powerups.push_back(new Health_Powerup());
			
			break;
		case SHIELD_POWERUP:
			powerups.push_back(new Shield_Powerup());
			
			break;
		case ENERGY_POWERUP:
			powerups.push_back(new Energy_Powerup());
			
			break;
		case TRIPLE_POWERUP:
			powerups.push_back(new Triple_Powerup());
			
			break;
		case GUIDED_POWERUP:
			powerups.push_back(new Guided_Powerup());
			
			break;
		case LASER_POWERUP:
			powerups.push_back(new Laser_Powerup());
			
			break;
		case ROCKET_POWERUP:
			powerups.push_back(new Rocket_Powerup());
			
			break;
		case LASER_BEAM_POWERUP:
			powerups.push_back(new Laser_Beam_Powerup());
			
			break;
		default:
			break;
		}

		collision_system->Set_New_Powerup_Position(**(powerups.end()-1), area); 
	}
}

void Powerup_System::Update(GLfloat fps)
{
	if (SDL_GetTicks() >= next_powerup_time)
	{
        Put_Powerup();
		next_powerup_time = SDL_GetTicks() + ((rand() % (max_interval - min_interval + 1)) + min_interval);
	}
	
	vector<Powerup*>::const_iterator it = powerups.begin();
	while (it < powerups.end())
	{
		if ((*it)->Time_Out())
		{
			vector<Powerup*>::const_iterator new_it;
			bool first = false;
			if (it == powerups.begin())
			{
				first = true;
			}
			else
			{
			    new_it = it-1;
			}
			
			delete *it;
			
			powerups.erase(it);
			if (powerups.empty())
			{
				break;
			}
			if (first)
				it = powerups.begin();
			else
			    it = new_it;
		}
		it++;
	}
}

Powerup_System::~Powerup_System()
{
	num_instances--;
	if (num_instances == 0)
	{
		for (vector<Powerup*>::iterator it = powerups.begin(); it < powerups.end(); it++)
		{
			delete *it;
		}

		powerups.clear();

		if (initialized == true)
		{
			Health_Powerup::Delete_List();
			Shield_Powerup::Delete_List();
			Energy_Powerup::Delete_List();
			Triple_Powerup::Delete_List();
			Guided_Powerup::Delete_List();
			Laser_Powerup::Delete_List();
			Rocket_Powerup::Delete_List();
			Laser_Beam_Powerup::Delete_List();
			initialized = false;
		}
	}
}

Powerup_System::Powerup_System(const Powerup_System & powerup_system)
	: min_interval(powerup_system.min_interval), max_interval(powerup_system.max_interval), next_powerup_time(powerup_system.next_powerup_time),
	area(powerup_system.area), powerups(powerup_system.powerups), collision_system(powerup_system.collision_system)
{
	num_instances++;
	
	for (int i = 0; i < NUM_POWERUPS; i++)
	{
		allowed[i] = powerup_system.allowed[i];
	}
}










