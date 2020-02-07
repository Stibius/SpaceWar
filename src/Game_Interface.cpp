
/** \file Game_Interface.cpp
*   \brief Obsahuje definice metod a konstant tridy Game_Interface.
*/

#include "Game_Interface.h"
#include "Math.h"
#include "Point.h"
#include "Powerup_System.h"

int Game_Interface::num_instances = 0; 

Font Game_Interface::score_font; 
Font Game_Interface::fps_font;   
Texture Game_Interface::triple_powerup_indicator_text; 

const Player* Game_Interface::players[Settings::NUM_PLAYERS] = { NULL, NULL }; 

GLuint Game_Interface::health_symbol_list; 
GLuint Game_Interface::shield_symbol_list; 
GLuint Game_Interface::energy_symbol_list; 
GLuint Game_Interface::guided_powerup_indicator_list; 
GLuint Game_Interface::triple_powerup_indicator_list; 
GLuint Game_Interface::bar_list; 

const GLfloat Game_Interface::SYMBOL_BORDER_ALPHA = 1.0f; 
const GLfloat Game_Interface::SYMBOL_INSIDE_ALPHA = 0.2f; 
const GLfloat Game_Interface::BAR_BORDER_ALPHA = 1.0f; 
const GLfloat Game_Interface::BAR_INSIDE_ALPHA = 0.5f;  
const GLfloat Game_Interface::BORDERS_LINE_WIDTH = 2.0f; 
const GLfloat Game_Interface::POWERUP_INDICATOR_CIRLCE_LINE_WIDTH = 4.0f;

const GLfloat Game_Interface::BAR_WIDTH = 150.0f; 
const GLfloat Game_Interface::BAR_HEIGHT = 20.0f; 
const GLfloat Game_Interface::POWERUP_INDICATOR_RADIUS = 18.0f; 

const GLfloat Game_Interface::TOP_MARGIN = 10.0f;          
const GLfloat Game_Interface::LEFT_RIGHT_MARGIN = 10.0f; 
const GLfloat Game_Interface::SYMBOL_WIDTH = 20.0f;         
const GLfloat Game_Interface::SYMBOL_BAR_GAP = 10.0f;     
const GLfloat Game_Interface::BARS_VERTICAL_GAP = 10.0f;   
const GLfloat Game_Interface::BARS_POWERUP_INDICATOR_GAP = 10.0f; 
const GLfloat Game_Interface::POWERUP_INDICATORS_GAP = 10.0f; 

bool Game_Interface::initialized = false; 

void Game_Interface::Draw_Health_Symbol_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	health_symbol_list = i;

	glNewList(health_symbol_list, GL_COMPILE); 
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    glLineWidth(BORDERS_LINE_WIDTH);
		
		//vykresli neprusvitny obrys krize
	    glDisable(GL_BLEND);
		glBegin(GL_LINE_LOOP);
		    glVertex2f(7.0f, 0.0f);
		    glVertex2f(13.0f, 0.0f);
		    glVertex2f(13.0f, 7.0f);
		    glVertex2f(20.0f, 7.0f);
		    glVertex2f(20.0f, 13.0f);
		    glVertex2f(13.0f, 13.0f);
		    glVertex2f(13.0f, 20.0f);
		    glVertex2f(7.0f, 20.0f);
		    glVertex2f(7.0f, 13.0f);
		    glVertex2f(0.0f, 13.0f);
		    glVertex2f(0.0f, 7.0f);
		    glVertex2f(7.0f, 7.0f);
	    glEnd();
	
		//vykresli prusvitny vnitrek krize
	    glEnable(GL_BLEND);
	    glBegin(GL_QUADS);

		    //svisla cast krize
	        glVertex2f(7.0f, 0.0f);
		    glVertex2f(13.0f, 0.0f);
		    glVertex2f(13.0f, 20.0f);
		    glVertex2f(7.0f, 20.0f);

			//leva cast vodorovne casti krize
		    glVertex2f(0.0f, 7.0f);
		    glVertex2f(7.0f, 7.0f);
		    glVertex2f(7.0f, 13.0f);
		    glVertex2f(0.0f, 13.0f);

			//prava cast vodorovne casti krize
			glVertex2f(13.0f, 7.0f);
		    glVertex2f(20.0f, 7.0f);
		    glVertex2f(20.0f, 13.0f);
		    glVertex2f(13.0f, 13.0f);
	    glEnd();

	glEndList();	
}

