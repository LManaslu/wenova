#include "Resources.h"

#include "Game.h"

#include <algorithm>

unordered_map<string, shared_ptr<SDL_Texture> > Resources::image_table;
unordered_map<string, shared_ptr<Mix_Music> > Resources::music_table;
unordered_map<string, shared_ptr<Mix_Chunk> > Resources::sound_table;
unordered_map<string, shared_ptr<TTF_Font> > Resources::font_table;

shared_ptr<SDL_Texture> Resources::get_image(string file){
	if(image_table.find(file) == image_table.end()){
		SDL_Texture * tx =
		IMG_LoadTexture(Game::get_instance().get_renderer(), file.c_str());

		if(tx == nullptr){
			printf("%s: %s\n", SDL_GetError(), file.c_str());
			exit(-1);
		}

		shared_ptr<SDL_Texture> texture(tx, [](SDL_Texture * txt) { SDL_DestroyTexture(txt); });

		image_table.emplace(file, texture);
	}

	return image_table[file];
}

void Resources::clear_images(){
	for(auto texture : image_table){
		if(texture.second.unique()){
			image_table.erase(texture.first);
		}
	}
}

shared_ptr<Mix_Music> Resources::get_music(string file){
	if(music_table.find(file) == music_table.end()){
		Mix_Music * mx = Mix_LoadMUS(file.c_str());

		if(mx == nullptr){
			printf("%s: %s\n", Mix_GetError(), file.c_str());
			exit(-1);
		}

		shared_ptr<Mix_Music> music(mx, [](Mix_Music * msc) { Mix_FreeMusic(msc); });

		music_table.emplace(file, music);
	}

	return music_table[file];

}

void Resources::clear_music(){
	for(auto music : music_table){
		if(music.second.unique()){
			music_table.erase(music.first);
		}
	}
}

shared_ptr<Mix_Chunk> Resources::get_sound(string file){
	if(sound_table.find(file) == sound_table.end()){
		Mix_Chunk * ck = Mix_LoadWAV(file.c_str());

		if(ck == nullptr){
			printf("%s: %s\n", Mix_GetError(), file.c_str());
			exit(-1);
		}

		shared_ptr<Mix_Chunk> sound(ck, [](Mix_Chunk * chk) { Mix_FreeChunk(chk); });

		sound_table.emplace(file, sound);
	}

	return sound_table[file];

}

void Resources::clear_sound(){
	for(auto sound : sound_table){
		if(sound.second.unique()){
			sound_table.erase(sound.first);
		}
	}
}

shared_ptr<TTF_Font> Resources::get_font(string file, int size){
	string tsize = std::to_string(size);
	if(font_table.find(file + tsize) == font_table.end()){
		TTF_Font * ft = TTF_OpenFont(file.c_str(), size);

		if(ft == nullptr){
			printf("%s: %s\n", SDL_GetError(), file.c_str());
			exit(-1);
		}

		shared_ptr<TTF_Font> font(ft, [](TTF_Font * fnt) { TTF_CloseFont(fnt); });

		font_table.emplace(file + tsize, font);
	}

	return font_table[file + tsize];

}

void Resources::clear_fonts(){
	for(auto font : font_table){
		if(font.second.unique()){
			font_table.erase(font.first);
		}
	}
}
