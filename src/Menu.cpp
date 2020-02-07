
/** \file Menu.cpp
*   \brief Obsahuje definice metod tridy Menu.
*/

#include "Math.h"
#include "Menu.h"
#include "Powerup_System.h"

//////////////////////////////////ITEMS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Font Menu::Screen::Head_Item::texts[NUM_LEVELS];

bool Menu::Screen::Interactive_Item::edited = false;
Menu::Screen::Interactive_Item* Menu::Screen::Interactive_Item::selected_item = NULL; 

Font Menu::Screen::Link_Item::text;
Color Menu::Screen::Link_Item::colors[NUM_STATES]; 

Font Menu::Screen::Setting_Item::text; 
Color Menu::Screen::Setting_Item::colors[NUM_STATES]; 
Settings* Menu::Screen::Setting_Item::settings = NULL; 

GLfloat Menu::Screen::Head_Item::Get_Level_Height(levels level) 
{ 
	return texts[level].Get_Font_Height(); 
}

GLfloat Menu::Screen::Head_Item::Get_Width() const 
{ 
	return texts[level].Get_Text_Width(Get_Name()); 
}

GLfloat Menu::Screen::Head_Item::Get_Height() const 
{ 
	return texts[level].Get_Font_Height(); 
}

void Menu::Screen::Head_Item::Set_Level_Height(int height, levels level) 
{ 
	texts[level].Change_Height(height); 
}

void Menu::Screen::Head_Item::Set_Level_Font(const string & path, int height, levels level) 
{ 
	texts[level].Load_Font(path, height); 
}

void Menu::Screen::Head_Item::Draw() const
{
	if (diff_color)
	{
		Color original_color = texts[level].Get_Color();
	    texts[level].Draw_Text(Get_Name(), Get_Position(), *diff_color);
		texts[level].Set_Color(original_color);
	}
	else
	{
		texts[level].Draw_Text(Get_Name(), Get_Position());
	}
}

GLfloat Menu::Screen::Link_Item::Get_Height_Static() 
{ 
	return text.Get_Font_Height(); 
}

GLfloat Menu::Screen::Link_Item::Get_Width() const 
{ 
	return text.Get_Text_Width(Get_Name()); 
}

GLfloat Menu::Screen::Link_Item::Get_Height() const 
{ 
	return text.Get_Font_Height(); 
}

void Menu::Screen::Link_Item::Set_Height(int height) 
{ 
	text.Change_Height(height); 
}

void Menu::Screen::Link_Item::Set_Font(const string & path, int height) 
{ 
	text.Load_Font(path, height); 
}

void Menu::Screen::Link_Item::Draw() const
{
	if (Is_Selected())
	{
	    text.Draw_Text(Get_Name(), Get_Position(), colors[SELECTED]);
	}
	else
	{
		text.Draw_Text(Get_Name(), Get_Position(), colors[NORMAL]);
	}
}

GLfloat Menu::Screen::Setting_Item::Get_Height_Static() 
{ 
	return text.Get_Font_Height(); 
}

GLfloat Menu::Screen::Setting_Item::Get_Width() const 
{ 
	return text.Get_Text_Width(Get_Name()); 
}

GLfloat Menu::Screen::Setting_Item::Get_Height() const 
{ 
	return text.Get_Font_Height(); 
}

void Menu::Screen::Setting_Item::Set_Height(int height) 
{ 
	text.Change_Height(height); 
}

void Menu::Screen::Setting_Item::Set_Font(const string & path, int height) 
{ 
	text.Load_Font(path, height); 
}

const Color & Menu::Screen::Setting_Item::Get_Color() const
{
	if (Is_Edited())
	{
		return colors[EDITING];
	}
	else if (Is_Selected())
	{
		return colors[SELECTED];
	}
	else
	{
		return colors[NORMAL];
	}
}

string Menu::Screen::Bool_Setting_Item::Get_Value_Text() const
{
	const Settings* settings = Get_Settings();
	return settings->Get_Setting_Value_Text(setting); //do menu
}

void Menu::Screen::Bool_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) return;
	if (key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_LEFT || key == SDLK_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Set_Setting(setting, !settings->Get_Setting_Value(setting));
	}
}

void Menu::Screen::Bool_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) 
		return;

	if (mouse_button == SDL_BUTTON_LEFT || mouse_button == SDL_BUTTON_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Set_Setting(setting, !settings->Get_Setting_Value(setting));
	}
}

void Menu::Screen::Bool_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Font font = Get_Text();
	string value = Get_Value_Text();
	GLfloat value_width = font.Get_Text_Width(value);
	GLfloat value_height = font.Get_Text_Height(value);
	const Color color = Get_Color();

	font.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	font.Draw_Text("< ", position, color);
	position.x += font.Get_Text_Width("< ");
	font.Draw_Text(Get_Value_Text(), position, color);
	position.x += value_width;
	font.Draw_Text(" >", position, color);
}

string Menu::Screen::Player_Setting_Item::Get_Value_Text() const
{
	const Settings* settings = Get_Settings();
	return settings->Get_Setting_Value_Text(setting); //do menu
}

void Menu::Screen::Player_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) 
		return;

	if (key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Player_Setting_Up(setting);
	}
	else if (key == SDLK_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Player_Setting_Down(setting);
	}
}

void Menu::Screen::Player_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) 
		return;

	if (mouse_button == SDL_BUTTON_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Player_Setting_Up(setting);
	}
	else if (mouse_button == SDL_BUTTON_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Player_Setting_Down(setting);
	}
}

