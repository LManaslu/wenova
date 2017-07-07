#ifndef SOUND_H
#define SOUND_H

#include "SDL_mixer.h"

#include <memory>
#include <string>

using std::string;
using std::shared_ptr;

class Sound{
private:
	shared_ptr<Mix_Chunk> sound;
	int channel;
	string file;

public:
	Sound();
	Sound(string cfile);

	void play(int times = 0); // plays 1 time with times = 0
	void stop();
	void open(string cfile);
	bool is_open();
	string get_file();

};

#endif
