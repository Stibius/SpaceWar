
#include "Texture.h"

map<int, int> & Texture::Get_Num_Instances()
{
	static map<int, int>* ans = new std::map<int, int>;
    return *ans;
}

Texture::Texture() 
{ 
	glGenTextures(1, &id); 
	static map<int, int> & num_instances = Texture::Get_Num_Instances(); 
	num_instances[id] = 1; 
};

Texture::Texture(GLsizei width, GLsizei height, GLenum type, const GLvoid *pixels, GLint wrap_s, GLint wrap_t) 
	: width(width), height(height), type(type)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //filtr pouzity pri zmensovani textry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //filtr pouzity pri zvetsovani textry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);  //textura se neopakuje, kvuli artefaktum
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);  //textura se neopakuje, kvuli artefaktum
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, type, pixels); //nacteni textury do GPU

	static map<int, int> & num_instances = Texture::Get_Num_Instances();
	num_instances[id] = 1;
}

Texture::Texture(const Texture & texture)
{
	id = texture.id;
	static map<int, int> & num_instances = Texture::Get_Num_Instances();
	num_instances[id]++;
	width = texture.width;
	height = texture.height;
	type = texture.type;
}

Texture::~Texture()
{
	static map<int, int> & num_instances = Texture::Get_Num_Instances();
	num_instances[id]--;
	if (num_instances[id] == 0)
	{
		glDeleteTextures(1, &id);
		num_instances.erase(id);	
	}
}

Texture & Texture::operator=(const Texture & texture)
{
	if (this == &texture) 
        return *this;    

	static map<int, int> & num_instances = Texture::Get_Num_Instances();

	num_instances[id]--;
	if (num_instances[id] == 0)
	{
		glDeleteTextures(1, &id);
		num_instances.erase(id);
	}
	
	id = texture.id;
	num_instances[id]++;
	width = texture.width;
	height = texture.height;
	type = texture.type;

	return *this;
}

void Texture::Draw(const Point & position) const
{
	glEnable(GL_TEXTURE_2D);     
	glBindTexture(GL_TEXTURE_2D, id);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2f(position.x, position.y);

		glTexCoord2f(1.0f, 0.0f); 
		glVertex2f(position.x + width, position.y);

		glTexCoord2f(1.0f, 1.0f); 
		glVertex2f(position.x + width, position.y + height);

		glTexCoord2f(0.0f, 1.0f); 
		glVertex2f(position.x, position.y + height);
	glEnd();
	
	glFinish(); 

	glDisable(GL_TEXTURE_2D);
}