
/** \file Sound.h
*   \brief Obsahuje deklaraci tridy Sound.
*/

#ifndef SOUND_H_
#define SOUND_H_

#include <SDL_mixer.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

/** \class Sound
*   \brief Trida reprezentujici zvuk.
*/
class Sound
{
private:

	enum 
	{ 
		NO_CHANNEL = -2, //!<urcuje, ze tomuto zvuku neni prirazen zadny kanal
		DEFAULT_NUM_CHANNELS = 16 //!<defaultni pocet kanalu
	}; 

	Mix_Chunk* chunk; //!<audio data, NULL pokud neni nahrany zvuk
	string path; //!<cesta k souboru s nahranym zvukem, prazdny retezec, pokud neni nahrany zvuk
	int channel; //!<kanal, ve kterem se prave prehrava tento zvuk, NO_CHANNEL, pokud se neprehrava
	static int num_instances; //!<pocet instanci teto tridy
	static int num_channels;  //!<pocet alokovanych kanalu pro prehravani 

	//! \brief Uvolni tento zvuk.
	void Free_Chunk();

public:

	//! \brief Inkrementuje pocet instanci, pokud jde o prvni instanci, inicializuje SDL_ttf a alokuje kanaly.
	Sound();

	//! \brief Inkrementuje pocet instanci, pokud jde o prvni instanci, inicializuje SDL_ttf a alokuje kanaly. Nahraje zvuk ze souboru daneho parametrem.
	Sound(string path);

	//! \brief Inkrementuje pocet instanci, zkopiruje zvuk.
	Sound(const Sound & sound);

	//! \brief Zkopiruje zvuk.
	Sound & operator=(const Sound & sound);

	//! \brief Dekrementuje pocet instanci, uvolni zvuk, pokud je to posledni instance, deinicializuje SDL_ttf.
	~Sound();

	/** \brief Nahraje zvuk ze souboru.
	*   \param path Cesta k souboru se zvukem.
	*   \return Zda se podarilo zvuk nahrat.
	*/
	bool Load_Sound(string path);

	/** \brief Prehraje zvuk.
	*   \param loops Pocet opakovani (0 = prehraje se jen jednou, -1 = nekonecna smycka)
	*/
	bool Play(int loops);

	/* \brief Alokuje dany pocet kanalu.
	*  \return Pocet kanalu, ktere se podarilo alokovat.
	*/
	static int Set_Num_Channels(int num_ch);

	//! \brief Zjisti, jestli je zvuk zapauzovany.
	bool Is_Paused() const;

	//! \brief Zjisti, jestli je zvuk prave prehravan.
	bool Is_Playing() const;
	
	//! \brief Zapauzuje zvuk, pokud je prave prehravany.
	void Pause();

	//! \brief Obnovi prehravani zapauzovaneho zvuku, pokud je zapauzovany.
	void Resume();

	//! \brief Ukonci prehravani zvuku, pokud je prave prehravany nebo zapauzovany.
	void Stop();		
};

#endif