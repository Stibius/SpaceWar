
/** \file Player.cpp
*   \brief Obsahuje definice metod tridy Player.
*/

#include <cmath>
#include "Circle.h"
#include "Events.h"
#include "Player.h"
#include "Projectile.h"

int Player::Weapon::energy_usage[Settings::NUM_WEAPONS];
int Player::Weapon::rate[Settings::NUM_PROJECTILE_WEAPONS];
Settings::weapons_t Player::Weapon::default_weapon;
int Player::Weapon::max_energy;
int Player::Weapon::energy_regeneration_rate;
int Player::Weapon::energy_regeneration_amount;
int Player::Weapon::beam_damage;
const string Player::Weapon::laser_shot_sound_file = "laser_shot.wav";
const string Player::Weapon::rocket_shot_sound_file = "rocket_shot.wav";
const string Player::Weapon::beam_sound_file = "beam.wav";
const string Player::Weapon::beam_hit_sound_file = "beam_hit.wav";

int Player::num_instances = 0;        
bool Player::initialized = false;
int Player::max_health; 
int Player::max_shield;
int Player::shield_regeneration_rate;
int Player::shield_regeneration_amount;
GLuint Player::acceleration_list;                 
GLuint Player::ship_list;
GLuint Player::deceleration_list;
const GLfloat Player::EXPLOSION_RADIUS = 70.0f;
const GLfloat Player::RADIUS = 36.0f;

int Player::mouse_controls[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS];
bool Player::mouse[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS];

GLfloat Player::acceleration;
GLfloat Player::deceleration;
SDLKey Player::key_controls[Settings::NUM_PLAYERS][Settings::NUM_SHIP_CONTROLS];

const string Player::jet_sound_file = "jet.wav";
const string Player::explosion_sound_file = "explosion.wav";

Player::Weapon::Weapon(const Settings & settings, Player & player, Settings::weapons_t type) 
	: settings(&settings), player(&player), triple(false), guided(false), firing(false), beam_active(false), list_drawn(false), 
	triple_remaining_time(0), guided_remaining_time(0), shots_counter(0),
	laser_shot_sound(laser_shot_sound_file), rocket_shot_sound(rocket_shot_sound_file), beam_sound(beam_sound_file), beam_hit_sound(beam_hit_sound_file)
{ 
	beam_lengths[LEFT_BEAM] = 0;
	beam_lengths[CENTER_BEAM] = 0;
	beam_lengths[RIGHT_BEAM] = 0;
	Set_Settings(settings); 
	Set_Type(default_weapon); 
	Draw_Beam_End_List(); 
}

Player::Weapon::Weapon(const Weapon & weapon)
{
	energy = weapon.energy; 
	type = weapon.type; 
	beam_active = weapon.beam_active; 
	beam_lengths[LEFT_BEAM] = weapon.beam_lengths[LEFT_BEAM];
	beam_lengths[CENTER_BEAM] = weapon.beam_lengths[CENTER_BEAM];
	beam_lengths[RIGHT_BEAM] = weapon.beam_lengths[RIGHT_BEAM];
	shots_counter = weapon.shots_counter; 
	triple = weapon.triple; 
	guided = weapon.guided; 
	firing = weapon.firing;
	triple_remaining_time = weapon.triple_remaining_time; 
	guided_remaining_time = weapon.guided_remaining_time; 
	last_shot = weapon.last_shot; 
	player = weapon.player; 
	settings = weapon.settings; 
	laser_shot_sound = weapon.laser_shot_sound; 
    rocket_shot_sound = weapon.rocket_shot_sound; 
	beam_sound = weapon.beam_sound;
	beam_hit_sound = weapon.beam_hit_sound;
	list_drawn = false;

	Draw_Beam_End_List();
}

Player::Weapon & Player::Weapon::operator=(const Weapon & weapon)
{
	energy = weapon.energy; 
	type = weapon.type; 
	beam_active = weapon.beam_active; 
	beam_lengths[LEFT_BEAM] = weapon.beam_lengths[LEFT_BEAM];
	beam_lengths[CENTER_BEAM] = weapon.beam_lengths[CENTER_BEAM];
	beam_lengths[RIGHT_BEAM] = weapon.beam_lengths[RIGHT_BEAM];
	shots_counter = weapon.shots_counter; 
	triple = weapon.triple; 
	guided = weapon.guided; 
	firing = weapon.firing;
	triple_remaining_time = weapon.triple_remaining_time; 
	guided_remaining_time = weapon.guided_remaining_time; 
	last_shot = weapon.last_shot; 
	player = weapon.player; 
	settings = weapon.settings; 
	laser_shot_sound = weapon.laser_shot_sound; 
    rocket_shot_sound = weapon.rocket_shot_sound; 
	beam_sound = weapon.beam_sound;
	beam_hit_sound = weapon.beam_hit_sound;

	Draw_Beam_End_List();

	return *this;
}

Player::Weapon::~Weapon() 
{ 
	if (list_drawn) 
	{ 
		glDeleteLists(beam_end_list, 1); 
	} 

	beam_sound.Stop(); 
	beam_hit_sound.Stop(); 
}

void Player::Weapon::Reset()
{
	triple = false;
	guided = false;
	firing = false;
	triple_remaining_time = 0;
	guided_remaining_time = 0;
	Set_Type(default_weapon);
	beam_active = false;
	beam_lengths[LEFT_BEAM] = 0;
	beam_lengths[CENTER_BEAM] = 0;
	beam_lengths[RIGHT_BEAM] = 0;
	beam_sound.Stop();
	beam_hit_sound.Stop();
}

