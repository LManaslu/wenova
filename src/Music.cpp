#include "Music.h"

#include "Resources.h"

Music::Music(){
	music = nullptr;
}

Music::Music(string file){
	this->open(file);
}

void Music::play(int times){
	int mix_play_music = Mix_PlayMusic(music.get(), times);
	if(mix_play_music){
		printf("Play music: %s\n", Mix_GetError());
		exit(-1);
	}
}

void Music::stop(){
	int mix_fade_out_music = Mix_FadeOutMusic(100);
	if(mix_fade_out_music){
		printf("Stop music: %s\n", Mix_GetError());
		exit(-1);
	}
}

void Music::open(string file){
	music = Resources::get_music("res/" + file);
}

bool Music::is_open(){
	return music != nullptr;
}
