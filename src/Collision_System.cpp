
/** \file Collision_System.cpp
*   \brief Obsahuje definice metod tridy Collision_System.  
*/

#include "Collision_System.h"
#include "Math.h"
#include "Point.h"
//#include <SDL.h>

using std::make_pair;

Collision_System::Collision_System(const SDL_Rect & resolution, Player & player1, Player & player2, vector<Projectile*> & projectiles, Powerup_System & powerup_system, const Star & star)
		: projectiles(&projectiles), powerups(&powerup_system.powerups), star(&star), resolution(resolution)
{ 
	players[0] = &player1; 
	players[1] = &player2; 
	Set_Margins(resolution); 
}

bool Collision_System::Collide(const Player & player1, const Player & player2) const
{
	if (player1.Get_Mode() != Player::NORMAL || player2.Get_Mode() != Player::NORMAL)
		return false;

	for (int double1 = Moving_Object::ORIGINAL; double1 < Moving_Object::NUM_DOUBLES; double1++)
	{
		if (!player1.Double_Activated(Moving_Object::doubles_positions(double1)))
			continue;
	
		for (int double2 = Moving_Object::ORIGINAL; double2 < Moving_Object::NUM_DOUBLES; double2++)
	    {
			if (!player2.Double_Activated(Moving_Object::doubles_positions(double2)))
			    continue;

			if (Math::Circles_Collision(Circle(player1.Get_Radius(), player1.Get_Double_Position(Moving_Object::doubles_positions(double1))), Circle(player2.Get_Radius(), player2.Get_Double_Position(Moving_Object::doubles_positions(double2)))))
			{
				if (Math::Polygons_Collision(player1.Get_Border(Moving_Object::doubles_positions(double1)), player2.Get_Border(Moving_Object::doubles_positions(double2))))
	            {
		            return true;
	            }
			}
	    }
	}

	return false;	
}

bool Collision_System::Collide(const Player & player, const Projectile & projectile) const
{
	if (player.Get_Mode() != Player::NORMAL || projectile.Get_Mode() != Projectile::NORMAL)
		return false;

	for (int player_double = Moving_Object::ORIGINAL; player_double < Moving_Object::NUM_DOUBLES; player_double++)
	{
		if (!player.Double_Activated(Moving_Object::doubles_positions(player_double)))
			continue;

		if (Math::Circles_Collision(Circle(player.Get_Radius(), player.Get_Double_Position(Moving_Object::doubles_positions(player_double))), Circle(projectile.Get_Radius(), projectile.Get_Center())))
	    {
			if (Math::Polygons_Collision(player.Get_Border(Moving_Object::doubles_positions(player_double)), projectile.Get_Border()))
	        {
		        return true;
	        }
		}
	}

	return false;	
}

bool Collision_System::Collide(const Player & player, const Star & star) const
{

	if (star.Is_Active() == false || player.Get_Mode() != Player::NORMAL) 
		return false;

	for (int player_double = Moving_Object::ORIGINAL; player_double < Moving_Object::NUM_DOUBLES; player_double++)
	{
		if (!player.Double_Activated(Moving_Object::doubles_positions(player_double)))
			continue;

		if (Math::Circles_Collision(Circle(player.Get_Radius(), player.Get_Double_Position(Moving_Object::doubles_positions(player_double))), Circle(star.Get_Radius(), star.Get_Center())))
	    {
			if (Math::Polygon_Circle_Collision(player.Get_Border(Moving_Object::doubles_positions(player_double)), Circle(star.Get_Radius(), star.Get_Center())))
	        {
		        return true;
	        }
		}
	}

	return false;
}

bool Collision_System::Collide(const Player & player, const Powerup_System::Powerup & powerup) const
{
	if (player.Get_Mode() != Player::NORMAL) 
	{
		return false;
	}

	for (int player_double = Moving_Object::ORIGINAL; player_double < Moving_Object::NUM_DOUBLES; player_double++)
	{
		if (!player.Double_Activated(Moving_Object::doubles_positions(player_double)))
			continue;

		if (Math::Circles_Collision(Circle(player.Get_Radius(), player.Get_Double_Position(Moving_Object::doubles_positions(player_double))), Circle(powerup.Get_Radius(), powerup.Get_Center())))
	    {
			if (Math::Polygon_Circle_Collision(player.Get_Border(Moving_Object::doubles_positions(player_double)), Circle(powerup.Get_Radius(), powerup.Get_Center())))
	        {
		        return true;
	        }
		}
	}

	return false;
}