void Game_Interface::Draw_Shield_Symbol_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	shield_symbol_list = i;

	glNewList(shield_symbol_list, GL_COMPILE); 
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    glLineWidth(BORDERS_LINE_WIDTH);

		for (int i = 0; i < 2; i++) //vykresleni obrysu a vnitrku 
		{
			if (i == 0)
			{
				//vykresli se neprusvitny obrys
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
			}
			else
			{
	            //vykresli se prusvitny vnitrek
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
			}

			//vykresli polygon stitu
		    glBegin(GL_POLYGON);
	            glVertex2f(3.0f, 0.0f);
		        glVertex2f(3.0f, 13.0f);
		        glVertex2f(10.0f, 20.0f);
		        glVertex2f(17.0f, 13.0f);
		        glVertex2f(17.0f, 0.0f);
	        glEnd();
		}

	glEndList();
}

void Game_Interface::Draw_Energy_Symbol_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	energy_symbol_list = i;

	glNewList(energy_symbol_list, GL_COMPILE); 
	    glLineWidth(BORDERS_LINE_WIDTH);

		//vykresli se neprusvitny obrys
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_BLEND);
			
		glBegin(GL_LINE_LOOP);
		    glVertex2f(9.0f, 0.0f);
			glVertex2f(4.0f, 10.0f);
		    glVertex2f(9.0f, 10.0f);
			glVertex2f(4.0f, 20.0f);
			glVertex2f(16.0f, 6.25f);
			glVertex2f(11.0f, 6.25f);
	        glVertex2f(14.0f, 0.0f);
	    glEnd();
			
	    //vykresli se vnitrek
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_BLEND);

		//leva cast blesku
		glBegin(GL_QUADS);
	        glVertex2f(9.0f, 0.0f);
		    glVertex2f(4.0f, 10.0f);
			glVertex2f(9.0f, 10.0f);
	        glVertex2f(14.0f, 0.0f);
	    glEnd();

		//prava cast blesku
		glBegin(GL_TRIANGLES);
			glVertex2f(4.0f, 20.0f);
			glVertex2f(16.0f, 6.25f);
			glVertex2f(11.0f, 6.25f);
	    glEnd();
			
	glEndList();
}

void Game_Interface::Draw_Guided_Powerup_Indicator_List()
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	guided_powerup_indicator_list = i;

	glNewList(guided_powerup_indicator_list, GL_COMPILE); 
	    glLineWidth(BORDERS_LINE_WIDTH);
	    glDisable(GL_BLEND);

		Circle::Draw(Point(0.0f, 0.0f), 10.0f, false); //vykresleni kruhove casti 

		//vykresleni car
	    glBegin(GL_LINES);

		    //horni vodorovna carka
	        glVertex2f(-2.5f, -15.0f);
		    glVertex2f(2.5f, -15.0f);

			//prava svisla carka
		    glVertex2f(15.0f, -2.5f);
		    glVertex2f(15.0f, 2.5f);

			//dolni vodorovna carka
		    glVertex2f(-2.5f, 15.0f);
		    glVertex2f(2.5f, 15.0f);

			//leva svisla carka
	        glVertex2f(-15.0f, -2.5f);
		    glVertex2f(-15.0f, 2.5f);

			//hlavni vodorovna cara
		    glVertex2f(-15.0f, 0.0f);
		    glVertex2f(15.0f, 0.0f);

			//hlavni svisla cara
		    glVertex2f(0.0f, -15.0f);
		    glVertex2f(0.0f, 15.0f);

	    glEnd();

	    glEnable(GL_BLEND);
	   
	glEndList();
}

