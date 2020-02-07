
/** \file Font.cpp
*   \brief Obsahuje definice metod tridy Font.
*/

#include "Font.h"
#include "Math.h"

const string Font::czech_characters = "ÁČĎÉĚÍŇÓŘŠŤÚŮÝŽáčďéěíňóřšťúůýž°"; 
int Font::num_instances = 0;

map<int, Font::DL_map> & Font::Get_Maps()
{
	static map<int, DL_map>* ans = new map<int, DL_map>;
    return *ans;
}

vector<Uint16> Font::Get_UTF8_Codes(const string & text)
{
	vector<Uint16> result;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] >= 0 && text[i] <= ASCII_LAST) //znak z ASCII tabulky
		{
			result.push_back(text[i]);
			continue;
		}

		//jde o cesky znak

		string czch = text.substr(i, 2);

		if (czch == "Á")
			result.push_back(193);
		else if (czch == "á")
			result.push_back(225);
		else if (czch == "Č")
			result.push_back(268);
		else if (czch == "č")	
			result.push_back(269);
		else if (czch == "Ď")
			result.push_back(270);
		else if (czch == "ď")
			result.push_back(271);
		else if (czch == "É")
			result.push_back(201);
		else if (czch == "é")
			result.push_back(233);
		else if (czch == "Ě")	
			result.push_back(282);
		else if (czch == "ě")
			result.push_back(283);
		else if (czch == "Í")
			result.push_back(205);
		else if (czch == "í")
			result.push_back(237);
		else if (czch == "Ň")
			result.push_back(327);
		else if (czch == "ň")
			result.push_back(328);
		else if (czch == "Ó")
			result.push_back(211);
		else if (czch == "ó")
			result.push_back(243);
		else if (czch == "Ř")
			result.push_back(344);
		else if (czch == "ř")
			result.push_back(345);
		else if (czch == "Š")
			result.push_back(352);
		else if (czch == "š")
			result.push_back(353);
		else if (czch == "Ť")
			result.push_back(356);
		else if (czch == "ť")
			result.push_back(357);
		else if (czch == "Ú")
			result.push_back(218);
		else if (czch == "ú")
			result.push_back(250);
		else if (czch == "Ů")
			result.push_back(366);
		else if (czch == "ů")
			result.push_back(367);
		else if (czch == "Ý")
			result.push_back(221);
		else if (czch == "ý")
			result.push_back(253);
		else if (czch == "Ž")
			result.push_back(381);
		else if (czch == "ž")
			result.push_back(382);
		else if (czch == "°")
			result.push_back(176);
		else 
			result.push_back(32);
		
		i++;
	}
	return result;
}

Font::Font(const string & path, int size) : font(NULL)
{
	num_instances++;
	Load_Font(path, size);
}

Font::Font(const Font & text)
	: color(text.color), font(text.font), font_path(text.font_path), scale(text.scale), map_key(text.map_key)
{
	num_instances++;
	static map<int, DL_map> & maps = Get_Maps();
	maps[map_key].uses++;
}

Font::~Font()
{
	Close_Font();
	num_instances--;

	if (num_instances == 0 && TTF_WasInit())
	{
		TTF_Quit();
	}
}

Font & Font::operator=(const Font & text)
{
	if (this == &text)    
        return *this;     
	
	Close_Font(); //vymazani pripadneho predchoziho fontu
	color = text.color;
	font = text.font;
	font_path = text.font_path;
	scale = text.scale;
	map_key = text.map_key;
	static map<int, DL_map> & maps = Get_Maps();
	maps[map_key].uses++;

	return *this;
}

bool Font::Load_Font(const string & path, int size)
{
	if (font != NULL) Close_Font(); //zavreni pripadneho drive nahraneho fontu

	if (!TTF_WasInit()) //inicializace SDL_TTF
	{
	    if (TTF_Init()) return false;
	}
	
	if (!(font = TTF_OpenFont(path.c_str(), size))) //nepodarilo se otevrit font
	{
		scale = 1.0;
		map_key = NO_MAP;
		return false; 
	}
	else
	{
		font_path = path;
	    scale = GLfloat(size) / TTF_FontHeight(font);
		this->color = color;
		TTF_SetFontKerning(font, 0); //vypnuti kerningu, aby sly presne pocitat pozice pismen
		if (!Create_Textures()) return false;
		return true;
	}
}

void Font::Close_Font()
{
	if (map_key != NO_MAP) //objekt je asociovany s nejakym nahranym fontem
	{
		static map<int, DL_map> & maps = Get_Maps();

		maps[map_key].uses--;
		
		if (maps[map_key].uses == 0) //zadny jiny objekt tento font nepouziva, vymazeme display-listy, DL_map a font
	    {
			for (map<Uint16, GLuint>::iterator it = maps[map_key].display_lists.begin(); it != maps[map_key].display_lists.end(); it++)
			{
				glDeleteLists((*it).second, 1);
			}
			maps.erase(map_key);
			if (font != NULL) 
			{ 
				TTF_CloseFont(font); 
				font = NULL; 
			}
		}	
	}
}

