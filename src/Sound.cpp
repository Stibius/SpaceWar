
/** \file Sound.cpp
*   \brief Obsahuje definice metod tridy Sound.
*/

#include "Sound.h"

int Sound::num_instances = 0;
int Sound::num_channels = DEFAULT_NUM_CHANNELS;

int Sound::Set_Num_Channels(int num_ch) 
{ 
	num_channels = Mix_AllocateChannels(num_ch); 
	return num_channels;
}

void Sound::Free_Chunk()
{
	if (chunk != NULL)
	{
		if (channel != NO_CHANNEL && Mix_Playing(channel)) 
		{
		    Mix_HaltChannel(channel);
		}

		Mix_FreeChunk(chunk);
		chunk = NULL;
		channel = NO_CHANNEL;
		path = "";
	}
}

Sound::Sound() : chunk(NULL)
{
	num_instances++;
	channel = NO_CHANNEL;

	if (num_instances == 1)
	{
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
		Set_Num_Channels(num_channels);
	}
}

Sound::Sound(string path) : chunk(NULL), channel(NO_CHANNEL)
{
    num_instances++;

	if (num_instances == 1)
	{
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
		Set_Num_Channels(num_channels);
	}

	Load_Sound(path);
}

Sound::Sound(const Sound & sound) : path(sound.path), channel(NO_CHANNEL)
{
	num_instances++;
	chunk = Mix_LoadWAV(path.c_str());
}

Sound & Sound::operator=(const Sound & sound)
{
	if (this == &sound) 
        return *this;     

	Free_Chunk();

	path = sound.path;
	chunk = Mix_LoadWAV(path.c_str());
	channel = NO_CHANNEL;

	return *this;
}

Sound::~Sound()
{
	num_instances--;
	Free_Chunk();

	if (num_instances == 0)
	{
		Mix_CloseAudio();
	}
}

bool Sound::Load_Sound(string path)
{
	this->path = path;

	Free_Chunk();

	chunk = Mix_LoadWAV(path.c_str());

	if (chunk == NULL)
	{
		this->path = "";
		return false;
	}
	else
	{
		return true;
	}
}

bool Sound::Play(int loops)
{
	if (chunk == NULL) return false;
	
	int i;
	for (i = 0; i < num_channels; i++)
	{
		if (!Mix_Playing(i)) 
		    break;
	}

	channel = i;

	if (channel == num_channels || (channel = Mix_PlayChannel(channel, chunk, loops)) == -1)
	{
		channel = NO_CHANNEL;
		return false;
	}
	else
	{
		return true;
	}
}
	
void Sound::Pause()
{
	if (channel != NO_CHANNEL && Mix_Playing(channel) && !Mix_Paused(channel))
	{
	    Mix_Pause(channel);
	}
}

void Sound::Resume()
{
	if (channel != NO_CHANNEL && Mix_Paused(channel))
	{
		Mix_Resume(channel);
	}
}

void Sound::Stop()
{
	if (channel != NO_CHANNEL && Mix_Playing(channel))
	{
		Mix_HaltChannel(channel);
		channel = NO_CHANNEL;
	}
}

bool Sound::Is_Paused() const
{
	return (channel != NO_CHANNEL && Mix_Paused(channel));
}

bool Sound::Is_Playing() const
{
	return (channel != NO_CHANNEL && Mix_Playing(channel) && !Mix_Paused(channel));
}