void Player::Weapon::Set_Settings(const Settings & settings)
{
	default_weapon = settings.Get_Setting_Value(Settings::DEFAULT_WEAPON);

	beam_damage = settings.Get_Setting_Value(Settings::LASER_BEAM_DAMAGE);

	energy_usage[Settings::LASER_WEAPON] = settings.Get_Setting_Value(Settings::LASER_ENERGY_USAGE);
	energy_usage[Settings::ROCKET_WEAPON] = settings.Get_Setting_Value(Settings::ROCKET_ENERGY_USAGE);
	energy_usage[Settings::LASER_BEAM_WEAPON] = settings.Get_Setting_Value(Settings::LASER_BEAM_ENERGY_USAGE);

	rate[Settings::LASER_WEAPON] = settings.Get_Setting_Value(Settings::LASER_RATE);
	rate[Settings::ROCKET_WEAPON] = settings.Get_Setting_Value(Settings::ROCKET_RATE);

	max_energy = settings.Get_Setting_Value(Settings::MAX_ENERGY);
	energy_regeneration_rate = settings.Get_Setting_Value(Settings::ENERGY_REGENERATION_INTERVAL);
	energy_regeneration_amount = settings.Get_Setting_Value(Settings::ENERGY_REGENERATION_AMOUNT);

	if (triple_remaining_time > Uint32(settings.Get_Setting_Value(Settings::TRIPLE_POWERUP_DURATION)) * 1000)
	{
		triple_remaining_time = settings.Get_Setting_Value(Settings::TRIPLE_POWERUP_DURATION) * 1000;
	}

	if (guided_remaining_time > Uint32(settings.Get_Setting_Value(Settings::GUIDED_POWERUP_DURATION)) * 1000)
	{
		guided_remaining_time = settings.Get_Setting_Value(Settings::GUIDED_POWERUP_DURATION) * 1000;
	}

	Draw_Beam_End_List();
}

Point Player::Weapon::Get_Beam_Spawn_Point(beams_t beam) const
{
	Point original_tip = player->Get_Tip();

	switch (beam)
	{
	case LEFT_BEAM:
		original_tip.x -= TRIPLE_BEAM_GAP;
		original_tip.y += SIDE_BEAM_GAP;
		break;
	case CENTER_BEAM:
		original_tip.y -= CENTER_BEAM_GAP;
		break;
	case RIGHT_BEAM:
		original_tip.x += TRIPLE_BEAM_GAP;
		original_tip.y += SIDE_BEAM_GAP;
		break;
	default:
		break;
	}

	Point result = Math::Transform_Point(original_tip, player->Get_Center_X(), player->Get_Center_Y(), player->Get_Angle());

	int possible_result_found = false;

	if (result.x >= 0.0f && result.y >= 0.0f)
	{
		possible_result_found = true;
	}

	//z tech spicek aktivovanych dvojniku, ktere nejsou v minusu, se vybere ta s nejmensi vzdalenosti od pocatku (je na herni plose)
	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++) 
	{
		doubles_positions pos = doubles_positions(i);

		if (player->Double_Activated(pos))
		{
			Point double_tip = original_tip;

			switch (beam)
	        {
	        case LEFT_BEAM:
		        double_tip.x -= TRIPLE_BEAM_GAP;
		        double_tip.y += SIDE_BEAM_GAP;
		        break;
	        case CENTER_BEAM:
				original_tip.y -= CENTER_BEAM_GAP;
	         	break;
	        case RIGHT_BEAM:
	         	double_tip.x += TRIPLE_BEAM_GAP;
	        	double_tip.y += SIDE_BEAM_GAP;
	         	break;
	        default:
	         	break;
	        }

			double_tip = Math::Transform_Point(double_tip, player->Get_Center_X() + player->Get_Double_Translation_X(pos), player->Get_Center_Y() + player->Get_Double_Translation_Y(pos), player->Get_Angle());
	     
			if (double_tip.x >= 0.0f && double_tip.y >= 0.0f)
			{
				if (possible_result_found == false)
				{
					result = double_tip;
					possible_result_found = true;
				}
				else
				{
					if (double_tip.x < result.x || double_tip.y < result.y)
					{
						result = double_tip;
					}
				}
			}
		}
	}
	
	return result;
}

Point Player::Weapon::Get_Projectile_Spawn_Point() const
{
	Point original_tip = player->Get_Tip();
	Point result = Math::Transform_Point(original_tip, player->Get_Center_X(), player->Get_Center_Y(), player->Get_Angle());

	int possible_result_found = false;

	if (result.x >= 0.0f && result.y >= 0.0f)
	{
		possible_result_found = true;
	}

	//z tech spicek aktivovanych dvojniku, ktere nejsou v minusu, se vybere ta s nejmensi vzdalenosti od pocatku (je na herni plose)
	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++) 
	{
		doubles_positions pos = doubles_positions(i);

		if (player->Double_Activated(pos))
		{
			Point double_tip = original_tip;
			double_tip = Math::Transform_Point(double_tip, player->Get_Center_X() + player->Get_Double_Translation_X(pos), player->Get_Center_Y() + player->Get_Double_Translation_Y(pos), player->Get_Angle());
	     
			if (double_tip.x >= 0.0f && double_tip.y >= 0.0f)
			{
				if (possible_result_found == false)
				{
					result = double_tip;
					possible_result_found = true;
				}
				else
				{
					if (double_tip.x < result.x || double_tip.y < result.y)
					{
						result = double_tip;
					}
				}
			}
		}
	}
	
	return result;
}

void Player::Weapon::Draw_Beam_End_List()
{
	if (player != NULL)
	{
		if (list_drawn)
	    {
	        glDeleteLists(beam_end_list, 1);
	    }

	    Circle circle = Circle(30.0f, Point(0.0f, 0.0f), player->Get_Color());
		int i = 1;
	    while (glIsList(i))
		    i++;
	    beam_end_list = i;
	    glNewList(beam_end_list, GL_COMPILE);
	        circle.Draw(1.0f, 0.2f);
	    glEndList();
		list_drawn = true;
	}
}