bool Font::Create_Texture(const string & path, int size, string text, Texture & texture)
{
	//http://www.gamedev.net/topic/284259-for-reference-using-sdl_ttf-with-opengl/

	TTF_Font* font;
	if(!(font = TTF_OpenFont(path.c_str(), size)))
	{
		return false;
	}

	GLfloat scale = GLfloat(size) / TTF_FontHeight(font);
	
	if(!(font = TTF_OpenFont(path.c_str(), int(size*scale)))) //znovu nacteme tak, aby realna vyska fontu opravdu odpovidala pozadovane
	{
		return false;
	}

	SDL_Surface *initial;
	SDL_Color s_color = { 255, 255, 255 }; 

	if (!(initial = TTF_RenderText_Blended(font, text.c_str(), s_color))) //vyrenderuje text
	{
		return false;
	}

	//rozmery textury museji byt mocninou 2
	int texture_width = Math::Next_Power_Of_Two(initial->w);
	int texture_height = Math::Next_Power_Of_Two(initial->h);

	//vytvorime pomocny surface ve spravnem formatu pro texturu
	SDL_Surface *intermediary;
	if (!(intermediary = SDL_CreateRGBSurface(0, texture_width, texture_height, 32,   
	//Při vytváření surface je vhodné specifikovat masky v závislosti na pořadí bytů, které se používá na dané platformě (little/big endian).
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    #else
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    #endif
        ))) 
	{
		SDL_FreeSurface(initial);
		return false;
	}

	//zkopirovat pixely do noveho surface
	if (SDL_BlitSurface(initial, 0, intermediary, 0) != 0)
	{
		SDL_FreeSurface(initial);
	    SDL_FreeSurface(intermediary);
		return false;
	}

	//vytvoreni textury
	texture = Texture(intermediary->w, intermediary->h, GL_UNSIGNED_BYTE, intermediary->pixels, GL_CLAMP, GL_CLAMP);

	//cleanup
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);

	return true;
}

bool Font::Create_Texture(Uint16 character, DL_map & map) const
{
	//http://www.gamedev.net/topic/284259-for-reference-using-sdl_ttf-with-opengl/

	if (!font) 
    {
		return false;
	}

	SDL_Surface *initial;
	SDL_Color s_color = { 255, 255, 255 }; 
	if (!(initial = TTF_RenderGlyph_Blended(font, character, s_color))) //vyrenderuje text
    {
		return false;
	}

	int advance, minx, maxy;
	if (TTF_GlyphMetrics(font, character, &minx, NULL, NULL, &maxy, &advance) == -1)//zjistime potrebne metriky znaku
	{
		return false;
	}

	//rozmery textury museji byt mocninou 2
	int texture_width = Math::Next_Power_Of_Two(initial->w);
	int texture_height = Math::Next_Power_Of_Two(initial->h);

	//vytvorime pomocny surface ve spravnem formatu pro texturu
	SDL_Surface *intermediary;
	if (!(intermediary = SDL_CreateRGBSurface(0, texture_width, texture_height, 32,  
	//Při vytváření surface je vhodné specifikovat masky v závislosti na pořadí bytů, které se používá na dané platformě (little/big endian).
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    #else
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    #endif
        ))) 
	{
		SDL_FreeSurface(initial);
		return false;
	}

	//zkopirovat pixely do noveho surface
	if (SDL_BlitSurface(initial, 0, intermediary, 0) != 0)
	{
		SDL_FreeSurface(initial);
	    SDL_FreeSurface(intermediary);
		return false;
	}

	//vytvoreni textury
	Texture texture = Texture(intermediary->w, intermediary->h, GL_UNSIGNED_BYTE, intermediary->pixels, GL_CLAMP, GL_CLAMP);
	map.textures.push_back(texture);

	//cleanup
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);

	//vygenerujeme id display-listu
	GLuint id = 1;
	while (glIsList(id)) 
		id++;
	map.display_lists[character] = id; //aktualizujeme mapu

	//vykreslime texturu do DL
	glNewList(id, GL_COMPILE);
	    map.textures[map.textures.size()-1].Draw(Point(GLfloat(minx), GLfloat(TTF_FontAscent(font) - maxy)));
		glTranslatef(GLfloat(advance), 0.0f, 0.0f);
	glEndList();

	return true;
}

