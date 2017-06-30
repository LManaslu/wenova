#include <iostream>

#include "Sprite.h"
#include "Game.h"
#include "Resources.h"
#include "Config.h"

#include <cmath>

#define PI 3.14159265358979

Sprite::Sprite(){
	texture = nullptr;
	scale_x = scale_y = 1;
	frame_count = 1;
	frame_time = 1;
	current_frame = time_elapsed = 0;
}

Sprite::Sprite(string file, int cframe_count, float cframe_time, int ccolumns, int cur_frame){
	frame_count = cframe_count;
	frame_time = cframe_time;
	current_frame = cur_frame;
	columns = (ccolumns ? ccolumns : frame_count);
	rows = ceil((1.0 * frame_count) / columns);
	time_elapsed = 0;
	texture = nullptr;
	finished = false;
	open(RES_FOLDER + file);

	scale_x = scale_y = 1;
}

Sprite::~Sprite(){
}

int Sprite::get_width(){
	return width * scale_x;
}

int Sprite::get_height(){
	return height * scale_y;
}

bool Sprite::is_open(){
	return texture != nullptr;
}

void Sprite::open(string file){

	texture = Resources::get_image(file);

	int query_texture = SDL_QueryTexture(texture.get(), nullptr, nullptr,
	&width, &height);

	width /= columns;
	height /= rows;
	if(query_texture){
		printf("Open: %s\n", SDL_GetError());
		exit(-1);
	}

	set_clip((current_frame % columns) * width, (current_frame / columns) * height, width, height);
}

void Sprite::set_clip(int x, int y, int w, int h){
	clip_rect = SDL_Rect{x, y, w, h};
}

void Sprite::set_frame(int frame){
	current_frame = frame;
	set_clip((current_frame % columns) * width, (current_frame / columns) * height, width, height);
}

void Sprite::set_frame_count(int cframe_count){
	frame_count = cframe_count;
}

void Sprite::set_frame_time(float cframe_time){
	frame_time = cframe_time;
}

void Sprite::update(float delta){
	time_elapsed += delta;
	if(time_elapsed >= frame_time){
		time_elapsed = 0;

		current_frame = current_frame + 1;
		if(current_frame == frame_count){
			finished = true;
			current_frame = 0;
		}
		set_clip((current_frame % columns) * width, (current_frame / columns) * height, width, height);
	}
}

void Sprite::render(int x, int y, float angle, SDL_RendererFlip flip){
	SDL_Rect dstrect = SDL_Rect{x, y, (int)(clip_rect.w * scale_x), (int)(clip_rect.h * scale_y)};

	angle *= (180 / PI);
	int render_copy = SDL_RenderCopyEx(Game::get_instance().get_renderer(), texture.get(),
	&clip_rect, &dstrect, angle, nullptr, flip);
	if(render_copy){
		printf("Render: %s\n", SDL_GetError());
		exit(-1);
	}
}

void Sprite::set_scale_x(float scale){
	scale_x = scale;
}

void Sprite::set_scale_y(float scale){
	scale_y = scale;
}

void Sprite::set_scale(float scale){
	scale_x = scale_y = scale;
}

void Sprite::set_scale(float cscale_x, float cscale_y){
	scale_x = cscale_x;
	scale_y = cscale_y;
}

void Sprite::update_scale_x(float scale){
	scale_x += scale;
	if(scale_x < 0.05) scale_x = 0.05;
}

void Sprite::restart_count(int cframe){
	current_frame = cframe;
	finished = false;
}

bool Sprite::is_finished(){
	return finished;
}

int Sprite::get_current_frame(){
	return current_frame;
}