bool Player::Weapon::Fire_Beams(Uint32 elapsed_time)
{
    if (player == NULL) return false;

	if (energy > 0)
	{
		static float energy_loss = 0;
		energy_loss += energy_usage[type] * (elapsed_time / 1000.0f);
		if (energy_loss >= 1.0f)
		{
			Drain_Energy(1);
		    energy_loss -= 1;
		}

		return true;
	}

	return false;
}

bool Player::Weapon::Fire_Projectile() 
{	
	if (player == NULL) return false;

	Uint32 elapsed_time = SDL_GetTicks() - last_shot; //doba od posledniho vystrelu v milisekundach

	if (elapsed_time >= unsigned int(rate[type]) && energy >= energy_usage[type]) //pokud od posledniho vystrelu ubehl dostatecny cas a je dost energie, je mozno znovu vystrelit
	{
		Drain_Energy(energy_usage[type]);
	    shots_counter++;
	
	    Point spawn_point = Get_Projectile_Spawn_Point();
	    GLfloat ang = player->Get_Angle();

	    if (!triple)
	    {
	        Projectile* p = new Projectile(*settings, player->Get_Player_Number(), type, guided, false, shots_counter, spawn_point, ang);
		    p->Move((p->Get_Length() / 2) + 5, ang); //aby nekolidoval s hracem, ktery ho vystrelil
		    Send_Event(p);
	    }
	    else
	    {
		    Projectile* p1 = new Projectile(*settings, player->Get_Player_Number(), type, guided, true, shots_counter, spawn_point, ang);		    
		    p1->Move((p1->Get_Length() / 2) + 5, ang); //aby nekolidoval s hracem, ktery ho vystrelil
		    p1->Move(p1->Get_Width()+TRIPLE_PROJECTILE_GAP, ang-90.0f); //levy
		    Send_Event(p1);

		    //prostredni
		    Projectile* p2 = new Projectile(*settings, player->Get_Player_Number(), type, guided, true, shots_counter, spawn_point, ang);
		    p2->Move((p2->Get_Length() / 2) + 5, ang); //aby nekolidoval s hracem, ktery ho vystrelil
		    Send_Event(p2);

		    Projectile* p3 = new Projectile(*settings, player->Get_Player_Number(), type, guided, true, shots_counter, spawn_point, ang);
		    p3->Move((p3->Get_Length() / 2) + 5, ang); //aby nekolidoval s hracem, ktery ho vystrelil
		    p3->Move(p3->Get_Width()+TRIPLE_PROJECTILE_GAP, ang+90.0f); //pravy
		    Send_Event(p3);
	    }
	    
	    last_shot = SDL_GetTicks(); //nova hodnota casu posledniho vystrelu

		return true;
	} 

	return false;
}

void Player::Weapon::Send_Event(Projectile* projectile) const
{
	SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = USR_EVT_FIRE;
    event.user.data1 = projectile;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
}

void Player::Weapon::Set_Type(Settings::weapons_t type)
{
	this->type = type;
	energy = max_energy;
	last_shot = 0; 
	beam_active = false;

	if (type == Settings::LASER_BEAM_WEAPON)
	{
	    guided_remaining_time = 0;
		guided = false;
	}
}

void Player::Weapon::Draw_Beams() const
{
	if (beam_active == true)
	{
		for (int beam = LEFT_BEAM; beam <= RIGHT_BEAM; beam++)
		{
			if (triple == false && beam != CENTER_BEAM)
			{
				continue;
			}

			glPushMatrix();
	        glLoadIdentity();

		    Point p = Get_Beam_Spawn_Point(beams_t(beam));
	        glTranslatef(p.x, p.y, 0.0f);
	        glRotatef(player->Get_Angle(), 0.0f, 0.0f, 1.0f);
		    const Color & color = player->Get_Color();

		    //prostredek paprsku, nulova pruhlednost
		    glColor4f(color.red, color.green, color.blue, 1.0f);
		    glBegin(GL_QUADS);
	            glVertex2f(-0.5f, 0.0f);                      
                glVertex2f(-0.5f, -beam_lengths[beam]);    
                glVertex2f(0.5f, -beam_lengths[beam]);
		        glVertex2f(0.5f, 0.0f);
	        glEnd();

		    //leva strana paprsku, postupne prechazi ve zcela pruhledny
		    glBegin(GL_QUADS);
	            glVertex2f(-0.5f, 0.0f);                      
                glVertex2f(-0.5f, -beam_lengths[beam]);  
			    glColor4f(color.red, color.green, color.blue, 0.0f);
                glVertex2f(-3.0f, -beam_lengths[beam]);
		        glVertex2f(-3.0f, 0.0f);
	        glEnd();

		    //prava strana paprsku, postupne prechazi ve zcela pruhledny
		    glColor4f(color.red, color.green, color.blue, 1.0f);
		    glBegin(GL_QUADS);
	            glVertex2f(0.5f, 0.0f);                      
                glVertex2f(0.5f, -beam_lengths[beam]);   
			    glColor4f(color.red, color.green, color.blue, 0.0f);
                glVertex2f(3.0f, -beam_lengths[beam]);
		        glVertex2f(3.0f, 0.0f);
	        glEnd();

		    glTranslatef(0.0f, -beam_lengths[beam], 0.0f);

		    glCallList(beam_end_list);
			
			glPopMatrix();
		}
	}
}

