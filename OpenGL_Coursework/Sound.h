#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>


class Sound
{
public:
	Sound(const std::string& filename);
	~Sound();
	void play();
	void stop();
	void setVolume(float volume);

private:
	ALuint buffer;
	ALuint source;
	ALCdevice* device;
	ALCcontext* context;
	bool loadWAVFile(const std::string& filename);
	void initSource();
};