bool Font::Create_Textures()
{
	DL_map new_map;

	//vytvoreni textur pro ASCII znaky
	for (char ch = ASCII_FIRST; ch <= ASCII_LAST; ch++)
	{
		if (!Create_Texture(ch, new_map)) return false;	
	}
	
	vector<Uint16> czchs = Get_UTF8_Codes(czech_characters);

	//vytvoreni textur pro ceske znaky
	for (unsigned int i = 0; i < czchs.size(); i++)
	{
		if (!Create_Texture(czchs[i], new_map)) return false;
	}	

	//pridani nove mapy
	new_map.uses = 1;
	static map<int, DL_map> & maps = Get_Maps();
	if (maps.empty())
		map_key = 0;
	else
	    map_key = (*--maps.end()).first + 1;
	maps[map_key] = new_map;

	return true;
}

vector<GLuint> Font::Characters_To_Lists(const string & characters) const
{
	vector<GLuint> result;
	if (map_key == NO_MAP) return result;

	vector<Uint16> codes = Get_UTF8_Codes(characters);
	static map<int, DL_map> & maps = Get_Maps();

	for (unsigned int i = 0; i < codes.size(); i++)
	{
		map<Uint16, GLuint>::const_iterator it = maps[map_key].display_lists.find(codes[i]);
		  
		if (it != maps[map_key].display_lists.end()) //znak je v mape
		{
			result.push_back((*it).second);
		}
		else //znak neni v mape
		{
			result.push_back(0);
		}
	}

	return result;
}

void Font::Draw_Text(const string & text, const Point & position) const
{
	if (text.size() == 0) return;
	vector<Uint16> codes = Get_UTF8_Codes(text);
	
	int minx;
	TTF_GlyphMetrics(font, codes[0], &minx, NULL, NULL, NULL, NULL);

	glPushMatrix();

	glTranslatef(position.x-minx, position.y, 0.0f);
	glScalef(scale, scale, 1);
	glColor4f(color.red, color.green, color.blue, 1.0f);

	//ulozit puvodni hodnoty blendfunc
	GLint dst, src;
	glGetIntegerv(GL_BLEND_DST, &dst);
	glGetIntegerv(GL_BLEND_SRC, &src);

	glBlendFunc(GL_ONE, GL_ONE); //kvuli alpha blended texturam

	vector<GLuint> lists = Characters_To_Lists(text);
	
	if (lists.size()) 
		glCallLists(GLsizei(lists.size()), GL_UNSIGNED_INT, &lists[0]);
	
	//vratit puvodni hodnoty blendfunc
	glBlendFunc(src, dst); 

	glPopMatrix();
}

void Font::Draw_Text(const string & text, const Point & position, const Color & color)
{
	Set_Color(color);
	Draw_Text(text, position);
}


GLfloat Font::Get_Font_Height() const 
{ 
	if (!font) 
	{
		return -1;
	}

    return scale * TTF_FontHeight(font);
}

GLfloat Font::Get_Text_Width(const string & text) const
{
	if (!font) return -1;
	
	int w;
	int last_advance, last_maxx, first_minx;
	
	if (TTF_SizeUTF8(font, text.c_str(), &w, NULL) == -1) return -1;

	vector<Uint16> codes = Get_UTF8_Codes(text);
	if (TTF_GlyphMetrics(font, *(codes.begin()), &first_minx, NULL, NULL, NULL, NULL) == -1) return -1;
	if (first_minx > 0) w -= first_minx;

	if (TTF_GlyphMetrics(font, *(codes.end()-1), NULL, &last_maxx, NULL, NULL, &last_advance) == -1) return -1;
	if (last_maxx != 0)
	    w -= last_advance - last_maxx;
	
	return w * scale;
}

GLfloat Font::Get_Text_Width(const string & path, int size, const string & text)
{
	TTF_Font* font;
	if(!(font = TTF_OpenFont(path.c_str(), size))) return -1;

	GLfloat scale = GLfloat(size) / TTF_FontHeight(font);

	int width;
	if (TTF_SizeUTF8(font, text.c_str(), &width, NULL) == -1) return -1;

	int last_advance, last_maxx, first_minx;
	vector<Uint16> codes = Get_UTF8_Codes(text);
	if (TTF_GlyphMetrics(font, *(codes.begin()), &first_minx, NULL, NULL, NULL, NULL) == -1) return -1;
	
	if (first_minx > 0) width -= first_minx;

	if (TTF_GlyphMetrics(font, *(codes.end()-1), NULL, &last_maxx, NULL, NULL, &last_advance) == -1) return -1;
	if (last_maxx != 0)
	    width -= last_advance - last_maxx;

	return width * scale;
}

GLfloat Font::Get_Text_Height(const string & text) const
{
	if (!font) return -1;
	int h;
	
	TTF_SizeUTF8(font, text.c_str(), NULL, &h);

	return h * scale;
}

void Font::Change_Height(int font_height)
{
	Load_Font(font_path, font_height);
}