void Player::Weapon::Set_Beam_Length(beams_t beam, GLfloat beam_length) 
{ 
	this->beam_lengths[beam] = beam_length; 
	if (this->beam_lengths[beam] > MAX_BEAM_LENGTH) 
	{
		this->beam_lengths[beam] = MAX_BEAM_LENGTH; 
	}
}

line_t Player::Weapon::Get_Beam_Line(beams_t beam) const
{
	line_t beam_line;
	
	beam_line.first = Get_Beam_Spawn_Point(beam);

	beam_line.second = Math::Get_Circle_Angle_Point(beam_lengths[beam], player->Get_Angle());
	beam_line.second.x += player->Get_Center_X();
	beam_line.second.y += player->Get_Center_Y();

	beam_line.first.x = Math::Round(beam_line.first.x);
	beam_line.first.y = Math::Round(beam_line.first.y);
	beam_line.second.x = Math::Round(beam_line.second.x);
	beam_line.second.y = Math::Round(beam_line.second.y);
	
	return beam_line;
}


void Player::Weapon::Update(GLfloat fps)
{
	Uint32 miliseconds = Uint32(1000 / fps); //pocet milisekund mezi dvema snimky

	if (triple_remaining_time <= miliseconds) 
	{
		triple_remaining_time = 0;
		triple = false;
	}
	else
	{
		triple_remaining_time -= miliseconds;
	}

	if (guided_remaining_time <= miliseconds) 
	{
		guided_remaining_time = 0;
		guided = false;
	}
	else
	{
		guided_remaining_time -= miliseconds;
	}
	
	//regenerace energie
	if (energy != max_energy)
	{
		static Uint32 next_regeneration = 0;
		if (next_regeneration == 0)
			next_regeneration = SDL_GetTicks() + energy_regeneration_rate;
		if (SDL_GetTicks() >= next_regeneration)
		{
		    energy += energy_regeneration_amount;
			if (energy >= max_energy) 
			{ 
				energy = max_energy;
				next_regeneration = 0;
			}
			else
			{
				next_regeneration = SDL_GetTicks() + energy_regeneration_rate;
			}
		}
	}

	if (firing == true)
	{
		switch (type)
		{
		case Settings::LASER_BEAM_WEAPON:
			beam_active = Fire_Beams(miliseconds);
			break;
		case Settings::LASER_WEAPON:
			beam_active = false;
		    if (Fire_Projectile())
			{
			    laser_shot_sound.Play(0);
			}
			break;
		case Settings::ROCKET_WEAPON:
			beam_active = false;
		    if (Fire_Projectile())
			{
			    rocket_shot_sound.Play(0);
			}
			break;
		default:
			break;
		}
	}
	else 
	{
		beam_active = false;
	}

	if (beam_active)
	{
		if ((beam_lengths[LEFT_BEAM] == 0 || beam_lengths[LEFT_BEAM] == MAX_BEAM_LENGTH) &&
			(beam_lengths[CENTER_BEAM] == 0 || beam_lengths[CENTER_BEAM] == MAX_BEAM_LENGTH) &&
			(beam_lengths[RIGHT_BEAM] == 0 || beam_lengths[RIGHT_BEAM] == MAX_BEAM_LENGTH))
		{   //laserove paprsky s nicim nekoliduji
			if (!beam_sound.Is_Playing())
		    {
			    beam_sound.Play(-1);
		    }

		    if (beam_hit_sound.Is_Playing())
		    {
			    beam_hit_sound.Stop();
		    }
		}
		else //nektery laserovy paprsek neco zasahuje
		{
			if (beam_sound.Is_Playing())
		    {
			    beam_sound.Stop();
		    }

		    if (!beam_hit_sound.Is_Playing())
		    {
			    beam_hit_sound.Play(-1);
		    }
		}
	}
	else //laserove paprsky nejsou aktivni
	{
		beam_sound.Stop();
		beam_hit_sound.Stop();
	} 
}

void Player::Add_Health(int health) 
{ 
	this->health += health; 
	if (this->health > max_health)
	{
		this->health = max_health;
	}
}

void Player::Add_Shield(int shield) 
{ 
	this->shield += shield; 
	if (this->shield > max_shield)
	{ 
		this->shield = max_shield;
	}
}

void Player::Add_Energy(int energy) 
{ 
	weapon.Add_Energy(energy); 
}

void Player::Set_Weapon(Settings::weapons_t type) 
{ 
	weapon.Set_Type(type); 
}

void Player::Weapon::Add_Energy(int energy)
{
	this->energy += energy; 
	if (this->energy > max_energy)
	{ 
		this->energy = max_energy;
	}
}

void Player::Weapon::Drain_Energy(int energy)
{
	this->energy -= energy; 
	if (this->energy < 0)
	{ 
		this->energy = 0;
	}
}

void Player::Damage(int damage)
{
	if (shield > 0) //pokud je stit, poskozuje se nejdriv ten
	{
		shield -= damage;
		if (shield < 0) //zbyvajici poskozeni jde do zdravi
		{
			health += shield;
			shield = 0;
		}
	}
	else //neni stit, vsechno poskozeni jde do zdravi
	{
		health -= damage;
	}

	if (health <= 0) //lod byla znicena
	{
		Destroy();
	}
}

void Player::Destroy()
{
	if (state == NORMAL)
	{
		health = 0;
		shield = 0;
		state = EXPLOSION;
		destroy_time = SDL_GetTicks() + EXPLOSION_DURATION;

		if (jet_sound.Is_Playing())
		{
			jet_sound.Pause();
		}
		explosion_sound.Play(0);

		weapon.Reset();
	}
}

