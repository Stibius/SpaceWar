
/** \file Settings.cpp
*   \brief Obsahuje definice metod a konstant tridy Settings.
*/

#include <climits>
#include <fstream>
#include "Math.h"
#include "Settings.h"
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::stringstream;

const bool Settings::DEFAULT_BOOL_SETTINGS[NUM_BOOL_SETTINGS] = 
{ 
	true,  //STAR
	true,  //FULLSCREEN
	false, //FPS
	true,  //GRID
	true,  //HEALTH_POWERUP_ALLOWED
	true,  //SHIELD_POWERUP_ALLOWED
	true,  //ENERGY_POWERUP_ALLOWED
	true,  //TRIPLE_POWERUP_ALLOWED
	true,  //GUIDED_POWERUP_ALLOWED
	true,  //LASER_POWERUP_ALLOWED
	true,  //ROCKET_POWERUP_ALLOWED
	true   //LASER_BEAM_POWERUP_ALLOWED
};

const int Settings::DEFAULT_INT_SETTINGS[NUM_INT_SETTINGS] = 
{ 
	50,   //SHIP_GRAVITY
	100,  //PROJECTILE_GRAVITY
	100,  //ACCELERATION
	100,  //DECELERATION
	20,   //PLAYER_RESISTANCE
	0,    //LASER_RESISTANCE
	0,    //ROCKET_RESISTANCE
	1000, //MAX_SPEED 
	144,  //PLAYER_ROTATION_SPEED
	70,   //GUIDED_MISSILE_ROTATION_SPEED
	500,  //LASER_SPEED
	300,  //ROCKET_SPEED
	40,   //LASER_DAMAGE
	60,   //ROCKET_DAMAGE
	20,   //LASER_BEAM_DAMAGE
	120,  //LASER_RATE
	500,  //ROCKET_RATE
	200,  //MAX_HEALTH
	200,  //MAX_SHIELD
	200,  //MAX_ENERGY
	1000, //SHIELD_REGENERATION_INTERVAL
	10,   //SHIELD_REGENERATION_AMOUNT
	1000, //ENERGY_REGENERATION_INTERVAL 
	10,   //ENERGY_REGENERATION_AMOUNT
	10,   //POWERUP_DISPLAY_DURATION
	10,   //POWERUP_MIN_INTERVAL
	10,   //POWERUP_MAX_INTERVAL
	100,  //HEALTH_POWERUP_AMOUNT
	100,  //SHIELD_POWERUP_AMOUNT
	100,  //ENERGY_POWERUP_AMOUNT
	30,   //TRIPLE_POWERUP_DURATION
	30,   //GUIDED_POWERUP_DURATION
	10,   //LASER_ENERGY_USAGE
	30,   //ROCKET_ENERGY_USAGE
	40,   //LASER_BEAM_ENERGY_USAGE
	500   //NUM_STARS
};

const SDLKey Settings::DEFAULT_CONTROLS[NUM_PLAYERS][NUM_SHIP_CONTROLS] =
{
	//FIRST
	{
	    SDLK_w, //ACCELERATE
	    SDLK_s, //DECELERATE
	    SDLK_a, //TURN_LEFT
	    SDLK_d, //TURN_RIGHT
	    SDLK_f  //FIRE
	},

	//SECOND
	{
		SDLK_u, //ACCELERATE
	    SDLK_j, //DECELERATE
	    SDLK_h, //TURN_LEFT
	    SDLK_k, //TURN_RIGHT
	    SDLK_l  //FIRE
	}
};

const SDL_Rect Settings::MIN_RESOLUTION = {0, 0, 800, 600};

const Color Settings::DEFAULT_COLOR_SETTINGS[NUM_COLOR_SETTINGS] = 
{ 
	Color(1.0f, 0.0f, 0.0f, 1.0f), //PLAYER1_COLOR
	Color(0.0f, 1.0f, 0.0f, 1.0f), //PLAYER2_COLOR
	Color(1.0f, 1.0f, 0.0f, 1.0f), //STAR_COLOR
	Color(0.0f, 0.0f, 1.0f, 1.0f)  //GRID_COLOR
};

const Settings::players_t Settings::DEFAULT_PLAYER_SETTINGS[NUM_PLAYER_SETTINGS] = 
{ 
	NONE //MOUSE_TURNING_PLAYER
};

const Settings::weapons_t Settings::DEFAULT_WEAPON_SETTINGS[NUM_WEAPON_SETTINGS] =
{
	LASER_WEAPON
};

const Color Settings::COLORS[NUM_COLORS] =
{
	Color(1.0f, 0.0f, 0.0f, 1.0f), //RED
	Color(0.0f, 1.0f, 0.0f, 1.0f), //GREEN
	Color(0.0f, 0.0f, 1.0f, 1.0f), //BLUE
	Color(1.0f, 1.0f, 0.0f, 1.0f), //YELLOW
	Color(1.0f, 0.0f, 1.0f, 1.0f), //MAGENTA
	Color(0.0f, 1.0f, 1.0f, 1.0f), //CYAN
	Color(1.0f, 1.0f, 1.0f, 1.0f), //WHITE
	Color(0.627f, 0.322f, 0.176f, 1.0f),  //BROWN
	Color(1.0f, 0.647f, 0.0f, 1.0f),  //ORANGE
	Color(1.0f, 0.412f, 0.706f, 1.0f)  //PINK
};

const string Settings::COLOR_NAMES[NUM_COLORS] =
{
	"červená",
	"zelená",
	"modrá",
	"žlutá",
	"fialová",
	"světle modrá",
	"bílá",
	"hnědá",
	"oranžová",
	"růžová"
};

const Settings::players_t Settings::PLAYER_VALUES[NUM_PLAYER_VALUES] = 
{
	FIRST,
	SECOND,
	NONE
};

const string Settings::PLAYER_NAMES[NUM_PLAYER_VALUES] =
{
	"první",
	"druhý",
	"žádný"
};

const Settings::weapons_t Settings::WEAPON_VALUES[NUM_WEAPONS] =
{
	LASER_WEAPON,
	ROCKET_WEAPON,
	LASER_BEAM_WEAPON
};

const string Settings::WEAPON_NAMES[NUM_WEAPONS] =
{
	"LASEROVÉ STŘELY",
	"RAKETY",
	"LASEROVÝ PAPRSEK"
};

