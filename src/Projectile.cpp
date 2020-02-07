
/** \file Projectile.cpp
*   \brief Obsahuje definice metod tridy Projectile.
*/


#include "Circle.h"
#include "Projectile.h"
#include "Star.h"
#include "Math.h"

const GLfloat Projectile::EXPLOSION_RADIUSES[Settings::NUM_PROJECTILE_WEAPONS] = { 20.0f, 70.0f };
const Color Projectile::EXPLOSION_COLOR = Color(1.0f, 1.0f, 0.0f);
Color Projectile::player_colors[Settings::NUM_PLAYERS];
Uint32 Projectile::lists_numbers[Settings::NUM_PROJECTILE_WEAPONS]; 
int Projectile::damage[Settings::NUM_PROJECTILE_WEAPONS] = { NULL, NULL }; 
int Projectile::starting_speed[Settings::NUM_WEAPONS] = { NULL, NULL }; 
int Projectile::guided_rotation_speed;
Sound Projectile::explosion_sound = Sound("explosion.wav");
bool Projectile::initialized = false;
polygon_t Projectile::borders[Settings::NUM_PROJECTILE_WEAPONS];
const GLfloat Projectile::LENGTHS[Settings::NUM_PROJECTILE_WEAPONS] = { 20.0f, 40.0f };
const GLfloat Projectile::WIDTHS[Settings::NUM_PROJECTILE_WEAPONS] = { 4.0f, 24.0f };

void Projectile::Delete_Lists()
{
	if (initialized == false) 
		return;

	for (int i = Settings::LASER_WEAPON; i < Settings::NUM_WEAPONS; i++)
	{
		glDeleteLists(lists_numbers[i], 1);
	}

	initialized = false;
}

void Projectile::Draw() const
{
	//nastavi barvu vykreslovani podle hrace, ktery projektil vystrelil
	glColor4f(player_colors[player].red, player_colors[player].green, player_colors[player].blue, 0.2f);
	
	//vykresleni podle aktualniho stavu
	switch (state)
	{
	case NORMAL: //projektil jeste nic nezasahl
		glPushMatrix();
	    glLoadIdentity();

		glTranslatef(Get_Center_X(), Get_Center_Y(), 0.0f);
	    glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);
	    glCallList(lists_numbers[type]);
		glLoadIdentity();

		//vykresleni aktivovanych dvojniku
		for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	    {
			doubles_positions current_double = doubles_positions(i);

		    if (Double_Activated(current_double))
		    {
			    glTranslatef(Get_Center_X() + Get_Double_Translation_X(current_double), Get_Center_Y() + Get_Double_Translation_Y(current_double), 0.0f);
                glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);
			    glCallList(lists_numbers[type]);
	            glLoadIdentity();	
			}
		}
		glPopMatrix();
		break;
	case EXPLOSION: //projektil neco zasahl a prave probiha jeho exploze
		Draw_Explosion();
		break;
	case DESTROYED: //projektil neco zasahl a exploze uz zkoncila, je znicen
		break;
	default:
		break;
	}
}

void Projectile::Draw_Explosion() const
{
	if (SDL_GetTicks() >= destroy_time) return;

	Uint32 remaining_time = destroy_time - SDL_GetTicks(); //cas zbyvajici do konce exploze

	//pruhlednost se s casem zvysuje tak, aby v case konce exploze byla 100% (postupne mizeni exploze)
	GLfloat alpha = (GLfloat(remaining_time) / EXPLOSION_DURATION);

	//vykresleni exploze projektilu
	glColor4f(EXPLOSION_COLOR.red, EXPLOSION_COLOR.green, EXPLOSION_COLOR.blue, alpha);	
	Circle::Draw(Get_Center(), EXPLOSION_RADIUSES[type], true);

	//vykresleni explozi aktivovanych dvojniku projektilu
	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		doubles_positions current_double = doubles_positions(i);

		if (Double_Activated(current_double))
		{
			glColor4f(EXPLOSION_COLOR.red, EXPLOSION_COLOR.green, EXPLOSION_COLOR.blue, alpha);		
	        Circle::Draw(Point(Get_Center_X() + Get_Double_Translation_X(current_double),  Get_Center_Y() + Get_Double_Translation_Y(current_double)), EXPLOSION_RADIUSES[type], true);
        }
    }	
}