void Menu::Screen::Player_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Font font = Get_Text();
	string value = Get_Value_Text();
	GLfloat value_width = font.Get_Text_Width(value);
	GLfloat value_height = font.Get_Text_Height(value);
	const Color color = Get_Color();

	font.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	font.Draw_Text("< ", position, color);
	position.x += font.Get_Text_Width("< ");
	Settings* settings = Setting_Item::Get_Settings();
	if (settings->Get_Setting_Value(setting) == Settings::FIRST)
	{
	    const Color color2 = settings->Get_Setting_Value(Settings::PLAYER1_COLOR);
		font.Draw_Text(Get_Value_Text(), position, color2);
	}
	else if (settings->Get_Setting_Value(setting) == Settings::SECOND)
	{
		const Color color2 = settings->Get_Setting_Value(Settings::PLAYER2_COLOR);
		font.Draw_Text(Get_Value_Text(), position, color2);
	}
	else
	{
		font.Draw_Text(Get_Value_Text(), position, color);
	}
	position.x += value_width;
	font.Draw_Text(" >", position, color);	
}

string Menu::Screen::Weapon_Setting_Item::Get_Value_Text() const
{
	const Settings* settings = Get_Settings();
	return settings->Get_Setting_Value_Text(setting); //do menu
}

void Menu::Screen::Weapon_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) 
		return;

	if (key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Weapon_Setting_Up(setting);
	}
	else if (key == SDLK_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Weapon_Setting_Down(setting);
	}
}

void Menu::Screen::Weapon_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) 
		return;

	if (mouse_button == SDL_BUTTON_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Weapon_Setting_Up(setting);
	}
	else if (mouse_button == SDL_BUTTON_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Weapon_Setting_Down(setting);
	}
}

void Menu::Screen::Weapon_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Font font = Get_Text();
	string value = Get_Value_Text();
	GLfloat value_width = font.Get_Text_Width(value);
	GLfloat value_height = font.Get_Text_Height(value);
	const Color color = Get_Color();

	font.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	font.Draw_Text("< ", position, color);
	position.x += font.Get_Text_Width("< ");
	font.Draw_Text(Get_Value_Text(), position, color);
	position.x += value_width;
	font.Draw_Text(" >", position, color);	
}

string Menu::Screen::Color_Setting_Item::Get_Value_Text() const
{
	const Settings* settings = Get_Settings();
	return settings->Get_Setting_Value_Text(setting); //do menu
}

void Menu::Screen::Color_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) return;
	if (key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Color_Up(setting);
	}
	else if (key == SDLK_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Color_Down(setting);
	}
}

void Menu::Screen::Color_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) return;
	if (mouse_button == SDL_BUTTON_LEFT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Color_Up(setting);
	}
	else if (mouse_button == SDL_BUTTON_RIGHT)
	{
		Settings* settings = Setting_Item::Get_Settings();
        settings->Color_Down(setting);
	}
}

void Menu::Screen::Color_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Font font = Get_Text();
	string value = Get_Value_Text();
	GLfloat value_width = font.Get_Text_Width(value);
	GLfloat value_height = font.Get_Text_Height(value);
	const Color color = Get_Color();
	Settings* settings = Setting_Item::Get_Settings();
	const Color color2 = settings->Get_Setting_Value(setting);

	font.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	font.Draw_Text("< ", position, color);
	position.x += font.Get_Text_Width("< ");
	font.Draw_Text(Get_Value_Text(), position, color2);
	position.x += value_width;
	font.Draw_Text(" >", position, color);
}

string Menu::Screen::Key_Setting_Item::Get_Value_Text() const
{
	if (this->Is_Edited()) return "_";
	const Settings* settings = Get_Settings();
	return settings->Get_Setting_Value_Text(setting, player); //do menu
}

void Menu::Screen::Key_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) return;
	if (Is_Edited())
	{
		if (key != SDLK_ESCAPE)
		{
			Settings* settings = Get_Settings();
			settings->Set_Setting(setting, player, key);
		}
		Stop_Editing_H(); 
	}
	else if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
    {
		Edit();
	}
}

void Menu::Screen::Key_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) return;
	if (Is_Edited())
	{
		Settings* settings = Get_Settings();
		settings->Set_Setting(setting, player, mouse_button);

		Stop_Editing_H(); 
	}
	else if (mouse_button == SDL_BUTTON_LEFT)
    {
		Edit();
	}
}

void Menu::Screen::Key_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Font text = Get_Text();
	const Color color = Get_Color();
	text.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	text.Draw_Text(Get_Value_Text(), position, color);
}

string Menu::Screen::Int_Setting_Item::Get_Value_Text() const
{
	if (value_empty) return "_";
	if (Is_Edited())
	{
		return Math::Int_To_String(current_int_value) + "_";
	}
	const Settings* settings = Get_Settings();
	return Math::Int_To_String(settings->Get_Setting_Value(setting)); //do menu
}

void Menu::Screen::Int_Setting_Item::Update(SDLKey key)
{
	if (!Is_Selected()) return;

	if (Is_Edited())
	{
		if (key == SDLK_ESCAPE)
		{
			Stop_Editing_H(); 
			value_empty = false;
		}
		else if (key >= SDLK_0 && key <= SDLK_9)
		{
			current_int_value *= 10;
			current_int_value += key - SDLK_0;
			value_empty = false;
		}
		else if (key >= SDLK_KP0 && key <= SDLK_KP9)
		{
			current_int_value *= 10;
			current_int_value += key - SDLK_KP0;
			value_empty = false;
		}
		else if (key == SDLK_PLUS || key == SDLK_MINUS || key == SDLK_KP_PLUS || key == SDLK_KP_MINUS)
		{
			Settings* settings = Get_Settings();
			if (settings->Get_Min(setting) < 0)
			    current_int_value *= -1;
		}
		else if (key == SDLK_BACKSPACE)
		{
			current_int_value = Math::Backspace_Number(current_int_value);
			if (!current_int_value) value_empty = true;
		}
		else if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
		{
			Stop_Editing_H(); 
			value_empty = false;
			Settings* settings = Get_Settings();
			settings->Set_Setting(setting, current_int_value);
		}
	}
	else if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
    {
		Edit();
	}
}

 void Menu::Screen::Int_Setting_Item::Update(int mouse_button)
{
	if (!Is_Selected()) return;

	if (Is_Edited())
	{
		if (mouse_button == SDL_BUTTON_LEFT)
		{
			Stop_Editing_H(); 
			value_empty = false;
			Settings* settings = Get_Settings();
			settings->Set_Setting(setting, current_int_value);
		}
	}
	else if (mouse_button == SDL_BUTTON_LEFT)
    {
		Edit();
	}
}