void Player::Draw() const
{
	switch (state)
	{
	case NORMAL: //lod neni znicena
		//vykresleni originalu
		glPushMatrix();
	    glLoadIdentity();

	    glTranslatef(Get_Center_X(), Get_Center_Y(), 0.0f);
	    glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);
		glColor4f(color.red, color.green, color.blue, 0.2f);
	    glCallList(ship_list);
		if (accelerating) 
		{
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glCallList(acceleration_list);
		}
		if (decelerating) 
		{
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glCallList(deceleration_list);
		}
		glLoadIdentity();

		//vykresleni dvojniku
		for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	    {
			doubles_positions current_double = doubles_positions(i);

		    if (Double_Activated(current_double))
		    {
		
			    glTranslatef(Get_Center_X() + Get_Double_Translation_X(current_double), Get_Center_Y() + Get_Double_Translation_Y(current_double), 0.0f);
                glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);
			    glColor4f(color.red, color.green, color.blue, 0.2f);
	            glCallList(ship_list);
		        if (accelerating) 
		        {
			        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			        glCallList(acceleration_list);
		        }
				else if (decelerating) 
		        {
			        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			        glCallList(deceleration_list);
		        }
	            glLoadIdentity();	
			}
		}
		glPopMatrix();
		weapon.Draw_Beams();
		break;
	case EXPLOSION: //lod prave exploduje
		Draw_Explosion();
		break;
    case DESTROYED: //exploze uz zkoncila
		break;
	default:
		break;
	}
}

void Player::Draw_Explosion() const
{
	if (SDL_GetTicks() >= destroy_time) return;

	Uint32 remaining_time = destroy_time - SDL_GetTicks(); //cas zbyvajici do konce exploze

	//pruhlednost se s casem zvysuje tak, aby v case konce exploze byla 100% (postupne mizeni exploze)
	GLfloat ct = (GLfloat(remaining_time) / EXPLOSION_DURATION); //hodnota alpha (pruhlednost) 

	//vykresleni exploze projektilu
	glColor4f(1.0f, 1.0f, 0.0f, ct);
	Circle::Draw(Point(Get_Center()), EXPLOSION_RADIUS, true);	

	//vykresleni explozi aktivovanych dvojniku projektilu
	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		doubles_positions current_double = doubles_positions(i);

		if (Double_Activated(current_double))
		{
			glColor4f(1.0f, 1.0f, 0.0f, ct);
	        Circle::Draw(Point(Get_Center_X() + Get_Double_Translation_X(current_double),  Get_Center_Y() + Get_Double_Translation_Y(current_double)), EXPLOSION_RADIUS, true);
        }
    }	
}

void Player::Draw_Deceleration_Jet() const
{
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	    glVertex2f(19.0f, -0.5f);                      
        glVertex2f(25.0f, -0.5f); 
		glVertex2f(22.0f, -6.0f);

		glVertex2f(-19.0f, -0.5f);                      
        glVertex2f(-25.0f, -0.5f); 
		glVertex2f(-22.0f, -6.0f);
	glEnd();
}

void Player::Draw_Acceleration_Jet() const
{
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	    glVertex2f(-6.0f, 37.0f);                      
        glVertex2f(6.0f, 37.0f); 
		glVertex2f(0.0f, 45.0f);
	glEnd();
}

void Player::Draw_Ship() const
{
	glLineWidth(2.0f);

	//vykresli se zvlast obrys a vnitrek
	for (int i = 0; i < 2; i++)
	{
		if (i == 0) //obrys
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			//prave kridlo
			glBegin(GL_LINE_LOOP);
	            glVertex2f(10.0f, -10.0f);                      
                glVertex2f(13.0f, -10.0f); 
		        glVertex2f(30.0f, 20.0f);
			    glVertex2f(30.0f, 25.0f); 
				glVertex2f(26.0f, 30.0f);
				glVertex2f(6.0f, 30.0f); 
				glVertex2f(12.0f, 9.0f);
                glVertex2f(12.0f, -3.0f); 
	        glEnd();

			glBegin(GL_TRIANGLES);
			 
	            glVertex2f(19.0f, 0.5f);                      
                glVertex2f(25.0f, 11.0f); 
		        glVertex2f(25.0f, 0.5f);
	        glEnd();

			//leve kridlo
			glBegin(GL_LINE_LOOP);
	            glVertex2f(-10.0f, -10.0f);                      
                glVertex2f(-13.0f, -10.0f); 
		        glVertex2f(-30.0f, 20.0f);
			    glVertex2f(-30.0f, 25.0f); 
				glVertex2f(-26.0f, 30.0f);
				glVertex2f(-6.0f, 30.0f); 
				glVertex2f(-12.0f, 9.0f);
                glVertex2f(-12.0f, -3.0f); 
	        glEnd();

			glBegin(GL_TRIANGLES);
			 
	            glVertex2f(-19.0f, 0.5f);                      
                glVertex2f(-25.0f, 11.0f); 
		        glVertex2f(-25.0f, 0.5f);
	        glEnd();
	    }
		else //vnitrek
		{
			glEnable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//prave kridlo
			glBegin(GL_TRIANGLE_STRIP);
	            glVertex2f(10.0f, -10.0f);                      
                glVertex2f(13.0f, -10.0f);              
                glVertex2f(12.0f, -3.0f);
		        glVertex2f(30.0f, 20.0f);
				glVertex2f(12.0f, 9.0f);
				glVertex2f(30.0f, 25.0f);                      
                glVertex2f(6.0f, 30.0f);              
                glVertex2f(26.0f, 30.0f);
	        glEnd();

			//leve kridlo
		    glBegin(GL_TRIANGLE_STRIP);
	            glVertex2f(-10.0f, -10.0f);                      
                glVertex2f(-13.0f, -10.0f);              
                glVertex2f(-12.0f, -3.0f);
		        glVertex2f(-30.0f, 20.0f);
				glVertex2f(-12.0f, 9.0f);
				glVertex2f(-30.0f, 25.0f);                      
                glVertex2f(-6.0f, 30.0f);              
                glVertex2f(-26.0f, 30.0f);
	        glEnd();
		}	
		
		//spicka
		glBegin(GL_POLYGON);
	        glVertex2f(0.0f, -36.0f);                      
            glVertex2f(3.0f, -34.0f);              
            glVertex2f(4.0f, -30.0f);
		    glVertex2f(-4.0f, -30.0f);
		    glVertex2f(-3.0f, -34.0f);
	    glEnd();

	    //trup
		glBegin(GL_POLYGON);
	        glVertex2f(-4.0f, -30.0f);                      
            glVertex2f(4.0f, -30.0f);              
            glVertex2f(12.0f, -3.0f);
		    glVertex2f(12.0f, 9.0f);               
            glVertex2f(6.0f, 30.0f);              
		    glVertex2f(-6.0f, 30.0f);    
            glVertex2f(-12.0f, 9.0f);
		    glVertex2f(-12.0f, -3.0f);
		    glVertex2f(-4.0f, -30.0f);
	    glEnd();

		//zadek
		glBegin(GL_POLYGON);                    
            glVertex2f(-6.0f, 30.0f);              
            glVertex2f(6.0f, 30.0f);
		    glVertex2f(6.0f, 36.0f);
		    glVertex2f(-6.0f, 36.0f);
	    glEnd();
	}
}