void Game_Interface::Draw_Triple_Powerup_Indicator_List()
{
	Font::Create_Texture("zektonbo.ttf", 25, "3x", triple_powerup_indicator_text); //vytvoreni textury s textem powerupu
	GLfloat w = Font::Get_Text_Width("zektonbo.ttf", 25, "3x");
	GLfloat h = 25;

	GLuint i = 1;
	while (glIsList(i))
		i++;
	triple_powerup_indicator_list = i;

	glNewList(triple_powerup_indicator_list, GL_COMPILE); 
			triple_powerup_indicator_text.Draw(Point(-w/2.0f, -h/2.0f)); //vypsani textu tak, aby byl v obou osach vycentrovany na prave nastavenou pozici
	glEndList();
}

void Game_Interface::Draw_Bar_List() 
{
	GLuint i = 1;
	while (glIsList(i))
		i++;
	bar_list = i;

	glNewList(bar_list, GL_COMPILE); 
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    glLineWidth(BORDERS_LINE_WIDTH);
		glDisable(GL_BLEND);

		//vykresli obrys obdelnikoveho ukazatele
		glBegin(GL_QUADS);
	        glVertex2f(0.0f, 0.0f);
		    glVertex2f(BAR_WIDTH, 0.0f);
		    glVertex2f(BAR_WIDTH, BAR_HEIGHT);
		    glVertex2f(0.0f, BAR_HEIGHT);
	    glEnd();

		glEnable(GL_BLEND);
	glEndList();
}

void Game_Interface::Draw_Filled_Bar(const Point & point, const Color & color, GLfloat portion) const
{
	if (portion > 1.0f) portion = 1.0f;
	if (portion < 0.0f) portion = 0.0f;

	glLoadIdentity();
	glTranslatef(point.x, point.y, 0.0f);
	glColor4f(color.red, color.green, color.blue, BAR_BORDER_ALPHA);
	glCallList(bar_list); //vykresli obrys obdelnikoveho ukazatele

	glColor4f(color.red, color.green, color.blue, BAR_INSIDE_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);

	//vykresli vnitrek obdelnikoveho ukazatele
	glBegin(GL_QUADS);
	    glVertex2f(0.0f, 0.0f);
		glVertex2f(BAR_WIDTH * portion, 0.0f);
		glVertex2f(BAR_WIDTH * portion, BAR_HEIGHT);
		glVertex2f(0.0f, BAR_HEIGHT);
	glEnd();
	    
	glLoadIdentity();
}

void Game_Interface::Draw_Guided_Powerup_Indicator(const Point & point, const Color & color, GLfloat portion) const
{
	if (portion > 1.0f) portion = 1.0f;
	if (portion < 0.0f) portion = 0.0f;

	glLoadIdentity();
	glTranslatef(point.x, point.y, 0.0f);
	glColor4f(color.red, color.green, color.blue, SYMBOL_BORDER_ALPHA);
	glCallList(guided_powerup_indicator_list); //vykresli znak powerupu

	glLineWidth(POWERUP_INDICATOR_CIRLCE_LINE_WIDTH);
	
	glLoadIdentity();

	//vykresli pomernou cast kruznice
	if (portion  == 1.0f)
		Circle::Draw(point, POWERUP_INDICATOR_RADIUS);
	else
	    Circle::Draw_Arc(point, POWERUP_INDICATOR_RADIUS, 360-(portion*360), 360);    

	glLoadIdentity();
}

void Game_Interface::Draw_Triple_Powerup_Indicator(const Point & point, const Color & color, GLfloat portion) const
{
	if (portion > 1.0f) portion = 1.0f;
	if (portion < 0.0f) portion = 0.0f;

	glLoadIdentity();
	glTranslatef(point.x, point.y, 0.0f);
	glColor4f(color.red, color.green, color.blue, 1.0f);

	//ulozit puvodni hodnoty blendfunc
	GLint dst, src;
	glGetIntegerv(GL_BLEND_DST, &dst);
	glGetIntegerv(GL_BLEND_SRC, &src);
	glBlendFunc(GL_ONE, GL_ONE); //Required if you want alpha-blended textures (for our fonts) 

	glCallList(triple_powerup_indicator_list); //vykresli text powerupu

	glBlendFunc(src, dst); //vratit buvodni hodnoty blendfunc

	glLineWidth(POWERUP_INDICATOR_CIRLCE_LINE_WIDTH);
	glLoadIdentity();

	//vykresli pomernou cast kruznice
	if (portion == 1.0f)
		Circle::Draw(point, POWERUP_INDICATOR_RADIUS);
	else
	    Circle::Draw_Arc(point, POWERUP_INDICATOR_RADIUS, 360-(portion*360), 360);   

	glLoadIdentity();
}