void Menu::Screen::Int_Setting_Item::Draw() const
{
	Point position = Get_Position();
	Settings* settings = Get_Settings();
	string units = settings->Get_Setting_Units(setting);
	
	Font text = Get_Text();
	const Color color = Get_Color();

	
	text.Draw_Text(Get_Name(), position, color);
    position.x += Get_Width() + 30.0f;
	text.Draw_Text(Get_Value_Text(), position, color); 
	position.x += text.Get_Text_Width(Get_Value_Text());
	if (show_max)
	{
        text.Draw_Text(" / " + Math::Int_To_String(settings->Get_Max(setting)), position, color);
	    position.x += text.Get_Text_Width(" / " + Math::Int_To_String(settings->Get_Max(setting)));
	}
	
	text.Draw_Text(units, position, color);
	
}

void Menu::Screen::Int_Setting_Item::Edit()
{
	Edit_H(); 
	value_empty = true;
	current_int_value = 0;
}

void Menu::Screen::Int_Setting_Item::Stop_Editing()
{
	Stop_Editing_H(); 
	value_empty = false;
	Settings* settings = Get_Settings();
	settings->Set_Setting(setting, current_int_value);
}

string Menu::Screen::Resolution_Item::Get_Value_Text() const
{
	const Settings* settings = Get_Settings();
	return settings->Get_Resolution_Text(); //do menu
}

void Menu::Screen::Resolution_Item::Update(SDLKey key)
{
	if (!Is_Selected()) return;

	Settings* settings = Get_Settings();

	
	if (key == SDLK_RIGHT || key == SDLK_RETURN || key == SDLK_KP_ENTER)
	{
		settings->Resolution_Up();
	}
	else if (key == SDLK_LEFT)
	{
		settings->Resolution_Down();
	}
}

void Menu::Screen::Resolution_Item::Update(int mouse_button)
{
	if (!Is_Selected()) return;

	Settings* settings = Get_Settings();

	
	if (mouse_button == SDL_BUTTON_LEFT)
	{
		settings->Resolution_Up();
	}
	else if (mouse_button == SDL_BUTTON_RIGHT)
	{
		settings->Resolution_Down();
	}
}

void Menu::Screen::Resolution_Item::Draw() const
{
	Point position = Get_Position();
	Font font = Get_Text();
	string value = Get_Value_Text();
	GLfloat value_width = font.Get_Text_Width(value);
	GLfloat value_height = font.Get_Text_Height(value);
	const Color color = Get_Color();

	font.Draw_Text(Get_Name(), position, color);
	position.x += Get_Width() + 30.0f;
	font.Draw_Text("< ", position, color);
	position.x += font.Get_Text_Width("< ");
	font.Draw_Text(Get_Value_Text(), position, color);
	position.x += value_width;
	font.Draw_Text(" >", position, color);
}

//////////////////////////////////SCREEN//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Rect Menu::Screen::resolution;                              //!<x-ovy rozmer menu.
GLfloat Menu::Screen::space_ratio = 0.2f;                   //!<Velikost mezer mezi radky.

Menu::Screen::Item* Menu::Screen::operator[](unsigned int i)
{
	if (i < items.size())
	{
		return items[i];
	}
	else
		return NULL;
}

Menu::Screen::Screen(const Screen & screen) : escape_screen(screen.escape_screen), escape_evt(screen.escape_evt)
{
	for (vector<Item*>::const_iterator it = screen.items.begin(); it < screen.items.end(); it++)
	{
		if (Head_Item* hi = dynamic_cast<Head_Item*>(*it))
		{
			Head_Item* hi2 = new Head_Item(*hi);
			items.push_back(hi2);
		}
		else if (Link_Item* li = dynamic_cast<Link_Item*>(*it))
		{
			
			Link_Item* li2 = new Link_Item(*li);
			items.push_back(li2);
		}
		else if (Bool_Setting_Item* bi = dynamic_cast<Bool_Setting_Item*>(*it))
		{
			Bool_Setting_Item* bi2 = new Bool_Setting_Item(*bi);
			items.push_back(bi2);
		}
		else if (Key_Setting_Item* ki = dynamic_cast<Key_Setting_Item*>(*it))
		{
			Key_Setting_Item* ki2 = new Key_Setting_Item(*ki);
			items.push_back(ki2);
		}
		else if (Player_Setting_Item* pi = dynamic_cast<Player_Setting_Item*>(*it))
		{
			Player_Setting_Item* pi2 = new Player_Setting_Item(*pi);
			items.push_back(pi2);
		}
		else if (Weapon_Setting_Item* wi = dynamic_cast<Weapon_Setting_Item*>(*it))
		{
			Weapon_Setting_Item* wi2 = new Weapon_Setting_Item(*wi);
			items.push_back(wi2);
		}
		else if (Int_Setting_Item* ii = dynamic_cast<Int_Setting_Item*>(*it))
		{
			Int_Setting_Item* ii2 = new Int_Setting_Item(*ii);
			items.push_back(ii2);
		}
		else if (Resolution_Item* ri = dynamic_cast<Resolution_Item*>(*it))
		{
			Resolution_Item* ri2 = new Resolution_Item(*ri);
			items.push_back(ri2);
		}
		else if (Color_Setting_Item* ci = dynamic_cast<Color_Setting_Item*>(*it))
		{
			
			Color_Setting_Item* ci2 = new Color_Setting_Item(*ci);
			items.push_back(ci2);
		}
	}
}