void Projectile::Draw_Laser_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	lists_numbers[Settings::LASER_WEAPON] = i;

	glNewList(lists_numbers[Settings::LASER_WEAPON], GL_COMPILE); 
		glLineWidth(2.0f);

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
		        glVertex2f(-2.0f, -10.0f);
			    glVertex2f(2.0f, -10.0f);
			    glVertex2f(2.0f, 10.0f);
			    glVertex2f(-2.0f, 10.0f);
		    glEnd();
		}

	glEndList();
}

void Projectile::Draw_Rocket_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	lists_numbers[Settings::ROCKET_WEAPON] = i;

	glNewList(lists_numbers[Settings::ROCKET_WEAPON], GL_COMPILE);
	    glLineWidth(2.0f);

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
			
			//tvar trupu
			glBegin(GL_POLYGON);
	            glVertex2f(0.0f, -20.0f);
		        glVertex2f(6.0f, -14.0f);
		        glVertex2f(6.0f, 20.0f);
		        glVertex2f(-6.0f, 20.0f);
				glVertex2f(-6.0f, -14.0f);
	        glEnd();

			//tvar leveho kridla
			glBegin(GL_POLYGON);
	            glVertex2f(-6.0f, 5.0f);
		        glVertex2f(-12.0f, 5.0f);
		        glVertex2f(-12.0f, 20.0f);
		        glVertex2f(-6.0f, 20.0f);
	        glEnd();

			//tvar praveho kridla
			glBegin(GL_POLYGON);
	            glVertex2f(6.0f, 5.0f);
		        glVertex2f(12.0f, 5.0f);
		        glVertex2f(12.0f, 20.0f);
		        glVertex2f(6.0f, 20.0f);
	        glEnd();
		}

		//plamen trysky
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		    glVertex2f(6.0f, 21.0f);
		    glVertex2f(-6.0f, 21.0f);
		    glVertex2f(0.0f, 30.0f);
		glEnd();

	glEndList();
}

void Projectile::Destroy()
{
	if (state == NORMAL)
	{
		state = EXPLOSION;
		destroy_time = SDL_GetTicks() + EXPLOSION_DURATION;
		explosion_sound.Play(0);
	}
}

polygon_t Projectile::Get_Border() const
{
	polygon_t transformed_border; //zde nakonec bude vysledny polygon

	//zjisti aktualni pozici projektilu 
	GLfloat x = Get_Center_X();
	GLfloat y = Get_Center_Y();

	//pretransformuje MODELVIEW matici tak, aby odpovidala aktualni transformacni matici projektilu 
	glTranslatef(x, y, 0.0f); 
	glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);

	//prekopiruje MODELVIEW matici do pole matrix
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix); 

	glLoadIdentity();

	transformed_border = borders[type]; 

	//pretransformovani hranicniho polygonu na zaklade transformacni matice projektilu
	for (unsigned int i = 0; i < transformed_border.size(); i++)
	{
		transformed_border[i] = Math::Transform_Point(borders[type][i], matrix);
	}
	
	return transformed_border;
}

void Projectile::Set_Settings(const Settings & settings)
{
	damage[Settings::LASER_WEAPON] = settings.Get_Setting_Value(Settings::LASER_DAMAGE);
	damage[Settings::ROCKET_WEAPON] = settings.Get_Setting_Value(Settings::ROCKET_DAMAGE);

	starting_speed[Settings::LASER_WEAPON] = settings.Get_Setting_Value(Settings::LASER_SPEED);
	starting_speed[Settings::ROCKET_WEAPON] = settings.Get_Setting_Value(Settings::ROCKET_SPEED);

	guided_rotation_speed = settings.Get_Setting_Value(Settings::GUIDED_MISSILE_ROTATION_SPEED);

	player_colors[Settings::FIRST] = settings.Get_Setting_Value(Settings::PLAYER1_COLOR);
	player_colors[Settings::SECOND] = settings.Get_Setting_Value(Settings::PLAYER2_COLOR);

	switch (type)
	{
	case Settings::LASER_WEAPON:
		Moving_Object::Set_Resistance(GLfloat(settings.Get_Setting_Value(Settings::LASER_RESISTANCE)));
		break;
	case Settings::ROCKET_WEAPON:
		Moving_Object::Set_Resistance(GLfloat(settings.Get_Setting_Value(Settings::ROCKET_RESISTANCE)));
		break;
	default:
		break;
	}
}