const int Settings::MOUSE_BUTTONS[NUM_MOUSE_BUTTONS] = 
{
	SDL_BUTTON_LEFT,
    SDL_BUTTON_MIDDLE,
    SDL_BUTTON_RIGHT,
    SDL_BUTTON_WHEELUP,
    SDL_BUTTON_WHEELDOWN,
    SDL_BUTTON_X1,
    SDL_BUTTON_X2
};

const string Settings::MOUSE_BUTTON_NAMES[NUM_MOUSE_BUTTONS] =
{
	"LEVÉ TLAČÍTKO MYŠI",
	"PROSTŘEDNÍ TLAČÍTKO MYŠI",
	"PRAVÉ TLAČÍTKO MYŠI",
	"KOLEČKO MYŠI NAHORU",
	"KOLEČKO MYŠI DOLŮ",
	"X1",
	"X2"
};

const bool Settings::BOOL_VALUES[NUM_BOOL_VALUES] =
{
	true,
	false
};

const string Settings::BOOL_NAMES[NUM_BOOL_VALUES] =
{
	"ANO",
	"NE"
};

const SDLKey Settings::SDL_KEYS[NUM_KEYS] =
{
	SDLK_UNKNOWN,
	SDLK_BACKSPACE,
	SDLK_TAB,
	SDLK_CLEAR,
	SDLK_RETURN,
	SDLK_PAUSE,
	SDLK_ESCAPE,
	SDLK_SPACE,
	SDLK_EXCLAIM,
	SDLK_QUOTEDBL,
	SDLK_HASH,
	SDLK_DOLLAR,
	SDLK_AMPERSAND,
	SDLK_QUOTE,
	SDLK_LEFTPAREN,
	SDLK_RIGHTPAREN,
	SDLK_ASTERISK,
	SDLK_PLUS,
	SDLK_COMMA,
	SDLK_MINUS,
	SDLK_PERIOD,
	SDLK_SLASH,
	SDLK_0,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_COLON,
	SDLK_SEMICOLON,
	SDLK_LESS,
	SDLK_EQUALS,
	SDLK_GREATER,
	SDLK_QUESTION,
	SDLK_AT,
	SDLK_LEFTBRACKET,
	SDLK_BACKSLASH,
	SDLK_RIGHTBRACKET,
	SDLK_CARET,
	SDLK_UNDERSCORE,
	SDLK_BACKQUOTE,
	SDLK_a,
	SDLK_b,
	SDLK_c,
	SDLK_d,
	SDLK_e,
	SDLK_f,
	SDLK_g,
	SDLK_h,
	SDLK_i,
	SDLK_j,
	SDLK_k,
	SDLK_l,
	SDLK_m,
	SDLK_n,
	SDLK_o,
	SDLK_p,
	SDLK_q,
	SDLK_r,
	SDLK_s,
	SDLK_t,
	SDLK_u,
	SDLK_v,
	SDLK_w,
	SDLK_x,
	SDLK_y,
	SDLK_z,
	SDLK_DELETE,
	SDLK_WORLD_0,		
	SDLK_WORLD_1,
	SDLK_WORLD_2,
	SDLK_WORLD_3,
	SDLK_WORLD_4,
	SDLK_WORLD_5,
	SDLK_WORLD_6,
	SDLK_WORLD_7,
	SDLK_WORLD_8,
	SDLK_WORLD_9,
	SDLK_WORLD_10,
	SDLK_WORLD_11,
	SDLK_WORLD_12,
	SDLK_WORLD_13,
	SDLK_WORLD_14,
	SDLK_WORLD_15,
	SDLK_WORLD_16,
	SDLK_WORLD_17,
	SDLK_WORLD_18,
	SDLK_WORLD_19,
	SDLK_WORLD_20,
	SDLK_WORLD_21,
	SDLK_WORLD_22,
	SDLK_WORLD_23,
	SDLK_WORLD_24,
	SDLK_WORLD_25,
	SDLK_WORLD_26,
	SDLK_WORLD_27,
	SDLK_WORLD_28,
	SDLK_WORLD_29,
	SDLK_WORLD_30,
	SDLK_WORLD_31,
	SDLK_WORLD_32,
	SDLK_WORLD_33,
	SDLK_WORLD_34,
	SDLK_WORLD_35,
	SDLK_WORLD_36,
	SDLK_WORLD_37,
	SDLK_WORLD_38,
	SDLK_WORLD_39,
	SDLK_WORLD_40,
	SDLK_WORLD_41,
	SDLK_WORLD_42,
	SDLK_WORLD_43,
	SDLK_WORLD_44,
	SDLK_WORLD_45,
	SDLK_WORLD_46,
	SDLK_WORLD_47,
	SDLK_WORLD_48,
	SDLK_WORLD_49,
	SDLK_WORLD_50,
	SDLK_WORLD_51,
	SDLK_WORLD_52,
	SDLK_WORLD_53,
	SDLK_WORLD_54,
	SDLK_WORLD_55,
	SDLK_WORLD_56,
	SDLK_WORLD_57,
	SDLK_WORLD_58,
	SDLK_WORLD_59,
	SDLK_WORLD_60,
	SDLK_WORLD_61,
	SDLK_WORLD_62,
	SDLK_WORLD_63,
	SDLK_WORLD_64,
	SDLK_WORLD_65,
	SDLK_WORLD_66,
	SDLK_WORLD_67,
	SDLK_WORLD_68,
	SDLK_WORLD_69,
	SDLK_WORLD_70,
	SDLK_WORLD_71,
	SDLK_WORLD_72,
	SDLK_WORLD_73,
	SDLK_WORLD_74,
	SDLK_WORLD_75,
	SDLK_WORLD_76,
	SDLK_WORLD_77,
	SDLK_WORLD_78,
	SDLK_WORLD_79,
	SDLK_WORLD_80,
	SDLK_WORLD_81,
	SDLK_WORLD_82,
	SDLK_WORLD_83,
	SDLK_WORLD_84,
	SDLK_WORLD_85,
	SDLK_WORLD_86,
	SDLK_WORLD_87,
	SDLK_WORLD_88,
	SDLK_WORLD_89,
	SDLK_WORLD_90,
	SDLK_WORLD_91,
	SDLK_WORLD_92,
	SDLK_WORLD_93,
	SDLK_WORLD_94,
	SDLK_WORLD_95,		
	SDLK_KP0,
	SDLK_KP1,
	SDLK_KP2,
	SDLK_KP3,
	SDLK_KP4,
	SDLK_KP5,
	SDLK_KP6,
	SDLK_KP7,
	SDLK_KP8,
	SDLK_KP9,
	SDLK_KP_PERIOD,
	SDLK_KP_DIVIDE,
	SDLK_KP_MULTIPLY,
	SDLK_KP_MINUS,
	SDLK_KP_PLUS,
	SDLK_KP_ENTER,
	SDLK_KP_EQUALS,
	SDLK_UP,
	SDLK_DOWN,
	SDLK_RIGHT,
	SDLK_LEFT,
	SDLK_INSERT,
	SDLK_HOME,
	SDLK_END,
	SDLK_PAGEUP,
	SDLK_PAGEDOWN,
	SDLK_F1,
	SDLK_F2,
	SDLK_F3,
	SDLK_F4,
	SDLK_F5,
	SDLK_F6,
	SDLK_F7,
	SDLK_F8,
	SDLK_F9,
	SDLK_F10,
	SDLK_F11,
	SDLK_F12,
	SDLK_F13,
	SDLK_F14,
	SDLK_F15,
	SDLK_NUMLOCK,
	SDLK_CAPSLOCK,
	SDLK_SCROLLOCK,
	SDLK_RSHIFT,
	SDLK_LSHIFT,
	SDLK_RCTRL,
	SDLK_LCTRL,
	SDLK_RALT,
	SDLK_LALT,
	SDLK_RMETA,
	SDLK_LMETA,
	SDLK_LSUPER,		
	SDLK_RSUPER,		
	SDLK_MODE,		
	SDLK_COMPOSE,		
	SDLK_HELP,
	SDLK_PRINT,
	SDLK_SYSREQ,
	SDLK_BREAK,
	SDLK_MENU,
	SDLK_POWER,		
	SDLK_EURO,		
	SDLK_UNDO,
};