Menu::Screen & Menu::Screen::operator=(const Screen & screen)
{
	if (this == &screen)
		return *this;

	for (vector<Item*>::iterator it = items.begin(); it < items.end(); it++)
	{
		delete *it;
	}

	for (vector<Item*>::const_iterator it = screen.items.begin(); it < screen.items.end(); it++)
	{
		if (Head_Item* hi = dynamic_cast<Head_Item*>(*it))
		{
			Head_Item* hi2 = new Head_Item(*hi);
			items.push_back(hi2);
		}
		else if (Link_Item* li = dynamic_cast<Link_Item*>(*it))
		{
			Link_Item* li2 = new Link_Item(*li);
			items.push_back(li2);
		}
		else if (Bool_Setting_Item* bi = dynamic_cast<Bool_Setting_Item*>(*it))
		{
			Bool_Setting_Item* bi2 = new Bool_Setting_Item(*bi);
			items.push_back(bi2);
		}
		else if (Key_Setting_Item* ki = dynamic_cast<Key_Setting_Item*>(*it))
		{
			Key_Setting_Item* ki2 = new Key_Setting_Item(*ki);
			items.push_back(ki2);
		}
		else if (Player_Setting_Item* pi = dynamic_cast<Player_Setting_Item*>(*it))
		{
			Player_Setting_Item* pi2 = new Player_Setting_Item(*pi);
			items.push_back(pi2);
		}
		else if (Weapon_Setting_Item* wi = dynamic_cast<Weapon_Setting_Item*>(*it))
		{
			Weapon_Setting_Item* wi2 = new Weapon_Setting_Item(*wi);
			items.push_back(wi2);
		}
		else if (Int_Setting_Item* ii = dynamic_cast<Int_Setting_Item*>(*it))
		{
			Int_Setting_Item* ii2 = new Int_Setting_Item(*ii);
			items.push_back(ii2);
		}
		else if (Resolution_Item* ri = dynamic_cast<Resolution_Item*>(*it))
		{
			Resolution_Item* ri2 = new Resolution_Item(*ri);
			items.push_back(ri2);
		}
		else if (Color_Setting_Item* ci = dynamic_cast<Color_Setting_Item*>(*it))
		{
			Color_Setting_Item* ci2 = new Color_Setting_Item(*ci);
			items.push_back(ci2);
		}
	}

	escape_screen = screen.escape_screen;
	escape_evt = screen.escape_evt;

	return *this;
}

void Menu::Screen::Update(GLfloat mouse_y)
{
	if (Interactive_Item::Get_Edited_Item()) return;

	Interactive_Item* last_ii = NULL;

	for (vector<Item*>::iterator it = items.begin(); it < items.end(); it++)
	{
		if (Interactive_Item* ii = dynamic_cast<Interactive_Item*>(*it))
		{
			last_ii = ii;
			if (mouse_y < (*it)->Get_Position().y + ((*it)->Get_Height() * (1 + space_ratio)))
		    {
			    ii->Select();
			    return;
		    }
		}
		if (it == items.end()-1 && last_ii != NULL) last_ii->Select();
	}	
}

Menu::sections Menu::Screen::Update(SDLKey key)
{
	if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Edited_Item())) //editing
	{
		si->Update(key);
	}
	else //not editing
	{
		switch (key)
	    {
	    case SDLK_UP:
		    Selected_Item_Up();
			break;
		case SDLK_DOWN:
			Selected_Item_Down();
			break;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			if (Link_Item* li = dynamic_cast<Link_Item*>(Interactive_Item::Get_Selected_Item())) //link item selected
			{
				switch (li->Get_Event())
				{
				case USR_EVT_START_GAME:
				case USR_EVT_RETURN_TO_GAME:
				case USR_EVT_QUIT_GAME:
				case USR_EVT_VIDEO_SETTINGS_CHANGED:
					Send_Event(li->Get_Event());
					break;
				case USR_EVT_QUIT_PROGRAM:
					SDL_Event event;
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
					break;
				default:
					break;
				}
				return li->Get_Linked_Section();
			}
			else if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Selected_Item())) //setting item selected
			{
				si->Update(key);
			}
			break;
		case SDLK_ESCAPE:
			Send_Event(escape_evt);
			return escape_screen;
			break;
		default:
			if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Selected_Item()))
			{
				si->Update(key);
			}
			break;
		}
	}

	return NO_SECTION;
}

Menu::sections Menu::Screen::Update(int mouse_button)
{
	if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Edited_Item())) //editing
	{
		si->Update(mouse_button);
	}
	else //not editing
	{
		switch (mouse_button)
	    {
		case SDL_BUTTON_LEFT:
			if (Link_Item* li = dynamic_cast<Link_Item*>(Interactive_Item::Get_Selected_Item())) //link item selected
			{
				switch (li->Get_Event())
				{
				case USR_EVT_START_GAME:
				case USR_EVT_RETURN_TO_GAME:
				case USR_EVT_QUIT_GAME:
				case USR_EVT_VIDEO_SETTINGS_CHANGED:
					Send_Event(li->Get_Event());
					break;
				case USR_EVT_QUIT_PROGRAM:
					SDL_Event event;
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
					break;
				default:
					break;
				}
				return li->Get_Linked_Section();
			}
			else if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Selected_Item())) //setting item selected
			{
				si->Update(mouse_button);
			}
			break;
		default:
			if (Setting_Item* si = dynamic_cast<Setting_Item*>(Interactive_Item::Get_Selected_Item()))
			{
				si->Update(mouse_button);
			}
			break;
		}
	}

	return NO_SECTION;
}