polygon_t Player::Get_Border(doubles_positions double_pos) const
{
	polygon_t transformed_border; //zde nakonec bude vysledny polygon

	if (double_pos >= FIRST_DOUBLE && !Double_Activated(double_pos))
	{
		return transformed_border; //je zadany dvojnik, ktery neni aktivovany, vraci se prazdny polygon
	}

	//zjisti aktualni pozici projektilu (originalu)
	GLfloat x = Get_Center_X();
	GLfloat y = Get_Center_Y();

	//pretransformuje MODELVIEW matici tak, aby odpovidala aktualni transformacni matici projektilu nebo pozadovaneho dvojnika
	if (double_pos == ORIGINAL)
	{
	    glTranslatef(x, y, 0.0f); 
	}
	else 
	{
		glTranslatef(x + Get_Double_Translation_X(double_pos), y + Get_Double_Translation_Y(double_pos), 0.0f);
	}
	//glScalef(Get_Scale(), Get_Scale(), 1);
	glRotatef(Get_Angle(), 0.0f, 0.0f, 1.0f);

	//prekopiruje MODELVIEW matici do pole matrix
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	glLoadIdentity();

	transformed_border = border; 

	//pretransformovani hranicniho polygonu na zaklade transformacni matice projektilu
	for (unsigned int i = 0; i < transformed_border.size(); i++)
	{
		transformed_border[i] = Math::Transform_Point(border[i], matrix);
	}
	
	return transformed_border;
}

vector<polygon_t> Player::Get_Borders() const
{
	vector<polygon_t> borders;

	borders.push_back(Get_Border(ORIGINAL));

	for (int i = FIRST_DOUBLE; i < NUM_DOUBLES; i++)
	{
		doubles_positions pos = doubles_positions(i);

		if (Double_Activated(pos))
		{
			polygon_t border = Get_Border(pos);
			borders.push_back(border);
		}
	}

	return borders;
}

Player::Player(const Settings & settings, Settings::players_t pl, Point start_position, GLfloat start_angle) 
	: Moving_Object(start_position, start_angle, GLfloat(settings.Get_Setting_Value(Settings::MAX_SPEED)), GLfloat(settings.Get_Setting_Value(Settings::PLAYER_ROTATION_SPEED)), GLfloat(settings.Get_Setting_Value(Settings::PLAYER_RESISTANCE))),
	player_number(pl), score(0), start_position(start_position), start_angle(start_angle), destroy_time(0), state(NORMAL), accelerating(false), decelerating(false)
{
	num_instances++;
	Set_Settings(settings);
	health = max_health; 
	shield = max_shield;
	jet_sound.Load_Sound(jet_sound_file);
	explosion_sound.Load_Sound(explosion_sound_file);
	weapon = Weapon(settings, *this);
	
	border.push_back(Point(0.0f, -36.0f));
	border.push_back(Point(3.0f, -34.0f));
	border.push_back(Point(4.0f, -30.0f));
	border.push_back(Point(10.0f, -10.0f));
	border.push_back(Point(13.0f, -10.0f));
	border.push_back(Point(30.0f, 20.0f));
	border.push_back(Point(30.0f, 25.0f));
	border.push_back(Point(26.0f, 30.0f));
	border.push_back(Point(6.0f, 30.0f));
	border.push_back(Point(6.0f, 36.0f));
	border.push_back(Point(-6.0f, 36.0f));
	border.push_back(Point(-6.0f, 30.0f));
	border.push_back(Point(-26.0f, 30.0f));
	border.push_back(Point(-30.0f, 25.0f));
	border.push_back(Point(-30.0f, 20.0f));
	border.push_back(Point(-13.0f, -10.0f));
	border.push_back(Point(-10.0f, -10.0f));
	border.push_back(Point(-4.0f, -30.0f));
	border.push_back(Point(-3.0f, -34.0f));

	Set_Radius(RADIUS);

	if (initialized == false)
	{
		Draw_Lists();
		initialized = true;
	}
}

void Player::Draw_Lists()
{
	int i = 1;
	while (glIsList(i))
		i++;
	ship_list = i;

	glNewList(ship_list, GL_COMPILE); 
	    Draw_Ship();
	glEndList();

	while (glIsList(i))
		i++;
	acceleration_list = i;

	glNewList(acceleration_list, GL_COMPILE); 
	    Draw_Acceleration_Jet();
	glEndList();

	while (glIsList(i))
		i++;
	deceleration_list = i;

	glNewList(deceleration_list, GL_COMPILE); 
	    Draw_Deceleration_Jet();
	glEndList();
}