void Projectile::Draw_Lists()
{
	if (initialized == false)
	{
	    Draw_Laser_List();
	    Draw_Rocket_List();
		initialized = true;
    }
}

Projectile::Projectile(const Settings & settings, Settings::players_t player, Settings::weapons_t type, bool guided, bool triple, unsigned int shot_identifier, const Point & position, GLfloat ang) 
	: Moving_Object(position, ang, -1.0f, GLfloat(guided_rotation_speed)), destroy_time(0), target(NULL), player(player), shot_identifier(shot_identifier),
	state(NORMAL), guided(guided), triple(triple), type(type)
{
	Set_Settings(settings);

	switch (type)
	{
	case Settings::LASER_WEAPON:
		if (borders[Settings::LASER_WEAPON].empty())
		{
			borders[Settings::LASER_WEAPON].push_back(Point(-2.0f, -10.0f));
			borders[Settings::LASER_WEAPON].push_back(Point(2.0f, -10.0f));
			borders[Settings::LASER_WEAPON].push_back(Point(2.0f, 10.0f));
			borders[Settings::LASER_WEAPON].push_back(Point(-2.0f, 10.0f));
		}
	    Set_Radius(10.0f);
	    break;
	case Settings::ROCKET_WEAPON:
		if (borders[Settings::ROCKET_WEAPON].empty())
		{
			borders[Settings::ROCKET_WEAPON].push_back(Point(0.0f, -20.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(6.0f, -14.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(6.0f, 5.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(12.0f, 5.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(12.0f, 20.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(-12.0f, 20.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(-12.0f, 5.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(-6.0f, 5.0f));
			borders[Settings::ROCKET_WEAPON].push_back(Point(-6.0f, -14.0f));
		}
	    Set_Radius(20.0f);
	    break;
	default:
		break;
	}

	if (starting_speed[type])
	    Set_Velocity(GLfloat(starting_speed[type]), ang);
}

void Projectile::Update(GLfloat fps)
{
	switch (state)
	{
	case NORMAL:
		if (target != NULL) //efekt navadene strely
		{
			GLfloat angle; //smer od projektilu ke sledovanemu hraci

			angle = Math::Get_Direction(Get_Center(), target->Get_Center());

			if (angle != Get_Angle()) //otaceni smerem ke sledovanemu hraci
			{
		        GLfloat plus;  //pocet stupnu po smeru hodinovych rucicek k cilovemu smeru
			    GLfloat minus; //pocet stupnu proti smeru hodinovych rucicek k cilovemu smeru

			    if (angle > Get_Angle())
			    {
				    plus = angle - Get_Angle();
				    minus = Get_Angle() + (360.0f - angle);
			    }
			    else if (angle < Get_Angle())
			    {
				    plus = angle + (360.0f - Get_Angle());
				    minus = Get_Angle() - angle;
				}

				//otoci se tim smerem, kde je to bliz
			    if (plus < minus)
			    {
				    Rotate(Moving_Object::RIGHT, fps);
			    }
			    else 
			    {
			        Rotate(Moving_Object::LEFT, fps);
			    }
				
			    Change_Direction(Get_Angle());
			}
		}

		Slow_Down(fps); //samovolne zpomalovani

		//zmena polohy v zavislosti na rychlosti
	    Set_Center_X(Get_Center_X() + (Get_Vel_X() / fps));
	    Set_Center_Y(Get_Center_Y() + (Get_Vel_Y() / fps));
		break;
	case EXPLOSION: //projektil prave exploduje
		if (SDL_GetTicks() >= destroy_time) //cas exploze vyprsel
		{
			state = DESTROYED; //projektil je znicen
		}
		break;
	case DESTROYED: //projektil je znicen, nedela se nic
		return;
		break;
	}
}