void Menu::Screen::Send_Event(int code) const
{
	SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = NULL; // Bez parametrů
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
}

GLfloat Menu::Screen::Get_Items_Height() const
{
	GLfloat result = 0;

	for (vector<Item*>::const_iterator it = items.begin(); it < items.end(); it++)
	{
		GLfloat item_height = (*it)->Get_Height(); 
		if (it == items.end() - 1) 
		{
			result += item_height;
		}
		else
		{
			result += item_height * (1 + space_ratio);
		}
	}
	
	return result;
}

void Menu::Screen::Draw() const
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		items[i]->Draw();
	}
}

void Menu::Screen::Selected_Item_Up()
{
	for (vector<Item*>::iterator it = items.begin(); it < items.end(); it++)
	{
		Interactive_Item* ii = dynamic_cast<Interactive_Item*>(*it);
		if (ii && ii->Is_Selected() && !ii->Is_Edited())
		{
			vector<Item*>::iterator it2 = it;

			while (it2 > items.begin())
			{
				it2--;
				if (Interactive_Item* ii2 = dynamic_cast<Interactive_Item*>(*it2)) 
				{
					ii2->Select();
					return;
				}
			} 
			
			for (it2 = items.end()-1; it2 > it; it2--)
			{
				if (Interactive_Item* ii2 = dynamic_cast<Interactive_Item*>(*it2)) 
				{
					ii2->Select();
					return;
				}
			}
		}
	}
}

void Menu::Screen::Selected_Item_Down()
{
	for (vector<Item*>::iterator it = items.begin(); it < items.end(); it++)
	{
		Interactive_Item* ii = dynamic_cast<Interactive_Item*>(*it);
	
		if (ii && ii->Is_Selected())
		{
	        for (vector<Item*>::iterator it2 = it + 1; it2 < items.end(); it2++)
			{
				if (Interactive_Item* ii2 = dynamic_cast<Interactive_Item*>(*it2)) 
				{
					ii2->Select();
					return;
				}
			}
			for (vector<Item*>::iterator it2 = items.begin(); it2 < it; it2++)
			{
				if (Interactive_Item* ii2 = dynamic_cast<Interactive_Item*>(*it2)) 
				{
					ii2->Select();
					return;
				}
			}
		}
	}
}

void Menu::Screen::Set_Items_Positions()
{
	GLfloat section_height = Get_Items_Height();
    GLfloat y_pos = (resolution.h - section_height) / 2;
	
    for (unsigned int i = 0; i < items.size(); i++)
    {    
	    GLfloat x_pos;
	 
	    if (dynamic_cast<Setting_Item*>(items[i]))
	    {
		    x_pos = (resolution.w / 2) - items[i]->Get_Width();
		}
		else
		{
			x_pos = (resolution.w / 2) - (items[i]->Get_Width() / 2);
		}
	
		items[i]->Set_Position(Point(x_pos, y_pos));
		
	    y_pos += items[i]->Get_Height() * (1 + space_ratio);
		
	}
	
}

Menu::Screen::~Screen()
{
	while (!items.empty())
	{
		delete items[0];
		items.erase(items.begin());
	}
}


//////////////////////////////////MENU//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const GLfloat Menu::HORIZONTAL_MARGINS = 10.0f;
const GLfloat Menu::VERTICAL_MARGINS = 10.0f;

void Menu::Set_Items_Sizes()
{
	GLfloat max_height = Get_Max_Height();
	GLfloat scale1 = (Screen::Get_Resolution().h - (2*VERTICAL_MARGINS)) / max_height;

	GLfloat max_width = Get_Max_Left_Width();
	GLfloat scale2 = ((Screen::Get_Resolution().w - (2*HORIZONTAL_MARGINS)) / 2.0f) / max_width;

	GLfloat scale = Math::Smaller(scale1, scale2);
	
	for (int i = Screen::Head_Item::FIRST_LEVEL; i <= Screen::Head_Item::LAST_LEVEL; i++)
	{
	    Screen::Head_Item::Set_Level_Height(int(Screen::Head_Item::Get_Level_Height(Screen::Head_Item::levels(i)) * scale), Screen::Head_Item::levels(i));
	}
	
	Screen::Link_Item::Set_Height(int(Screen::Link_Item::Get_Height_Static() * scale));
	Screen::Setting_Item::Set_Height(int(Screen::Setting_Item::Get_Height_Static() * scale));
}

void Menu::Set_Section(sections section) 
{
	if ((this->section == MAIN && section == PAUSED) || (this->section == QUIT_GAME && section == MAIN))
	{
		paused = !paused;
	}

	this->section = section;

	//oznaceni polozky podle pozice mysi
	int x, y;
	SDL_GetMouseState(&x, &y);

	Update(float(y));
}

Menu::Screen & Menu::Get_Current_Screen()
{
	if (paused && screens_paused[section].Get_Items_Height())
	{
		return screens_paused[section];
	}
	else
	{
		return screens[section];
	}
}