const string Settings::KEY_NAMES[NUM_KEYS] =
{ 
	"UNKNOWN",
    "BACKSPACE",
	"TAB",
	"CLEAR",
	"RETURN",
	"PAUSE",
	"ESCAPE",
	"SPACE",
	"EXCLAIM",
	"QUOTEDBL",
	"HASH",
	"DOLLAR",
	"AMPERSAND",
	"QUOTE",
	"LEFTPAREN",
	"RIGHTPAREN",
	"ASTERISK",
	"PLUS",
	"COMMA",
    "MINUS",
	"PERIOD",
	"SLASH",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"COLON",
	"SEMICOLON",
	"LESS",
	"EQUALS",
	"GREATER",
	"QUESTION",
	"AT",
    "LEFTBRACKET",
	"BACKSLASH",
	"RIGHTBRACKET",
	"CARET",
	"UNDERSCORE",
	"BACKQUOTE",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
    "O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"DELETE",
	"WORLD_0",
	"WORLD_1",
	"WORLD_2",
	"WORLD_3",
	"WORLD_4",
	"WORLD_5",
	"WORLD_6",
    "WORLD_7",
	"WORLD_8",
	"WORLD_9",
	"WORLD_10",
	"WORLD_11",
	"WORLD_12",
	"WORLD_13",
	"WORLD_14",
	"WORLD_15",
	"WORLD_16",
	"WORLD_17",
	"WORLD_18",
	"WORLD_19",
	"WORLD_20",
	"WORLD_21",
	"WORLD_22",
	"WORLD_23",
	"WORLD_24",
	"WORLD_25",
	"WORLD_26",
    "WORLD_27",
	"WORLD_28",
	"WORLD_29",
	"WORLD_30",
	"WORLD_31",
	"WORLD_32",
	"WORLD_33",
	"WORLD_34",
	"WORLD_35",
	"WORLD_36",
	"WORLD_37",
	"WORLD_38",
	"WORLD_39",
	"WORLD_40",
	"WORLD_41",
	"WORLD_42",
	"WORLD_43",
	"WORLD_44",
	"WORLD_45",
	"WORLD_46",
    "WORLD_47",
	"WORLD_48",
	"WORLD_49",
	"WORLD_50",
	"WORLD_51",
	"WORLD_52",
	"WORLD_53",
	"WORLD_54",
	"WORLD_55",
	"WORLD_56",
	"WORLD_57",
	"WORLD_58",
	"WORLD_59",
	"WORLD_60",
	"WORLD_61",
	"WORLD_62",
	"WORLD_63",
	"WORLD_64",
	"WORLD_65",
	"WORLD_66",
    "WORLD_67",
	"WORLD_68",
	"WORLD_69",
	"WORLD_70",
	"WORLD_71",
	"WORLD_72",
	"WORLD_73",
	"WORLD_74",
	"WORLD_75",
	"WORLD_76",
	"WORLD_77",
	"WORLD_78",
	"WORLD_79",
	"WORLD_80",
	"WORLD_81",
	"WORLD_82",
	"WORLD_83",
	"WORLD_84",
	"WORLD_85",
	"WORLD_86",
    "WORLD_87",
	"WORLD_88",
	"WORLD_89",
	"WORLD_90",
	"WORLD_91",
	"WORLD_92",
	"WORLD_93",
	"WORLD_94",
	"WORLD_95",
	"KP0",
	"KP1",
	"KP2",
	"KP3",
	"KP4",
	"KP5",
	"KP6",
	"KP7",
	"KP8",
	"KP9",
	"KP_PERIOD",
    "KP_DIVIDE",
	"KP_MULTIPLY",
	"KP_MINUS",
	"KP_PLUS",
	"KP_ENTER",
	"KP_EQUALS",
	"UP",
	"DOWN",
	"RIGHT",
	"LEFT",
	"INSERT",
	"HOME",
	"END",
	"PAGEUP",
	"PAGEDOWN",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
    "F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"NUMLOCK",
	"CAPSLOCK",
	"SCROLLOCK",
	"RSHIFT",
	"LSHIFT",
	"RCTRL",
	"LCTRL",
	"RALT",
	"LALT",
	"RMETA",
    "LMETA",
	"LSUPER",
	"RSUPER",
	"MODE",
	"COMPOSE",
	"HELP",
	"PRINT",
	"SYSREQ",
	"BREAK",
	"MENU",
	"POWER",
	"EURO",
	"UNDO"
};