bool Collision_System::Collide(const Projectile & projectile1, const Projectile & projectile2) const
{
	if (!Math::Circles_Collision(Circle(projectile1.Get_Radius(), projectile1.Get_Center()), Circle(projectile2.Get_Radius(), projectile2.Get_Center())))
		return false;

	if (projectile1.Get_Player() == projectile2.Get_Player() && projectile1.Get_Shot_Identifier() == projectile2.Get_Shot_Identifier()) 
	    return false;

	if (projectile1.Get_Mode() != Projectile::NORMAL || projectile2.Get_Mode() != Projectile::NORMAL)
		return false;

	polygon_t border1 = projectile1.Get_Border();
	polygon_t border2 = projectile2.Get_Border();

	if (Math::Polygons_Collision(border1, border2)) 
	{
	    return true;
	}
	
	return false;
}

bool Collision_System::Collide(const Projectile & projectile, const Star & star) const
{
	if (!star.Is_Active() || projectile.Get_Mode() != Projectile::NORMAL)
		return false;

	if (!Math::Circles_Collision(Circle(projectile.Get_Radius(), projectile.Get_Center()), Circle(star.Get_Radius(), star.Get_Center())))
		return false;

	polygon_t border = projectile.Get_Border();

	if (Math::Polygon_Circle_Collision(border, Circle(star.Get_Radius(), star.Get_Center())))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision_System::Collide(const Powerup_System::Powerup & powerup, const Star & star) const
{
	if (!star.Is_Active()) 
		return false;

	if (Math::Circles_Collision(powerup.Get_Circle(), Circle(star.Get_Radius(), star.Get_Center())))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision_System::Collide(const Powerup_System::Powerup & powerup1, const Powerup_System::Powerup & powerup2) const
{
	if (Math::Circles_Collision(powerup1.Get_Circle(), powerup2.Get_Circle()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision_System::Collide(Object & object, margins margin) const
{
	if (Player* ppl = dynamic_cast<Player*>(&object)) 
	{
		if (Math::Line_Circle_Intersections(margin_lines[margin], Circle(ppl->Get_Radius(), ppl->Get_Center()), true, NULL))
		{
			return true;
		}
		else
		{
			return false;
	    }
	}
	else if (Projectile* ppr = dynamic_cast<Projectile*>(&object)) //je to objekt tridy Projectile
	{
		if (Math::Line_Circle_Intersections(margin_lines[margin], Circle(ppr->Get_Radius(), ppr->Get_Center()), true, NULL))
		{
			return true;
		}
		else
		{
			return false;
	    }
	}

	return false;
}

void Collision_System::Set_Margins(const SDL_Rect & resolution)
{
	this->resolution = resolution;

	GLfloat screen_width = resolution.w;
	GLfloat screen_height = resolution.h;

	line_t lines[NUM_MARGINS] = 
	{
		make_pair(Point(-1.0f, -1.0f), Point(-screen_width, -1.0f)),                         //pokracovani horniho okraje za levym okrajem    
		make_pair(Point(-1.0f, -1.0f), Point(-1.0f, -screen_height)),                        //pokracovani leveho okraje za hornim okrajem
		make_pair(Point(0.0f, -1.0f), Point(screen_width-1.0f, -1.0f)),                      //horni okraj
		make_pair(Point(screen_width, -1.0f), Point(screen_width, -screen_height)),          //pokracovani leveho okraje za hornim okrajem
		make_pair(Point(screen_width, -1.0f), Point(2*screen_width, -1.0f)),                 //pokracovani horniho okraje za pravym okrajem
		make_pair(Point(-screen_width, screen_height), Point(-1.0f, screen_height)),         //pokracovani dolniho okraje za levym okrajem
		make_pair(Point(-1.0f, screen_height-1.0f), Point(-1.0f, 0.0f)),                     //levy okraj
		make_pair(Point(0.0f, screen_height), Point(screen_width-1.0f, screen_height)),      //spodni okraj
		make_pair(Point(screen_width, 0.0f), Point(screen_width, screen_height-1.0f)),       //pravy okraj
		make_pair(Point(screen_width, screen_height), Point(2*screen_width, screen_height)), //pokracovani spodniho okraje za pravym okrajem
		make_pair(Point(-1.0f, screen_height), Point(-1.0f, 2*screen_height)),               //pokracovani leveho okraje za spodnim okrajem
		make_pair(Point(screen_width, screen_height), Point(screen_width, 2*screen_height))  //pokracovani praveho okraje za spodnim okrajem
	};
	
	for (int i = 0; i < NUM_MARGINS; i++)
	{
		margin_lines[i] = lines[i];
	}
}

bool Collision_System::Determine_Doubles(Moving_Object & object) const
{
	object.Deactivate_Doubles();

	bool doubles = false; //urcuje, jestli byl aktivovan aspon jeden dvojnik

	for (int i = 0; i < NUM_MARGINS; i++)
	{
		line_t line = margin_lines[i];
		if (Collide(object, margins(i))) 
		{
			doubles = true;
			switch (i)
			{
			    case TOP_LEFT_LEFT_MARGIN: //pokracovani horniho okraje za levym okrajem 
				case TOP_LEFT_TOP_MARGIN:  //pokracovani leveho okraje za hornim okrajem
					object.Activate_Double(Moving_Object::BOTTOM_RIGHT_DOUBLE);
					break;
				case TOP_MARGIN: //horni okraj
					object.Activate_Double(Moving_Object::BOTTOM_DOUBLE);
					break;
                case TOP_RIGHT_TOP_MARGIN:   //pokracovani praveho okraje za hornim okrajem
			    case TOP_RIGHT_RIGHT_MARGIN: //pokracovani horniho okraje za pravym okrajem
					object.Activate_Double(Moving_Object::BOTTOM_LEFT_DOUBLE);
					//object.Activate_Double(Moving_Object::BOTTOM_RIGHT_DOUBLE);
					break;
				case RIGHT_MARGIN: //pravy okraj
					object.Activate_Double(Moving_Object::LEFT_DOUBLE);
					break;
				case BOTTOM_RIGHT_RIGHT_MARGIN:  //pokracovani spodniho okraje za pravym okrajem
				case BOTTOM_RIGHT_BOTTOM_MARGIN: //pokracovani praveho okraje za spodnim okrajem
					object.Activate_Double(Moving_Object::TOP_LEFT_DOUBLE);
					break;
				case BOTTOM_MARGIN: //spodni okraj
					object.Activate_Double(Moving_Object::TOP_DOUBLE);
					break;
				case BOTTOM_LEFT_BOTTOM_MARGIN: //pokracovani leveho okraje za spodnim okrajem
				case BOTTOM_LEFT_LEFT_MARGIN:   //pokracovani dolniho okraje za levym okrajem
					object.Activate_Double(Moving_Object::TOP_RIGHT_DOUBLE);
					break;
				case LEFT_MARGIN: //levy okraj
					object.Activate_Double(Moving_Object::RIGHT_DOUBLE);
					break;
				default:
					break;
            }
		}
	}

	return doubles;
}

void Collision_System::Set_New_Powerup_Position(Powerup_System::Powerup & new_powerup, const SDL_Rect & area) const
{
	bool collides = false;

	do
	{
		collides = false;

		//Nastaveni nahodne pozice z dane oblasti.
		new_powerup.Set_Center_X(GLfloat((rand() % (area.w+1)) + area.x));
		new_powerup.Set_Center_Y(GLfloat((rand() % (area.h+1)) + area.y));

		//Kontrola na kolize s hraci.
	    for (int i = 0; i < Settings::NUM_PLAYERS; i++)
	    {
			if (players[i] == NULL) continue;

			if (Collide(*players[i], new_powerup)) 
	        {
			    collides = true;
				break;
			}
	    }

		if (collides) continue;

		//Kontrola na kolize s powerupy.
	    for (vector<Powerup_System::Powerup*>::iterator it = powerups->begin(); it < powerups->end(); it++)
	    {
			if (*it == &new_powerup) continue; //Nemuze kolidovat sam se sebou.
		    if (Collide(**it, new_powerup))
			{
				collides = true;
				break;
			}
		}

		if (collides) continue;

		//Kontrola na kolizi s hvezdou.
		if (star != NULL && Collide(new_powerup, *star))
	    {
			collides = true;
		}

	} while (collides);
}

void Collision_System::Manage_Collisions() const
{
	if (players[Settings::FIRST] != NULL && players[Settings::SECOND] != NULL)
	{
	    //Kontrola kolize mezi hraci.
		if (Collide(*players[Settings::FIRST], *players[Settings::SECOND]))
		{
			players[Settings::FIRST]->Destroy();
			players[Settings::SECOND]->Destroy();
		}
	
	    for (int i = 0; i < Settings::NUM_PLAYERS; i++) 
	    {
			//Kontrola kolize mezi hracem a hvezdou.
		    if (star != NULL && Collide(*players[i], *star))
		    {
			    players[i]->Destroy();
			    players[i]->Change_Score(-1);
		    }
 
			//Kontrola kolize mezi hracem a projektily.
		    for (vector<Projectile*>::iterator it = projectiles->begin(); it < projectiles->end(); it++)
		    {
			    if (Collide(*players[i], **it))
			    {
				    players[i]->Damage((*it)->Get_Damage()); 
				    if (players[i]->Get_Health() == 0) //hrac byl znicen
				    {
					    Settings::players_t other; //druhy hrac
					    if (i == Settings::FIRST)
						    other = Settings::SECOND;
					    else
						    other = Settings::FIRST;

				        if ((*it)->Get_Player() == other) //projektil patril druhemu hraci, pricte se mu skore
					    { 
					        players[other]->Change_Score(1);
					    }
					    else //projektil patril stejnemu hraci, odecte se mu skore, trefil sam sebe
					    {
						    players[i]->Change_Score(-1);
					    }
				    }
				    (*it)->Destroy(); 
			    }
		    }

			//kontrola kolize mezi hracem a powerupy
			vector<Powerup_System::Powerup*>::const_iterator it = powerups->begin();
	        while (it < powerups->end())
	        {
				if (Powerup_System::Is_Useful((*it)->Get_Type(), *players[i]) && Collide(*players[i], **it))
		        {
                    (*it)->Apply(*players[i]);

					//vymazani powerupu z vektoru powerupu
			        vector<Powerup_System::Powerup*>::const_iterator new_it; //nova pozice iteratoru
			        bool first = false; 
			        if (it == powerups->begin())
			        {
				        first = true;
			        }
			        else
			        {
			            new_it = it-1;
			        }
			        delete *it;
			        powerups->erase(it);
			        if (powerups->empty())
			        {
				        break;
			        }
			        if (first)
				        it = powerups->begin();
			        else
			            it = new_it;
		        }
		        it++;
	        }
	    }
	}

	for (vector<Projectile*>::iterator it = projectiles->begin(); it < projectiles->end(); it++)
	{
		//Kontrola kolize mezi projektilem a hvezdou.
		if (Collide(**it, *star))
		{
			(*it)->Destroy();
		}

		//Kontrola kolize mezi projektilem a ostatnimi projektily.
		for (vector<Projectile*>::iterator it2 = it+1; it2 < projectiles->end(); it2++)
	    {
		    if (Collide(**it, **it2))
		    {
			    (*it)->Destroy();
				(*it2)->Destroy();
		    }
		}
	}

	Manage_Beam_Collisions();
}

void Collision_System::Manage_Beam_Collisions() const
{
	static Uint32 last_time = SDL_GetTicks();
    Uint32 current_time = SDL_GetTicks();

	//kolize mezi paprsky a jinymi hernimi objekty
	for (int player = Settings::FIRST; player <= Settings::SECOND; player++)
	{
		if (players[player] == NULL)
		{
			continue;
		}

		if (!players[player]->Beam_Active())
		{
			players[player]->Set_Beam_Length(Player::beams_t(Player::LEFT_BEAM), 0);
			players[player]->Set_Beam_Length(Player::beams_t(Player::CENTER_BEAM), 0);
			players[player]->Set_Beam_Length(Player::beams_t(Player::RIGHT_BEAM), 0);
			continue;
		}

		for (int beam = Player::LEFT_BEAM; beam <= Player::RIGHT_BEAM; beam++)
	    {
			if (!players[player]->Triple() && beam != Player::CENTER_BEAM)
			{
				players[player]->Set_Beam_Length(Player::beams_t(beam), 0);
				continue;
			}

	        players[player]->Set_Max_Beam_Length(Player::beams_t(beam)); //nastaveni paprsku na maximaln delku, pak se zkracuje na zaklade zjistenych kolizi

			line_t beam_line = players[player]->Get_Beam_Line(Player::beams_t(beam));
		    vector<Point> intersections;

			//kolize paprsku s hvezdou
		    if (star->Is_Active())
		    {
		        Math::Line_Circle_Intersections(beam_line, Circle(star->Get_Radius(), star->Get_Center()), true, &intersections);

		        if (intersections.size() == 2) //dva pruseciky, vybere se ten, ktery je bliz k pocatku paprsku
	            {
			        GLfloat length1 = Math::Points_Distance(intersections[0], beam_line.first);
			        GLfloat length2 = Math::Points_Distance(intersections[1], beam_line.first);

			        if (length1 < length2)
			        {
					    players[player]->Set_Beam_Length(Player::beams_t(beam), length1);
			        }
			        else
			        {
					    players[player]->Set_Beam_Length(Player::beams_t(beam), length2);  
			        }

				    beam_line = players[player]->Get_Beam_Line(Player::beams_t(beam));
		        }
		        else if (!intersections.empty()) //jeden prusecik
		        {
			        GLfloat length = Math::Points_Distance(intersections[0], beam_line.first);
				    players[player]->Set_Beam_Length(Player::beams_t(beam), length);
				    beam_line = players[player]->Get_Beam_Line(Player::beams_t(beam));
		        }
		    }

			//kolize paprsku s hraci, vybere se prusecik, ktery je nejbliz pocatku paprsku
		    Settings::players_t other_player;
		    if (player == Settings::FIRST)
		    {
			    other_player = Settings::SECOND;
		    }
		    else
		    {
			    other_player = Settings::FIRST;
		    }
		
		    vector<polygon_t> borders = players[other_player]->Get_Borders();
		    bool found = false;
		    GLfloat shortest_length = 0;
		    for (vector<polygon_t>::iterator it = borders.begin(); it < borders.end(); it++)
		    {
			    Math::Line_Polygon_Intersections(beam_line, *it, true, &intersections);

			    for (vector<Point>::iterator it2 = intersections.begin(); it2 < intersections.end(); it2++)
			    {
				    GLfloat length = Math::Points_Distance(*it2, beam_line.first);
				    if (length < shortest_length || found == false)
				    {
					    shortest_length = length;
					    found = true;
				    }
			    }
		    }

		    if (found == true)
		    {
			    players[player]->Set_Beam_Length(Player::beams_t(beam), shortest_length);
			    beam_line = players[player]->Get_Beam_Line(Player::beams_t(beam));
		    }

			//kolize paprsku s projektily
			bool found2 = false;
		    for (vector<Projectile*>::iterator it = projectiles->begin(); it < projectiles->end(); it++)
		    {
			    if ((*it)->Get_Mode() == Projectile::NORMAL && Math::Line_Polygon_Intersections(beam_line, (*it)->Get_Border(), true, NULL))
			    {
				    (*it)->Destroy();
					found2 = true;
			    }
		    }

			if (found && !found2) //byl zasazen druhy hrac
			{
			    static GLfloat damage = 0;
			    damage += ((current_time - last_time) / 1000.0f) * Player::Get_Beam_Damage();

			    if (damage >= 1.0f)
			    {
				    players[other_player]->Damage(1);
				    damage -= 1.0f;
			    }
			}
	    }
	}

	last_time = current_time;
}