GLfloat Menu::Get_Max_Height() const
{
	GLfloat max_height = 0.0f;
	for (int section = FIRST_SECTION; section < NUM_MENU_SECTIONS; section++)
	{
		GLfloat section_height = screens[section].Get_Items_Height();
		GLfloat paused_section_height = screens_paused[section].Get_Items_Height();
		GLfloat higher = Math::Greater(section_height, paused_section_height);

		if (higher > max_height)
		{
			max_height = higher;
		}
	}

	return max_height;
}

GLfloat Menu::Get_Max_Left_Width()
{
	GLfloat max_width = 0.0f;
	for (int section = FIRST_SECTION; section < NUM_MENU_SECTIONS; section++)
	{
		for (int i = 0; i < screens[section].Get_Items_Count(); i++)
		{
			GLfloat width = screens[section][i]->Get_Width();
			if (dynamic_cast<const Screen::Head_Item*>(screens[section][i])) //nadpis je uprostred, vlevo je polovina
			{
				width /= 2;
			}
			if (width > max_width)
			{
				max_width = width;
			}
		}

		for (int i = 0; i < screens_paused[section].Get_Items_Count(); i++)
		{
			GLfloat width = screens_paused[section][i]->Get_Width();
			if (dynamic_cast<const Screen::Head_Item*>(screens[section][i]))
			{
				width /= 2;
			}
			if (width > max_width)
			{
				max_width = width;
			}
		}
	}

	return max_width;
}