string Settings::Setting::Find_Config_Value(string config_content) const
{
	size_t pos = 0; //pozice v souboru
	string label = Get_Label(); //oznaceni parametru v souboru
	if (label.empty()) return ""; //neni podle ceho hledat
	
	do {
	    pos = config_content.find(label, pos);
	    if (pos == string::npos) return ""; //parametr nebyl nalezen
	} while (pos != 0 && config_content[pos-1] != '\n'); //nazev parametru musi zacinat na novem radku

	pos += label.length(); //posuneme se za nazev parametru 

	while (pos < config_content.length() && config_content[pos] == ' ') pos++; //preskoceni mezery

	string value = "";

	//cteme hodnotu
	for (size_t i = pos; i < config_content.size(); i++) //cteme hodnotu potencialne az do konce souboru
	{
		char c = config_content[i];

		if (c != '\n')
		{
			value += c;
		}
		else 
		{
			return value;
		}
	}

	return value;
}

bool Settings::Int_Setting::Set_Value(int value) 
{ 
	if (value < min) 
	{ 
		this->value = min; 
		return false; 
	} 
	else if (value > max) 
	{
		this->value = max; 
		return false; 
	} 
	else
	{ 
		this->value = value; 
		return true; 
	} 
}

bool Settings::String_To_Bool(const string & s_value, bool* value) const
{
	if (s_value == "ANO")
	{
		if (value) *value = true;
		return true;
	}
	else if (s_value == "NE")
	{
		if (value) *value = false;
		return true;
	}
	else
		return false;
}

bool Settings::String_To_Resolution(const string & s_value, SDL_Rect & resolution) const
{
	size_t x_position = 0; //pozice v retezci, kde se nachazi znak 'x' rozdelujici sirku a vysku
	if ((x_position = s_value.find("x")) == string::npos || x_position == 0 || x_position == s_value.size()-1) 
		return false; //znak x chybu nebo je na spatnem miste

	//ziskame rozmery
	int w, h;
	if (!Math::String_To_Int(s_value.substr(0, x_position), &w))
	{
		return false;
	}

	if (!Math::String_To_Int(s_value.substr(x_position+1), &h))
	{
		return false;
	}

	resolution.w = w;
	resolution.h = h;

	return true;
}

string Settings::Resolution_To_String(SDL_Rect resolution) const
{
	return Math::Int_To_String(resolution.w) + "x" + Math::Int_To_String(resolution.h);
}

bool Settings::String_To_Key(const string & s_value, SDLKey* key) const
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		if (KEY_NAMES[i] == s_value)
		{
			*key =  SDL_KEYS[i];
			return true;
		}
	}

	return false;
}

string Settings::Key_To_String(SDLKey key) const
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		if (SDL_KEYS[i] == key)
		{
			return KEY_NAMES[i];
		}
	}

	return ""; 
}

bool Settings::String_To_Color(const string & s_value, Color & color) const
{
	for (int i = 0; i < NUM_COLORS; i++)
	{
		if (COLOR_NAMES[i] == s_value)
		{
			color = COLORS[i];
			return true;
		}
	}

	return false;
}

string Settings::Color_To_String(const Color & color) const
{
	for (int i = 0; i < NUM_COLORS; i++)
	{
		if (COLORS[i] == color)
		{
			return COLOR_NAMES[i];
		}
	}

	return "";
}

bool Settings::String_To_Mouse_Button(const string & s_value, int & mouse_button) const
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		if (MOUSE_BUTTON_NAMES[i] == s_value)
		{
			mouse_button = MOUSE_BUTTONS[i];
			return true;
		}
	}

	return false;
}

string Settings::Mouse_Button_To_String(const int & mouse_button) const
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		if (MOUSE_BUTTONS[i] == mouse_button)
		{
			return MOUSE_BUTTON_NAMES[i];
		}
	}

	return "";
}

bool Settings::String_To_Player(const string & s_value, players_t & player) const
{
	for (int i = 0; i < NUM_PLAYER_VALUES; i++)
	{
		if (PLAYER_NAMES[i] == s_value)
		{
			player = players_t(i);
			return true;
		}
	}

	return false;
}

bool Settings::String_To_Weapon(const string & s_value, weapons_t & weapon) const
{
	for (int i = 0; i < NUM_WEAPONS; i++)
	{
		if (WEAPON_NAMES[i] == s_value)
		{
			weapon = weapons_t(i);
			return true;
		}
	}

	return false;
}

template <typename Type>
Settings::Choice_Setting<Type>::Choice_Setting(string label, const Type * choices, int length) : Setting(label)
{
	Set_Choices(choices, length);
}

template <typename Type>
bool Settings::Choice_Setting<Type>::Set_Value(const Type & value)
{
	int new_index = 0;
	for (vector<Type>::iterator it = choices.begin(); it < choices.end(); it++)
	{
		if (*it == value)
		{
			index = new_index; 
			return true;
		}
		new_index++;
	}

	return false;
}

template <> bool Settings::Choice_Setting<SDL_Rect>::Set_Value(const SDL_Rect & value)
{
	int new_index = 0;
	for (vector<SDL_Rect>::iterator it = choices.begin(); it < choices.end(); it++)
	{
		if (it->w == value.w && it->h == value.h)
		{
			index = new_index; 
			return true;
		}
		new_index++;
	}

	return false;
}

template <typename Type>
void Settings::Choice_Setting<Type>::Set_Choices(const vector<Type> & new_choices)
{
	if (new_choices.empty())
	{
		choices = new_choices;
		return;
	}

	if (choices.empty())
	{
		choices = new_choices;
		index = choices.size()-1;
		return;
	}
	
	unsigned int new_current_choice_index = 0;

	for (vector<Type>::const_iterator it = new_choices.begin(); it < new_choices.end(); it++)
	{
		if (*it == choices[index]) 
		{
			break;
		}
		
	    new_current_choice_index++;
	}
	
	choices = new_choices;
	if (new_current_choice_index >= choices.size())
	{
		index = choices.size()-1;
	}
	else
	{
		index = new_current_choice_index;
	}
}