void Game_Interface::Draw_Powerup_Indicators(const Player & player) const
{
	GLfloat x;
	if (player.Get_Player_Number() == Settings::FIRST)
	    x  = LEFT_RIGHT_MARGIN + SYMBOL_WIDTH + SYMBOL_BAR_GAP + BAR_WIDTH + BARS_POWERUP_INDICATOR_GAP + POWERUP_INDICATOR_RADIUS;
	else
		x = resolution.w - LEFT_RIGHT_MARGIN - BAR_WIDTH - SYMBOL_BAR_GAP - SYMBOL_WIDTH - BARS_POWERUP_INDICATOR_GAP - POWERUP_INDICATOR_RADIUS;
	GLfloat y = TOP_MARGIN + POWERUP_INDICATOR_RADIUS;

	Uint32 rtt = player.Get_Triple_Remaining_Time();
	if (rtt != 0)
	{
	    GLfloat portion = GLfloat(rtt) / (Powerup_System::Triple_Powerup::Get_Effect_Duration() * 1000);
	    Draw_Triple_Powerup_Indicator(Point(x, y), player.Get_Color(), portion);
	    y += (2 * POWERUP_INDICATOR_RADIUS) + POWERUP_INDICATORS_GAP;
	}

	Uint32 rtg = player.Get_Guided_Remaining_Time();
	if (rtg != 0)
	{
		GLfloat portion = GLfloat(rtg) / (Powerup_System::Guided_Powerup::Get_Effect_Duration() * 1000);
		Draw_Guided_Powerup_Indicator(Point(x, y), player.Get_Color(), portion);
		y += (2 * POWERUP_INDICATOR_RADIUS) + POWERUP_INDICATORS_GAP;
	}
}

void Game_Interface::Draw() const
{
	Draw_Powerup_Indicators(*players[Settings::FIRST]);
	Draw_Powerup_Indicators(*players[Settings::SECOND]);

	Draw_Bars(*players[Settings::FIRST]);
	Draw_Bars(*players[Settings::SECOND]);

	Draw_Score();
}

void Game_Interface::Draw_Bars(const Player & player) const
{
	const Color & color = player.Get_Color();

	GLfloat y = TOP_MARGIN;

	glLoadIdentity();

	if (player.Get_Player_Number() == Settings::FIRST)
	    glTranslatef(LEFT_RIGHT_MARGIN, y, 0.0f);
	else
		glTranslatef(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH-SYMBOL_BAR_GAP-SYMBOL_WIDTH, y, 0.0f);
	glColor4f(color.red, color.green, color.blue, SYMBOL_INSIDE_ALPHA);
	glCallList(health_symbol_list);
	GLfloat health_portion = GLfloat(player.Get_Health()) / player.Get_Max_Health();
	if (player.Get_Player_Number() == Settings::FIRST)
	    Draw_Filled_Bar(Point(LEFT_RIGHT_MARGIN+SYMBOL_WIDTH+SYMBOL_BAR_GAP, y), player.Get_Color(), health_portion);
	else
		Draw_Filled_Bar(Point(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH, y), player.Get_Color(), health_portion);

	if (Player::Get_Max_Shield() > 0)
	{
		y += BAR_HEIGHT+BARS_VERTICAL_GAP;
		glLoadIdentity();
		if (player.Get_Player_Number() == Settings::FIRST)
	        glTranslatef(LEFT_RIGHT_MARGIN, y, 0.0f);
		else
			glTranslatef(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH-SYMBOL_BAR_GAP-SYMBOL_WIDTH, y, 0.0f);
	    glColor4f(color.red, color.green, color.blue, SYMBOL_INSIDE_ALPHA);
	    glCallList(shield_symbol_list);
	    GLfloat shield_portion = GLfloat(player.Get_Shield()) / player.Get_Max_Shield();
		if (player.Get_Player_Number() == Settings::FIRST)
	        Draw_Filled_Bar(Point(LEFT_RIGHT_MARGIN+SYMBOL_WIDTH+SYMBOL_BAR_GAP, y), player.Get_Color(), shield_portion);
		else
			Draw_Filled_Bar(Point(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH, y), player.Get_Color(), shield_portion);
	}

	y += BAR_HEIGHT+BARS_VERTICAL_GAP;

	if (player.Get_Weapon_Energy_Usage() > 0)
	{
		glLoadIdentity();
		if (player.Get_Player_Number() == Settings::FIRST)
	        glTranslatef(LEFT_RIGHT_MARGIN, y, 0.0f);
		else
			glTranslatef(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH-SYMBOL_BAR_GAP-SYMBOL_WIDTH, y, 0.0f);
	    glColor4f(color.red, color.green, color.blue, SYMBOL_INSIDE_ALPHA);
	    glCallList(energy_symbol_list);
	    GLfloat energy_portion = GLfloat(player.Get_Energy()) / player.Get_Max_Energy();
		if (player.Get_Player_Number() == Settings::FIRST)
	        Draw_Filled_Bar(Point(LEFT_RIGHT_MARGIN+SYMBOL_WIDTH+SYMBOL_BAR_GAP, y), player.Get_Color(), energy_portion);
		else
			Draw_Filled_Bar(Point(resolution.w-LEFT_RIGHT_MARGIN-BAR_WIDTH, y), player.Get_Color(), energy_portion);
	}

	glLoadIdentity();
}