Menu::Menu(Settings & settings) : section(MAIN), paused(false)
{
	Screen::Setting_Item::Set_Settings(settings);
	vector<Screen::Item*> items;

	for (int i = 0; i < NUM_MENU_SECTIONS; i++)
	{
		items.clear();
		switch (i)
		{
		case MAIN:
			items.push_back(new Screen::Head_Item("Space War", Screen::Head_Item::MAIN_HEADING));
			items.push_back(new Screen::Link_Item("SPUSTIT HRU", PAUSED, USR_EVT_START_GAME));
			items.push_back(new Screen::Link_Item("NASTAVENÍ", SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("UKONČIT PROGRAM", QUIT_PROGRAM, NO_EVENT));
			screens[i] = Screen(items, QUIT_PROGRAM);
			break;
		case PAUSED:
			items.push_back(new Screen::Head_Item("HRA POZASTAVENA", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("ZPĚT DO HRY", PAUSED, USR_EVT_RETURN_TO_GAME));
		    items.push_back(new Screen::Link_Item("NASTAVENÍ", SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("UKONČIT HRU", QUIT_GAME, NO_EVENT));
			screens[i] = Screen(items, PAUSED, USR_EVT_RETURN_TO_GAME);
			break;
		case SETTINGS:
			items.push_back(new Screen::Head_Item("NASTAVENÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("ZOBRAZENÍ", VIDEO_SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("OVLÁDÁNÍ", CONTROLS, NO_EVENT));
		    items.push_back(new Screen::Link_Item("HERNÍ NASTAVENÍ", GAME_SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("ZPĚT", MAIN, NO_EVENT)); //!!!
			screens[i] = Screen(items, MAIN);
			items.clear();

			items.push_back(new Screen::Head_Item("NASTAVENÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("ZOBRAZENÍ", VIDEO_SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("OVLÁDÁNÍ", CONTROLS, NO_EVENT));
			items.push_back(new Screen::Link_Item("HERNÍ NASTAVENÍ", GAME_SETTINGS, NO_EVENT));
			items.push_back(new Screen::Link_Item("ZPĚT", PAUSED, NO_EVENT));
			screens_paused[i] = Screen(items, PAUSED);
			break;	
		case VIDEO_SETTINGS:
			items.push_back(new Screen::Head_Item("ZOBRAZENÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Resolution_Item());
			items.push_back(new Screen::Bool_Setting_Item(Settings::FULLSCREEN));
			items.push_back(new Screen::Int_Setting_Item(Settings::NUM_STARS, true));
			items.push_back(new Screen::Bool_Setting_Item(Settings::GRID));
			items.push_back(new Screen::Bool_Setting_Item(Settings::FPS));
			items.push_back(new Screen::Color_Setting_Item(Settings::PLAYER1_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::PLAYER2_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::STAR_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::GRID_COLOR));
			items.push_back(new Screen::Link_Item("ZPĚT", SETTINGS, USR_EVT_VIDEO_SETTINGS_CHANGED));
			screens[i] = Screen(items, SETTINGS, USR_EVT_VIDEO_SETTINGS_CHANGED);
			items.clear();

			items.push_back(new Screen::Head_Item("ZOBRAZENÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Bool_Setting_Item(Settings::FULLSCREEN));
			items.push_back(new Screen::Int_Setting_Item(Settings::NUM_STARS, true));
			items.push_back(new Screen::Bool_Setting_Item(Settings::GRID));
			items.push_back(new Screen::Bool_Setting_Item(Settings::FPS));
			items.push_back(new Screen::Color_Setting_Item(Settings::PLAYER1_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::PLAYER2_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::STAR_COLOR));
			items.push_back(new Screen::Color_Setting_Item(Settings::GRID_COLOR));
			items.push_back(new Screen::Link_Item("ZPĚT", SETTINGS, USR_EVT_VIDEO_SETTINGS_CHANGED));
			screens_paused[i] = Screen(items, SETTINGS, USR_EVT_VIDEO_SETTINGS_CHANGED);
			break;
		case CONTROLS:
			items.push_back(new Screen::Head_Item("OVLÁDÁNÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Player_Setting_Item(Settings::MOUSE_TURNING_PLAYER));
			items.push_back(new Screen::Head_Item("HRÁČ 1", Screen::Head_Item::SUBHEADING, settings.Get_Setting_Value(Settings::PLAYER1_COLOR)));
			items.push_back(new Screen::Key_Setting_Item(Settings::ACCELERATE, Settings::FIRST, "Dopředu:"));
		    items.push_back(new Screen::Key_Setting_Item(Settings::DECELERATE, Settings::FIRST, "Dozadu:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::TURN_LEFT, Settings::FIRST, "Doleva:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::TURN_RIGHT, Settings::FIRST, "Doprava:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::FIRE, Settings::FIRST, "Vystřelit:"));
			items.push_back(new Screen::Head_Item("HRÁČ 2", Screen::Head_Item::SUBHEADING, settings.Get_Setting_Value(Settings::PLAYER2_COLOR)));
			items.push_back(new Screen::Key_Setting_Item(Settings::ACCELERATE, Settings::SECOND, "Dopředu:"));
		    items.push_back(new Screen::Key_Setting_Item(Settings::DECELERATE, Settings::SECOND, "Dozadu:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::TURN_LEFT, Settings::SECOND, "Doleva:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::TURN_RIGHT, Settings::SECOND, "Doprava:"));
			items.push_back(new Screen::Key_Setting_Item(Settings::FIRE, Settings::SECOND, "Vystřelit:"));
			items.push_back(new Screen::Link_Item("ZPĚT", SETTINGS, NO_EVENT));
			screens[i] = Screen(items, SETTINGS);
			break;
		case GAME_SETTINGS:
			items.push_back(new Screen::Head_Item("HERNÍ NASTAVENÍ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("NASTAVENÍ BONUSŮ", POWERUPS_SETTINGS, NO_EVENT));
			items.push_back(new Screen::Bool_Setting_Item(Settings::STAR));
			items.push_back(new Screen::Int_Setting_Item(Settings::SHIP_GRAVITY));
			items.push_back(new Screen::Int_Setting_Item(Settings::PROJECTILE_GRAVITY));
			items.push_back(new Screen::Int_Setting_Item(Settings::ACCELERATION));
			items.push_back(new Screen::Int_Setting_Item(Settings::DECELERATION));
			items.push_back(new Screen::Int_Setting_Item(Settings::MAX_SPEED));
			items.push_back(new Screen::Int_Setting_Item(Settings::PLAYER_RESISTANCE));
			items.push_back(new Screen::Int_Setting_Item(Settings::PLAYER_ROTATION_SPEED));
			items.push_back(new Screen::Int_Setting_Item(Settings::MAX_HEALTH));
			items.push_back(new Screen::Int_Setting_Item(Settings::MAX_SHIELD));
			items.push_back(new Screen::Int_Setting_Item(Settings::SHIELD_REGENERATION_INTERVAL));
			items.push_back(new Screen::Int_Setting_Item(Settings::SHIELD_REGENERATION_AMOUNT, true));
			items.push_back(new Screen::Int_Setting_Item(Settings::MAX_ENERGY));
			items.push_back(new Screen::Int_Setting_Item(Settings::ENERGY_REGENERATION_INTERVAL));
			items.push_back(new Screen::Int_Setting_Item(Settings::ENERGY_REGENERATION_AMOUNT, true));
			items.push_back(new Screen::Weapon_Setting_Item(Settings::DEFAULT_WEAPON));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_SPEED));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_RESISTANCE));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_DAMAGE));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_RATE));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_ENERGY_USAGE, true));
			items.push_back(new Screen::Int_Setting_Item(Settings::ROCKET_SPEED));
			items.push_back(new Screen::Int_Setting_Item(Settings::ROCKET_RESISTANCE));
			items.push_back(new Screen::Int_Setting_Item(Settings::ROCKET_DAMAGE));
			items.push_back(new Screen::Int_Setting_Item(Settings::ROCKET_RATE));
			items.push_back(new Screen::Int_Setting_Item(Settings::ROCKET_ENERGY_USAGE, true));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_BEAM_DAMAGE));
			items.push_back(new Screen::Int_Setting_Item(Settings::LASER_BEAM_ENERGY_USAGE, true));
			items.push_back(new Screen::Link_Item("ZPĚT", SETTINGS, NO_EVENT));
			screens[i] = Screen(items, SETTINGS);
			break;
		case POWERUPS_SETTINGS:
			items.push_back(new Screen::Head_Item("NASTAVENÍ BONUSŮ", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Int_Setting_Item(Settings::POWERUP_DISPLAY_DURATION));
			items.push_back(new Screen::Int_Setting_Item(Settings::POWERUP_MIN_INTERVAL));
			items.push_back(new Screen::Int_Setting_Item(Settings::POWERUP_MAX_INTERVAL));
			items.push_back(new Screen::Head_Item("DOPLNĚNÍ ŽIVOTA", Screen::Head_Item::SUBHEADING, Powerup_System::Health_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::HEALTH_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Int_Setting_Item(Settings::HEALTH_POWERUP_AMOUNT, true));
			items.push_back(new Screen::Head_Item("DOPLNĚNÍ ŠTÍTU", Screen::Head_Item::SUBHEADING, Powerup_System::Shield_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::SHIELD_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Int_Setting_Item(Settings::SHIELD_POWERUP_AMOUNT, true));
			items.push_back(new Screen::Head_Item("DOPLNĚNÍ ENERGIE", Screen::Head_Item::SUBHEADING, Powerup_System::Energy_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::ENERGY_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Int_Setting_Item(Settings::ENERGY_POWERUP_AMOUNT, true));
			items.push_back(new Screen::Head_Item("TROJITÉ STŘELY", Screen::Head_Item::SUBHEADING, Powerup_System::Triple_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::TRIPLE_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Int_Setting_Item("Trvání účinku:", Settings::TRIPLE_POWERUP_DURATION));
			items.push_back(new Screen::Head_Item("NAVÁDĚNÉ STŘELY", Screen::Head_Item::SUBHEADING, Powerup_System::Guided_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::GUIDED_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Int_Setting_Item("Rychlost otáčení:", Settings::GUIDED_MISSILE_ROTATION_SPEED));
			items.push_back(new Screen::Int_Setting_Item("Trvání účinku:", Settings::GUIDED_POWERUP_DURATION));
			items.push_back(new Screen::Head_Item("LASEROVÉ STŘELY", Screen::Head_Item::SUBHEADING, Powerup_System::Laser_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::LASER_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Head_Item("RAKETY", Screen::Head_Item::SUBHEADING, Powerup_System::Rocket_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::ROCKET_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Head_Item("LASEROVÝ PAPRSEK", Screen::Head_Item::SUBHEADING, Powerup_System::Laser_Beam_Powerup::COLOR));
			items.push_back(new Screen::Bool_Setting_Item(Settings::LASER_BEAM_POWERUP_ALLOWED, "Povolen:"));
			items.push_back(new Screen::Link_Item("ZPĚT", GAME_SETTINGS, NO_EVENT));
			screens[i] = Screen(items, GAME_SETTINGS);
			break;
		case QUIT_PROGRAM:
			items.push_back(new Screen::Head_Item("OPRAVDU UKONČIT PROGRAM?", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("ANO", QUIT_PROGRAM, USR_EVT_QUIT_PROGRAM));
			items.push_back(new Screen::Link_Item("NE", MAIN, NO_EVENT));
			screens[i] = Screen(items, MAIN);
			break;
		case QUIT_GAME:
			items.push_back(new Screen::Head_Item("OPRAVDU UKONČIT HRU?", Screen::Head_Item::HEADING));
			items.push_back(new Screen::Link_Item("ANO", MAIN, USR_EVT_QUIT_GAME));
			items.push_back(new Screen::Link_Item("NE", PAUSED, NO_EVENT));
			screens[i] = Screen(items, PAUSED);
			break;
		}
	}

	Screen::Head_Item::Set_Level_Color(Color(1.0f, 0.0f, 0.0f, 1.0f), Screen::Head_Item::HEADING);
	Screen::Head_Item::Set_Level_Color(Color(0.0f, 0.0f, 1.0f, 1.0f), Screen::Head_Item::SUBHEADING);
	Screen::Head_Item::Set_Level_Color(Color(1.0f, 0.0f, 0.0f, 1.0f), Screen::Head_Item::MAIN_HEADING);
	Screen::Head_Item::Set_Level_Font("zektonbo.ttf", 30, Screen::Head_Item::HEADING);
	Screen::Head_Item::Set_Level_Font("zektonbo.ttf", 25, Screen::Head_Item::SUBHEADING);
	Screen::Head_Item::Set_Level_Font("zektonbo.ttf", 60, Screen::Head_Item::MAIN_HEADING);
	
	Screen::Link_Item::Set_State_Color(Color(0.0f, 1.0f, 0.0f, 1.0f), Screen::Link_Item::NORMAL);
	Screen::Link_Item::Set_State_Color(Color(1.0f, 1.0f, 1.0f, 1.0f), Screen::Link_Item::SELECTED);
	Screen::Link_Item::Set_Font("zekton.ttf", 20);

	Screen::Setting_Item::Set_State_Color(Color(0.0f, 1.0f, 0.0f, 1.0f), Screen::Setting_Item::NORMAL);
	Screen::Setting_Item::Set_State_Color(Color(1.0f, 1.0f, 1.0f, 1.0f), Screen::Setting_Item::SELECTED);
	Screen::Setting_Item::Set_State_Color(Color(1.0f, 1.0f, 0.0f, 1.0f), Screen::Setting_Item::EDITING);
	Screen::Setting_Item::Set_Font("zekton.ttf", 20);

	Set_Resolution(settings.Get_Resolution());
	
	Set_Section(MAIN);
}

void Menu::Set_Resolution(const SDL_Rect & resolution)
{
	Screen::Set_Resolution(resolution);
	Set_Items_Sizes();

	for (unsigned int i = 0; i < NUM_MENU_SECTIONS; i++)
	{
		screens[i].Set_Items_Positions();
		screens_paused[i].Set_Items_Positions();
	}
}

void Menu::Set_Settings(const Settings & settings)
{
	for (int i = 0; i < screens[CONTROLS].Get_Items_Count(); i++)
	{
		if (screens[CONTROLS][i]->Get_Name() == "HRÁČ 1")
		{
			dynamic_cast<Screen::Head_Item*>(screens[CONTROLS][i])->Set_Color(settings.Get_Setting_Value(Settings::PLAYER1_COLOR));
		}
		else if (screens[CONTROLS][i]->Get_Name() == "HRÁČ 2")
		{
			dynamic_cast<Screen::Head_Item*>(screens[CONTROLS][i])->Set_Color(settings.Get_Setting_Value(Settings::PLAYER2_COLOR));
		}
	}
}

void Menu::Draw() const
{
	glPushMatrix();

	if (paused && screens_paused[section].Get_Items_Height())
	{
		screens_paused[section].Draw();
	}
	else
	{
		screens[section].Draw();
	}
	
	glPopMatrix();
}

void Menu::Update(SDLKey key)
{
	Screen & screen = Get_Current_Screen();
	sections next_section = screen.Update(key);
	if (next_section != NO_SECTION) Set_Section(next_section);
}

void Menu::Update(int mouse_button)
{
	Screen & screen = Get_Current_Screen();
	sections next_section = screen.Update(mouse_button);
	if (next_section != NO_SECTION) Set_Section(next_section);
}

void Menu::Update(GLfloat y)
{
	Screen & screen = Get_Current_Screen();
	screen.Update(y);
}


