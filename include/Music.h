#ifndef MUSIC_H
#define MUSIC_H

#include "SDL_mixer.h"

#include <memory>
#include <string>

using std::string;
using std::shared_ptr;

class Music{
private:
	shared_ptr<Mix_Music> music;

public:
	Music();
	Music(string file);

	void play(int times = -1);
	void stop();
	void open(string file);
	bool is_open();

};

#endif