void Game_Interface::Draw_FPS(GLfloat fps) const
{
	static Uint32 last = 0;

	if (show_fps)
	{
		static GLfloat value;
		if (SDL_GetTicks() > last + FPS_RATE)
		{
		    value = fps;
			last = SDL_GetTicks();
		}
		string fps_s = "FPS: " + Math::Float_To_String(value, 2);
		fps_font.Draw_Text(fps_s, Point((resolution.w / 2) - fps_font.Get_Text_Width("FPS: "), resolution.h - fps_font.Get_Font_Height()), Color(1.0f, 1.0f, 1.0f));
	}
}

void Game_Interface::Draw_Score() const
{
	string score1 = Math::Int_To_String(players[Settings::FIRST]->Get_Score());
	string score2 = Math::Int_To_String(players[Settings::SECOND]->Get_Score());

	GLfloat y = TOP_MARGIN;

	score_font.Draw_Text(score1, Point((resolution.w / 2) - score_font.Get_Text_Width(score1) - score_font.Get_Text_Width(" "), y), players[Settings::FIRST]->Get_Color());
	score_font.Draw_Text(":", Point((resolution.w / 2) - (score_font.Get_Text_Width(":")/ 2), y), Color(1.0f, 1.0f, 1.0f));
	score_font.Draw_Text(score2, Point((resolution.w / 2) + score_font.Get_Text_Width(" "), y), players[Settings::SECOND]->Get_Color());
}

Game_Interface::Game_Interface(const Player & player1, const Player & player2, const Settings & settings)
{
	num_instances++;

	if (initialized == false)
	{
		fps_font = Font("zekton.ttf", 30);
		score_font = Font("zekton.ttf", 50);
		players[Settings::FIRST] = &player1;
		players[Settings::SECOND] = &player2;
        Draw_Health_Symbol_List();
	    Draw_Shield_Symbol_List();
	    Draw_Energy_Symbol_List();
	    Draw_Guided_Powerup_Indicator_List();
	    Draw_Triple_Powerup_Indicator_List();
	    Draw_Bar_List();
		initialized = true;
	}

	Set_Settings(settings);
}

Game_Interface::~Game_Interface()
{
	num_instances--;

	if (!num_instances && initialized == true)
	{
	    glDeleteLists(health_symbol_list, 1);
		glDeleteLists(shield_symbol_list, 1);
		glDeleteLists(energy_symbol_list, 1);
		glDeleteLists(triple_powerup_indicator_list, 1);
		glDeleteLists(guided_powerup_indicator_list, 1);
		glDeleteLists(bar_list, 1);
		initialized = false;
	}
}

void Game_Interface::Set_Settings(const Settings & settings)
{
	resolution = settings.Get_Resolution();
	show_fps = settings.Get_Setting_Value(Settings::FPS);
}