Player & Player::operator=(const Player & obj)
{
	if (this == &obj)    //object assigned to itself
        return *this;    //all done

	Moving_Object::operator=(obj);

	jet_sound = obj.jet_sound;
	explosion_sound = obj.explosion_sound;
	border = obj.border;
	destroy_time = obj.destroy_time;
	health = obj.health;
	player_number = obj.player_number;
	score = obj.score;
	shield = obj.shield;
	start_position = obj.start_position;
	start_angle = obj.start_angle;
	state = obj.state;
	weapon = obj.weapon;
    weapon.Set_Player(*this);
	color = obj.color;
	accelerating = obj.accelerating;
	decelerating = obj.decelerating;
	mouse_turning = obj.mouse_turning;

	return *this;
}

Player::Player(const Player & obj) 
	: Moving_Object(obj), color(obj.color), jet_sound(obj.jet_sound), explosion_sound(obj.explosion_sound), border(obj.border), destroy_time(obj.destroy_time),
	health(obj.health), player_number(obj.player_number), score(obj.score), shield(obj.shield), start_position(obj.start_position), start_angle(obj.start_angle),
	state(obj.state), weapon(obj.weapon), accelerating(obj.accelerating), decelerating(obj.decelerating), mouse_turning(obj.mouse_turning)
{
	num_instances++;
    weapon.Set_Player(*this);
}

Player::~Player()
{
	num_instances--;
    if (num_instances == 0 && initialized == true)
	{
		glDeleteLists(ship_list, 1);	
		glDeleteLists(acceleration_list, 1);
		glDeleteLists(deceleration_list, 1);
		initialized = false;
	}
}

void Player::Set_Settings(const Settings & settings)
{
	Set_Max_Speed_Value(GLfloat(settings.Get_Setting_Value(Settings::MAX_SPEED)));
	Set_Rotation_Speed(GLfloat(settings.Get_Setting_Value(Settings::PLAYER_ROTATION_SPEED)));

	for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	{
		for (int j = 0; j < Settings::NUM_SHIP_CONTROLS; j++)
	    { 
			if (settings.Get_Control_Type(Settings::controls_t(j), Settings::players_t(i)) == Settings::KEY)
		    {
		        key_controls[Settings::players_t(i)][j] = settings.Get_Key(Settings::controls_t(j), Settings::players_t(i));
			    mouse[Settings::players_t(i)][j] = false;
		    }
		    else
		    {
			    mouse_controls[Settings::players_t(i)][j] = settings.Get_Mouse_Button(Settings::controls_t(j), Settings::players_t(i));
			    mouse[Settings::players_t(i)][j] = true;
		    }
		}
	}
	
	acceleration = GLfloat(settings.Get_Setting_Value(Settings::ACCELERATION));
	deceleration = GLfloat(settings.Get_Setting_Value(Settings::DECELERATION));
	
	max_health = settings.Get_Setting_Value(Settings::MAX_HEALTH);
	max_shield = settings.Get_Setting_Value(Settings::MAX_SHIELD);
	shield_regeneration_rate = settings.Get_Setting_Value(Settings::SHIELD_REGENERATION_INTERVAL);
	shield_regeneration_amount = settings.Get_Setting_Value(Settings::SHIELD_REGENERATION_AMOUNT);

	if (health > max_health) 
	{
		health = max_health;
	}

	if (shield > max_shield) 
	{
		shield = max_shield;
	}

	if (player_number == Settings::FIRST)
	    color = settings.Get_Setting_Value(Settings::PLAYER1_COLOR);
	else
        color = settings.Get_Setting_Value(Settings::PLAYER2_COLOR);

	if (settings.Get_Setting_Value(Settings::MOUSE_TURNING_PLAYER) == player_number)
	    mouse_turning = true;
	else
		mouse_turning = false;

	Set_Resistance(GLfloat(settings.Get_Setting_Value(Settings::PLAYER_RESISTANCE)));

	weapon.Set_Settings(settings);

	Moving_Object::Set_Doubles_Translations(settings.Get_Resolution());
}