template <> void Settings::Choice_Setting<SDL_Rect>::Set_Choices(const vector<SDL_Rect> & new_choices)
{
	if (new_choices.empty())
	{
		choices = new_choices;
		return;
	}

	if (choices.empty())
	{
		choices = new_choices;
		index = choices.size()-1;
		return;
	}
	
	unsigned int new_current_choice_index = 0;

	for (vector<SDL_Rect>::const_iterator it = new_choices.begin(); it < new_choices.end(); it++)
	{
		if (it->w == choices[index].w && it->h == choices[index].h) break;
	    new_current_choice_index++;
	}

	choices = new_choices;
	if (new_current_choice_index >= choices.size())
	{
		index = choices.size()-1;
	}
	else
	{
		index = new_current_choice_index;
	}
}

template <typename Type>
void Settings::Choice_Setting<Type>::Set_Choices(const Type & val)
{
	choices = vector<Type>(1, val);
	index = choices.size()-1;
}

template <typename Type>
void Settings::Choice_Setting<Type>::Set_Choices(const Type * new_choices, int length)
{
	vector<Type> ch;
	for (int i = 0; i < length; i++)
	{
		ch.push_back(new_choices[i]);
	}

	Set_Choices(ch);
}

template <typename Type>
Type Settings::Choice_Setting<Type>::Get_Value() const 
{ 
	if (!choices.empty()) 
	{
		return choices[index];
	}
	else 
	{
		return Type(); 
	}
}

template <typename Type>
void Settings::Choice_Setting<Type>::Up()
{
	if (choices.size() == 0) 
	{
		return;
	}

	index++;
	if (index == choices.size())
		index = 0;
}

template <typename Type>
void Settings::Choice_Setting<Type>::Down()
{
	if (choices.size() == 0) return;

	if (index == 0)
		index = choices.size()-1;
	else
		index--;
}

bool Settings::Read_Config() 
{
	ifstream file;
	file.open("Config.txt");
	string file_content = "";
    
	if (file.is_open())
	{
		char c;
		do 
		{
			file.get(c);
			file_content += c;
		} while (!file.eof());
	}
	else 
		return false; //soubor se nepodarilo otevrit

	file.close();

	string s_value = ""; //hodnota z konfiguracniho souboru
	bool write_config = false; //zda se musi prepsat konfiguracni soubor

	//nacteni parametru typu bool
	for (int i = 0; i < NUM_BOOL_SETTINGS; i++)
	{
		if ((s_value = bool_settings[i].Find_Config_Value(file_content)) == "") 
			return false;
		bool value;
		if (!String_To_Bool(s_value, &value)) 
			return false;
		if (bool_settings[i].Get_Choices().empty())
		{
			bool_settings[i].Set_Choices(value);
		}
		else if (!bool_settings[i].Set_Value(value))
		{
			write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
		}
	}

	//nacteni parametru typu hrac
	for (int i = 0; i < NUM_PLAYER_SETTINGS; i++)
	{
		if ((s_value = player_settings[i].Find_Config_Value(file_content)) == "") 
			return false;
		players_t value;
		if (!String_To_Player(s_value, value)) 
			return false;
		if (player_settings[i].Get_Choices().empty())
		{
			player_settings[i].Set_Choices(value);
		}
		else if (!player_settings[i].Set_Value(value))
		{
			write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
		}
	}

	//nacteni parametru typu zbran
	for (int i = 0; i < NUM_WEAPON_SETTINGS; i++)
	{
		if ((s_value = weapon_settings[i].Find_Config_Value(file_content)) == "") 
			return false;
		weapons_t value;
		if (!String_To_Weapon(s_value, value)) 
			return false;
		if (weapon_settings[i].Get_Choices().empty())
		{
			weapon_settings[i].Set_Choices(value);
		}
		else if (!weapon_settings[i].Set_Value(value))
		{
			write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
		}
	}

	//nacteni parametru typu ovladaci prvek
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
	    for (int j = 0; j < NUM_SHIP_CONTROLS; j++)
	    {
		    if ((s_value = key_settings[i][j].Find_Config_Value(file_content)) == "") 
				return false;
		    SDLKey value;
		    int mouse_button;
		    if (String_To_Key(s_value, &value)) 
			{
                key_settings[i][j].Set_Key_Value(value);
			}
		    else if (String_To_Mouse_Button(s_value, mouse_button))
		    {
		    	key_settings[i][j].Set_Mouse_Button_Value(mouse_button);
		    }
		    else
		        return false;
	    }
	}

	//nacteni parametru typu int
	for (int i = 0; i < NUM_INT_SETTINGS; i++)
	{
		if ((s_value = int_settings[i].Find_Config_Value(file_content)) == "") 
			return false;
		int value;
		if (!Math::String_To_Int(s_value, &value)) 
			return false;
		if (!int_settings[i].Set_Value(value)) 
			write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
	}

	//nacteni parametru typu barva
	for (int i = 0; i < NUM_COLOR_SETTINGS; i++)
	{
		if ((s_value = color_settings[i].Find_Config_Value(file_content)) == "") 
			return false;
		Color value;
		if (!String_To_Color(s_value, value)) 
			return false;
		if (color_settings[i].Get_Choices().empty())
		{
			color_settings[i].Set_Choices(value);
		}
		else if (!color_settings[i].Set_Value(value))
		{
			write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
		}
	}

	//nacteni rozliseni
	if ((s_value = resolution.Find_Config_Value(file_content)) == "") 
		return false;
	SDL_Rect value;
	if (!String_To_Resolution(s_value, value)) 
		return false;
	if (resolution.Get_Choices().empty())
	{
		resolution.Set_Choices(value);
	}
	else if (!resolution.Set_Value(value))
	{
		write_config = true; //musi se zmenit konfiguracni soubor, protoze hodnota byla neplatna
	}

	if (write_config) 
		Write_Config();

	Send_Event(USR_EVT_SETTINGS_CHANGED);

	return true;
}

