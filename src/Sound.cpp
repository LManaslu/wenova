#include "Sound.h"

#include "Resources.h"
#include "Config.h"

Sound::Sound(){
	file = "";
	sound = nullptr;
}

Sound::Sound(string cfile){
	file = cfile;
	this->open(cfile);
}

void Sound::play(int times){
	channel = Mix_PlayChannel(-1, sound.get(), times);
	if(channel == -1){
		printf("Play chunk: %s\n", Mix_GetError());
		exit(-1);
	}
}

void Sound::stop(){
	Mix_HaltChannel(channel);
}

void Sound::open(string cfile){
	sound = Resources::get_sound(RES_FOLDER + cfile);
}

bool Sound::is_open(){
	return sound != nullptr;
}

string Sound::get_file(){
	return file;
}