void Player::Update(float fps)
{
	switch (state)
	{
	case NORMAL:
		{
		    SDL_PumpEvents(); //nacteni udalosti ze vstupnich zarizeni

	        Uint8* keys = SDL_GetKeyState(NULL); //vrací ukazatel na vnitøní pole indikátorù klávesnice

		    if (mouse_turning) //otaceni za mysi
		    {
			    int x, y;
		        SDL_GetMouseState(&x, &y);
				GLfloat ang = Math::Get_Direction(Get_Center(), Point(GLfloat(x), GLfloat(y))); //cilovy uhel
			    GLfloat angle = Get_Angle(); //aktualni uhel
			    if (ang > angle)
			    { 
				    //rotace doleva nebo doprava, podle toho, kterym smerem je to bliz do ciloveho uhlu
				    if (angle + 360.0f - ang < ang - angle)
				    {
				        Rotate(LEFT, fps);
					    if ((Get_Angle() > angle && Get_Angle() < ang)) //prekrocili jsme cilovy uhel, navrat zpet na nej
						{
						    Set_Angle(ang);
						}
				    }
				    else
				    {
					    Rotate(RIGHT, fps);
					    if (ang < Get_Angle()) //prekrocili jsme cilovy uhel, navrat zpet na nej
						{
						    Set_Angle(ang);
						}
				    }
			    }
			    else if (ang < Get_Angle())
			    {
				    //rotace doleva nebo doprava, podle toho, kterym smerem je to bliz do ciloveho uhlu
				    if (ang + 360.0f - angle < angle - ang)
				    {
				        Rotate(RIGHT, fps);
					    if ((Get_Angle() < angle && Get_Angle() > ang)) //prekrocili jsme cilovy uhel, navrat zpet na nej
						{
						    Set_Angle(ang);
						}
				    }
				    else
				    {
					    Rotate(LEFT, fps);
					    if (ang > Get_Angle()) //prekrocili jsme cilovy uhel, navrat zpet na nej
						{
						    Set_Angle(ang);
						}
				    }
			    }
		    }
			else //otaceni klavesami nebo tlacitky mysi
			{
		        if (mouse[player_number][Settings::TURN_LEFT])
		        {
			        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mouse_controls[player_number][Settings::TURN_LEFT])) 
			        {
				        Rotate(LEFT, fps);
			        }
		        }
		        else if (keys[key_controls[player_number][Settings::TURN_LEFT]] == SDL_PRESSED) 
			    {
				    Rotate(LEFT, fps);
			    }
		    

		        if (mouse[player_number][Settings::TURN_RIGHT])
		        {
			        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mouse_controls[player_number][Settings::TURN_RIGHT])) 
			        {
				        Rotate(RIGHT, fps);
			        }
		        }
		        else if (keys[key_controls[player_number][Settings::TURN_RIGHT]] == SDL_PRESSED) 
			    {
				    Rotate(RIGHT, fps);
			    }
		    }

		    if (mouse[player_number][Settings::ACCELERATE])
		    {
			    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mouse_controls[player_number][Settings::ACCELERATE]))
			    {
				    Push(acceleration, Get_Angle(), fps);
			        accelerating = true;
			    }
			    else 
			    {
				    accelerating = false;
			    }
		    }
		    else
		    {
			    if (keys[key_controls[player_number][Settings::ACCELERATE]] == SDL_PRESSED)
			    {
				    Push(acceleration, Get_Angle(), fps);
			        accelerating = true;
			    }
			    else 
			    {
				    accelerating = false;
			    }
		    }

		    if (mouse[player_number][Settings::DECELERATE])
		    {
			    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mouse_controls[player_number][Settings::DECELERATE]))
			    {
				    Push(deceleration, Get_Angle()-180.0f, fps);
			        decelerating = true;
			    }
			    else 
			    {
				    decelerating = false;
			    }
		    }
		    else
		    {
			    if (keys[key_controls[player_number][Settings::DECELERATE]] == SDL_PRESSED)
			    {
				    Push(deceleration, Get_Angle()-180.0f, fps);
			        decelerating = true;
			    }
			    else 
			    {
				    decelerating = false;
			    }
		    }

		    if (mouse[player_number][Settings::FIRE])
		    {
			    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mouse_controls[player_number][Settings::FIRE])) 
			    {
					weapon.Fire(true);
				    weapon.Update(fps);
			    }
				else
				{
					weapon.Fire(false);
					weapon.Update(fps);
				}
		    }
		    else
		    {
			    if (keys[key_controls[player_number][Settings::FIRE]] == SDL_PRESSED) 
			    {
				    weapon.Fire(true);
				    weapon.Update(fps);
			    }
				else
				{
					weapon.Fire(false);
					weapon.Update(fps);
				}
		    }

		    if (accelerating || decelerating)
		    {
			    if (!jet_sound.Is_Playing())
			    {
			        if (jet_sound.Is_Paused())
				    {
				        jet_sound.Resume();
				    }
			        else
				    {
				        jet_sound.Play(-1);
				    }
			    }
		    }
		    else if (jet_sound.Is_Playing())
		    {
			    jet_sound.Pause();
		    }
		
		    Slow_Down(fps); //samovolne zpomalovani
	
		    //nastaveni nove polohy v zavislosti na aktualni rychlosti a frameratu
	        Set_Center_X(Get_Center_X() + (Get_Vel_X() / fps));
	        Set_Center_Y(Get_Center_Y() + (Get_Vel_Y() / fps));

		    //regenerace stitu
		    if (shield != 0.0f && shield != max_shield)
		    {
			    static Uint32 next_regeneration = 0;
			    if (next_regeneration == 0)
				    next_regeneration = SDL_GetTicks() + shield_regeneration_rate;
			    if (SDL_GetTicks() >= next_regeneration)
			    {
			        shield += shield_regeneration_amount;
			        if (shield >= max_shield) 
				    { 
					    shield = max_shield;
				        next_regeneration = 0;
				    }
				    else
				    {
					    next_regeneration = SDL_GetTicks() + shield_regeneration_rate;
				    }
			    }
		    }
		}
		break;
	case EXPLOSION:
		if (SDL_GetTicks() >= destroy_time) //cas exploze vyprsel
		{
			state = DESTROYED; //projektil je znicen
		}
		break;
	case DESTROYED:
		Reset();
		break;
	}
}

void Player::Set_Beam_Length(beams_t beam, GLfloat beam_length) 
{ 
	weapon.Set_Beam_Length(beam, beam_length); 
}

void Player::Set_Max_Beam_Length(beams_t beam) 
{ 
	weapon.Set_Max_Beam_Length(beam); 
}

line_t Player::Get_Beam_Line(beams_t beam) const 
{ 
	return weapon.Get_Beam_Line(beam); 
}

void Player::Reset()
{
	Set_Center(start_position);
	Set_Angle(start_angle);
	Set_Vel_X(0.0f);
	Set_Vel_Y(0.0f);
	weapon.Reset();
    destroy_time = 0;
	health = max_health;
	shield = max_shield;
	state = NORMAL;
	accelerating = false;
	decelerating = false;
}