void Settings::Set_Int_Ranges()
{
	int_settings[LASER_ENERGY_USAGE].Set_Max(int_settings[MAX_ENERGY].Get_Value());
	int_settings[ROCKET_ENERGY_USAGE].Set_Max(int_settings[MAX_ENERGY].Get_Value());
	int_settings[LASER_BEAM_ENERGY_USAGE].Set_Max(int_settings[MAX_ENERGY].Get_Value());
	int_settings[SHIELD_REGENERATION_AMOUNT].Set_Max(int_settings[MAX_SHIELD].Get_Value());
	int_settings[ENERGY_REGENERATION_AMOUNT].Set_Max(int_settings[MAX_ENERGY].Get_Value());
	int_settings[HEALTH_POWERUP_AMOUNT].Set_Max(int_settings[MAX_HEALTH].Get_Value());
	int_settings[SHIELD_POWERUP_AMOUNT].Set_Max(int_settings[MAX_SHIELD].Get_Value());
	int_settings[ENERGY_POWERUP_AMOUNT].Set_Max(int_settings[MAX_ENERGY].Get_Value());
	int_settings[POWERUP_MIN_INTERVAL].Set_Max(int_settings[POWERUP_MAX_INTERVAL].Get_Value());
	int_settings[POWERUP_MAX_INTERVAL].Set_Min(int_settings[POWERUP_MIN_INTERVAL].Get_Value());
}

