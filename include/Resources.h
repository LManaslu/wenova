#ifndef RESOURCES_H
#define RESOURCES_H

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <unordered_map>
#include <memory>

using std::unordered_map;
using std::string;
using std::shared_ptr;

class Resources{
private:
	static unordered_map<string, shared_ptr<SDL_Texture> > image_table;
	static unordered_map<string, shared_ptr<Mix_Music> > music_table;
	static unordered_map<string, shared_ptr<Mix_Chunk> > sound_table;
	static unordered_map<string, shared_ptr<TTF_Font> > font_table;

public:
	static shared_ptr<SDL_Texture> get_image(string file);
	static void clear_images();

	static shared_ptr<Mix_Music> get_music(string file);
	static void clear_music();

	static shared_ptr<Mix_Chunk> get_sound(string file);
	static void clear_sound();

	static shared_ptr<TTF_Font> get_font(string file, int size);
	static void clear_fonts();
};

#endif
