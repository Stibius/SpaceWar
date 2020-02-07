
/** \file Events.h
*   \brief Obsahuje enumeraci definujici symbolicke konstanty pro kody SDL udalosti pouzivanych programem.  
*/

#ifndef EVENTS_H_
#define EVENTS_H_

//! \breif Identifikatory uzivatelskych SDL udalosti pouzivanych v programu.
enum events 
{ 
	USR_EVT_START_GAME, //!<ma se spustit hra z menu
	USR_EVT_RETURN_TO_GAME, //!<navrat do hry z menu
	USR_EVT_QUIT_GAME, //!<ukonceni rozehrane hry
	USR_EVT_QUIT_PROGRAM, //!<ukonceni programu
	USR_EVT_SETTINGS_CHANGED, //!<doslo ke zmene hernich nastaveni
	USR_EVT_VIDEO_SETTINGS_CHANGED, //!<doslo ke zmene nastaveni obrazovky
	USR_EVT_FIRE, //!<byl vytvoren novy projektil
	NO_EVENT //!<zadna udalost
}; 

#endif;