Settings::Settings()
{
	player_settings[MOUSE_TURNING_PLAYER] = Choice_Setting<players_t>("Hráč otáčený myší:", PLAYER_VALUES, NUM_PLAYER_VALUES);

	weapon_settings[DEFAULT_WEAPON] = Choice_Setting<weapons_t>("Implicitní zbraň:", WEAPON_VALUES, NUM_WEAPONS);

	bool_settings[STAR] = Choice_Setting<bool>("Hvězda:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[FULLSCREEN] = Choice_Setting<bool>("Fullscreen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[FPS] = Choice_Setting<bool>("Zobrazovat FPS:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[GRID] = Choice_Setting<bool>("Zobrazovat mřížku:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[HEALTH_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus doplnění životů povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[SHIELD_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus doplnění štítu povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[ENERGY_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus doplnění energie povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[TRIPLE_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus Trojité střely povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[GUIDED_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus Naváděné střely povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[LASER_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus Laserové střely povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[ROCKET_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus Rakety povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	bool_settings[LASER_BEAM_POWERUP_ALLOWED] = Choice_Setting<bool>("Bonus Laserový paprsek povolen:", BOOL_VALUES, NUM_BOOL_VALUES);
	
	int_settings[SHIP_GRAVITY] = Int_Setting("Gravitační zrychlení lodí:", " px/s/s");
	int_settings[PROJECTILE_GRAVITY] = Int_Setting("Gravitační zrychlení projektilů:", " px/s/s");
	int_settings[ACCELERATION] = Int_Setting("Zrychlení lodí vpřed:", " px/s/s", 0, INT_MAX);
	int_settings[DECELERATION] = Int_Setting("Zrychlení lodí vzad:", " px/s/s", 0, INT_MAX);
	int_settings[PLAYER_RESISTANCE] = Int_Setting("Samovolné zpomalování lodí:", " px/s/s", 0, INT_MAX);
	int_settings[LASER_RESISTANCE] = Int_Setting("Samovolné zpomalování laserových střel:", " px/s/s", 0, INT_MAX);
	int_settings[ROCKET_RESISTANCE] = Int_Setting("Samovolné zpomalování raket:", " px/s/s", 0, INT_MAX);
	int_settings[MAX_SPEED] = Int_Setting("Maximální rychlost lodí:", " px/s", 0, INT_MAX);
	int_settings[PLAYER_ROTATION_SPEED] = Int_Setting("Rychlost otáčení lodí:", " °/s", 0, INT_MAX);
	int_settings[GUIDED_MISSILE_ROTATION_SPEED] = Int_Setting("Rychlost otáčení naváděných projektilů:", " °/s", 0, INT_MAX);
	int_settings[LASER_SPEED] = Int_Setting("Počáteční rychlost laserových střel:", " px/s", 0, INT_MAX);
	int_settings[ROCKET_SPEED] = Int_Setting("Počáteční rychlost raket:", " px/s", 0, INT_MAX);
	int_settings[LASER_DAMAGE] = Int_Setting("Poškození laserovou střelou:", "", 0, INT_MAX);
	int_settings[ROCKET_DAMAGE] = Int_Setting("Poškození raketou:", "", 0, INT_MAX);
	int_settings[LASER_BEAM_DAMAGE] = Int_Setting("Poškození laserovým paprskem:", " / s", 0, INT_MAX);
	int_settings[LASER_RATE] = Int_Setting("Kadence laserových střel:", " ms", 0, INT_MAX);
	int_settings[ROCKET_RATE] = Int_Setting("Kadence raket:", " ms", 0, INT_MAX);
	int_settings[MAX_HEALTH] = Int_Setting("Maximum životů:", "", 1, INT_MAX);
	int_settings[MAX_SHIELD] = Int_Setting("Maximum štítu:", "", 0, INT_MAX);
	int_settings[MAX_ENERGY] = Int_Setting("Maximum energie:", "", 1, INT_MAX);
	int_settings[SHIELD_REGENERATION_INTERVAL] = Int_Setting("Interval regenerace štítu:", " ms", 0, INT_MAX);
	int_settings[SHIELD_REGENERATION_AMOUNT] = Int_Setting("Množství regenerovaného štítu:", "", 0, INT_MAX);
	int_settings[ENERGY_REGENERATION_INTERVAL] = Int_Setting("Interval regenerace energie:", " ms", 0, INT_MAX);
	int_settings[ENERGY_REGENERATION_AMOUNT] = Int_Setting("Množství regenerované energie:", "", 0, INT_MAX);
	int_settings[POWERUP_DISPLAY_DURATION] = Int_Setting("Doba zobrazení bonusů:", " s", 1, INT_MAX);
	int_settings[POWERUP_MIN_INTERVAL] = Int_Setting("Min. interval mezi bonusy:", " s", 1, INT_MAX);
	int_settings[POWERUP_MAX_INTERVAL] = Int_Setting("Max. interval mezi bonusy:", " s", 1, INT_MAX);
	int_settings[HEALTH_POWERUP_AMOUNT] = Int_Setting("Množství doplněných životů:", "", 0, INT_MAX);
	int_settings[SHIELD_POWERUP_AMOUNT] = Int_Setting("Množství doplněného štítu:", "", 0, INT_MAX);
	int_settings[ENERGY_POWERUP_AMOUNT] = Int_Setting("Množství doplněné energie:", "", 0, INT_MAX);
	int_settings[TRIPLE_POWERUP_DURATION] = Int_Setting("Trvání účinku bonusu Trojité střely:", " s", 0, INT_MAX);
	int_settings[GUIDED_POWERUP_DURATION] = Int_Setting("Trvání účinku bonusu Naváděné střely:", " s", 0, INT_MAX);
	int_settings[LASER_ENERGY_USAGE] = Int_Setting("Spotřeba energie laserových střel:", "", 0, INT_MAX);
	int_settings[ROCKET_ENERGY_USAGE] = Int_Setting("Spotřeba energie raket:", "", 0, INT_MAX);
	int_settings[LASER_BEAM_ENERGY_USAGE] = Int_Setting("Spotřeba energie laserového paprsku:", " / s", 0, INT_MAX);
	int_settings[NUM_STARS] = Int_Setting("Počet hvězd na pozadí:", "", 0, MAX_STARS);

	key_settings[FIRST][ACCELERATE] = Control_Setting("Dopředu 1:", DEFAULT_CONTROLS[FIRST][ACCELERATE]);
	key_settings[FIRST][DECELERATE] = Control_Setting("Dozadu 1:", DEFAULT_CONTROLS[FIRST][DECELERATE]);
	key_settings[FIRST][TURN_LEFT] = Control_Setting("Doleva 1:", DEFAULT_CONTROLS[FIRST][TURN_LEFT]);
	key_settings[FIRST][TURN_RIGHT] = Control_Setting("Doprava 1:", DEFAULT_CONTROLS[FIRST][TURN_RIGHT]);
	key_settings[FIRST][FIRE] = Control_Setting("Střelba 1:", DEFAULT_CONTROLS[FIRST][FIRE]);
	key_settings[SECOND][ACCELERATE] = Control_Setting("Dopředu 2:", DEFAULT_CONTROLS[SECOND][ACCELERATE]);
	key_settings[SECOND][DECELERATE] = Control_Setting("Dozadu 2:", DEFAULT_CONTROLS[SECOND][DECELERATE]);
	key_settings[SECOND][TURN_LEFT] = Control_Setting("Doleva 2:", DEFAULT_CONTROLS[SECOND][TURN_LEFT]);
	key_settings[SECOND][TURN_RIGHT] = Control_Setting("Doprava 2:", DEFAULT_CONTROLS[SECOND][TURN_RIGHT]);
	key_settings[SECOND][FIRE] = Control_Setting("Střelba 2:", DEFAULT_CONTROLS[SECOND][FIRE]);

	color_settings[PLAYER1_COLOR] = Choice_Setting<Color>("Barva hráče 1:", COLORS, NUM_COLORS);
	color_settings[PLAYER2_COLOR] = Choice_Setting<Color>("Barva hráče 2:", COLORS, NUM_COLORS);
	color_settings[STAR_COLOR] = Choice_Setting<Color>("Barva hvězdy:", COLORS, NUM_COLORS);
	color_settings[GRID_COLOR] = Choice_Setting<Color>("Barva mřížky:", COLORS, NUM_COLORS);

    resolution = Choice_Setting<SDL_Rect>("Rozlišení:");

	if (!Read_Config())
	{
		Set_Default_Settings();
	}

	Set_Int_Ranges();
}


bool Settings::Get_Setting_Value(bool_settings_t setting) const 
{ 
	return bool_settings[setting].Get_Value(); 
}

Color Settings::Get_Setting_Value(color_settings_t setting) const 
{ 
	return color_settings[setting].Get_Value(); 
}

Settings::players_t Settings::Get_Setting_Value(player_settings_t setting) const 
{ 
	return player_settings[setting].Get_Value(); 
}

Settings::weapons_t Settings::Get_Setting_Value(weapon_settings_t setting) const 
{ 
	return weapon_settings[setting].Get_Value(); 
}

SDL_Rect Settings::Get_Resolution() const 
{ 
	return resolution.Get_Value(); 
}

void Settings::Send_Event(int code) const
{
	SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = NULL; 
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
}

string Settings::Get_Setting_Value_Text(controls_t control, players_t player) const
{
	if (key_settings[player][control].Get_Type() == KEY)
	    return Key_To_String(key_settings[player][control].Get_Key_Value());
	else
		return Mouse_Button_To_String(key_settings[player][control].Get_Mouse_Button_Value());
}

string Settings::Get_Setting_Value_Text(color_settings_t setting) const 
{ 
	return Color_To_String(color_settings[setting].Get_Value()); 
}

string Settings::Get_Resolution_Text() const 
{ 
	return Resolution_To_String(resolution.Get_Value()); 
}

void Settings::Set_Default_Settings()
{
	for (int i = 0; i < NUM_PLAYER_SETTINGS; i++)
	{
		player_settings[i].Set_Value(DEFAULT_PLAYER_SETTINGS[i]);
	}

	for (int i = 0; i < NUM_WEAPON_SETTINGS; i++)
	{
		weapon_settings[i].Set_Value(DEFAULT_WEAPON_SETTINGS[i]);
	}

	for (int i = 0; i < NUM_PLAYERS; i++)
	{
	    for (int j = 0; j < NUM_SHIP_CONTROLS; j++)
	    {
		    key_settings[i][j].Set_Key_Value(DEFAULT_CONTROLS[i][j]);
	    }
	}

	for (int i = 0; i < NUM_INT_SETTINGS; i++)
	{
		int_settings[i].Set_Value(DEFAULT_INT_SETTINGS[i]);
	}
	
	for (int i = 0; i < NUM_BOOL_SETTINGS; i++)
	{
		bool_settings[i].Set_Choices(BOOL_VALUES, 2);
		bool_settings[i].Set_Value(DEFAULT_BOOL_SETTINGS[i]);
	}

	for (int i = 0; i < NUM_COLOR_SETTINGS; i++)
	{
		color_settings[i].Set_Choices(COLORS, NUM_COLORS);	
		color_settings[i].Set_Value(DEFAULT_COLOR_SETTINGS[i]);
	}

	Write_Config();

	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(bool_settings_t setting, bool value)
{
	bool_settings[setting].Set_Value(value);	
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(int_settings_t setting, int value)
{
	int_settings[setting].Set_Value(value);
	Set_Int_Ranges();
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(controls_t control, players_t player, SDLKey key)
{
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		for (int j = 0; j < NUM_SHIP_CONTROLS; j++)
	    {
			if ((i != player || j != control) && key_settings[i][j].Get_Type() == KEY && key_settings[i][j].Get_Key_Value() == key)
		    {
			    key_settings[i][j].Set_Key_Value(SDLK_UNKNOWN);
		    }
		}
	}
	
	key_settings[player][control].Set_Key_Value(key);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(controls_t control, players_t player, int mouse_button)
{
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		for (int j = 0; j < NUM_SHIP_CONTROLS; j++)
	    {
			if ((i != player || j != control) && key_settings[i][j].Get_Type() == MOUSE_BUTTON && key_settings[i][j].Get_Mouse_Button_Value() == mouse_button)
		    {
			    key_settings[i][j].Set_Key_Value(SDLK_UNKNOWN);
		    }
		}
	}
	
	key_settings[player][control].Set_Mouse_Button_Value(mouse_button);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(color_settings_t setting, const Color & value)
{
	color_settings[setting].Set_Value(value);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(player_settings_t setting, players_t player)
{
	player_settings[setting].Set_Value(player);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Setting(weapon_settings_t setting, weapons_t weapon)
{
	weapon_settings[setting].Set_Value(weapon);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Resolution(const SDL_Rect & rect)
{
	if (rect.w * rect.h < MIN_RESOLUTION.w * MIN_RESOLUTION.h) return;
	resolution.Set_Value(rect);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Resolution_Range(const vector<SDL_Rect> & range)
{
	vector<SDL_Rect> resolutions;

	for (vector<SDL_Rect>::const_iterator it = range.begin(); it < range.end(); it++)
	{
		if (it->w * it->h < MIN_RESOLUTION.w * MIN_RESOLUTION.h) continue;
		resolutions.push_back(*it);
	}

	resolution.Set_Choices(resolutions);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Set_Resolution_Range(SDL_Rect** resolutions)
{
	vector<SDL_Rect> resols;

	for (int i = 0; resolutions[i]; i++)
	{
		if (resolutions[i]->w * resolutions[i]->h < MIN_RESOLUTION.w * MIN_RESOLUTION.h) continue;
		resols.insert(resols.begin(), *resolutions[i]);
	}

	resolution.Set_Choices(resols);
	Write_Config();
	Send_Event(USR_EVT_SETTINGS_CHANGED);
}

void Settings::Resolution_Up() 
{ 
	resolution.Up(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
};

void Settings::Resolution_Down() 
{ 
	resolution.Down(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
};

void Settings::Color_Up(color_settings_t setting) 
{ 
	color_settings[setting].Up(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
};

void Settings::Color_Down(color_settings_t setting) 
{ 
	color_settings[setting].Down(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
}

void Settings::Player_Setting_Up(player_settings_t setting) 
{ 
	player_settings[setting].Up(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
}

void Settings::Player_Setting_Down(player_settings_t setting) 
{ 
	player_settings[setting].Down(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
}


void Settings::Weapon_Setting_Up(weapon_settings_t setting)
{ 
	weapon_settings[setting].Up(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
}

void Settings::Weapon_Setting_Down(weapon_settings_t setting)
{ 
	weapon_settings[setting].Down(); 
	Write_Config(); 
	Send_Event(USR_EVT_SETTINGS_CHANGED); 
}

string Settings::Get_Config_Entry(bool_settings_t setting) const
{
	string s_value = Bool_To_String(bool_settings[setting].Get_Value());
	return bool_settings[setting].Get_Label() + " " + s_value + "\n";
}

string Settings::Get_Config_Entry(int_settings_t setting) const
{
	int value;
	string s_value;
	value = int_settings[setting].Get_Value();
	s_value = Math::Int_To_String(value);
	
	return int_settings[setting].Get_Label() + " " + s_value + "\n";
}

string Settings::Get_Config_Entry(controls_t setting, players_t player) const
{
	if (key_settings[player][setting].Get_Type() == KEY)
	{
	    SDLKey value;
	    string s_value;
	    value = key_settings[player][setting].Get_Key_Value();
	    s_value = Key_To_String(value);
	    return key_settings[player][setting].Get_Label() + " " + s_value + "\n";
	}
	else
	{
		int value;
	    string s_value;
	    value = key_settings[player][setting].Get_Mouse_Button_Value();
	    s_value = Mouse_Button_To_String(value);
	    return key_settings[player][setting].Get_Label() + " " + s_value + "\n";
	}
}

string Settings::Get_Config_Entry(color_settings_t setting) const
{
	string s_value = Color_To_String(color_settings[setting].Get_Value());
	return color_settings[setting].Get_Label() + " " + s_value + "\n";
}

string Settings::Get_Config_Entry(player_settings_t setting) const
{
	string s_value = Player_To_String(player_settings[setting].Get_Value());
	return player_settings[setting].Get_Label() + " " + s_value + "\n";
}

string Settings::Get_Config_Entry(weapon_settings_t setting) const
{
	string s_value = Weapon_To_String(weapon_settings[setting].Get_Value());
	return weapon_settings[setting].Get_Label() + " " + s_value + "\n";
}

string Settings::Get_Resolution_Config_Entry() const
{
	string s_value = Resolution_To_String(resolution.Get_Value());
	return resolution.Get_Label() + " " + s_value + "\n";
}

void Settings::Write_Config() const
{
	ofstream file;
    
    file.open("Config.txt");

	if (file.is_open())
	{
		file << "GAME SETTINGS:\n";
		for (int i = 0; i < NUM_BOOL_SETTINGS; i++)
		{
			file << Get_Config_Entry(bool_settings_t(i));
		}

		for (int i = 0; i < NUM_INT_SETTINGS; i++)
		{
			file << Get_Config_Entry(int_settings_t(i));
		}

		file << "\n";

		for (int i = 0; i < NUM_PLAYER_SETTINGS; i++)
		{
			file << Get_Config_Entry(player_settings_t(i));
		}

		for (int i = 0; i < NUM_WEAPON_SETTINGS; i++)
		{
			file << Get_Config_Entry(weapon_settings_t(i));
		}

		file << "CONTROLS:\n";
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
		    for (int j= 0; j < NUM_SHIP_CONTROLS; j++)
		    {
			    file << Get_Config_Entry(controls_t(j), players_t(i));
		    }
		}
	
		file << "VIDEO:\n";
		file << Get_Resolution_Config_Entry();
		for (int i = 0; i < NUM_COLOR_SETTINGS; i++)
		{
			file << Get_Config_Entry(color_settings_t(i));
		}

        file.close();
	}
}









