#ifndef SPRITE_H
#define SPRITE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <memory>

using std::string;
using std::shared_ptr;

class Sprite{
private:
	shared_ptr<SDL_Texture> texture = nullptr;
	int width;
	int height;
	int frame_count;
	int current_frame;
	float time_elapsed;
	float frame_time;
	SDL_Rect clip_rect;
	float scale_x;
	float scale_y;

public:
	Sprite();
	Sprite(string file, int cframe_count = 1, float cframe_time = 1, int cur_frame = 0);
	~Sprite();

	int get_width();
	int get_height();
	bool is_open();

	void open(string file);
	void set_clip(int x, int y, int w, int h);
	void set_frame(int frame);
	void set_frame_count(int cframe_count);
	void set_frame_time(float cframe_time);

	void update(float delta);
	void render(int x, int y, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

  	void set_scale_x(float scale);
  	void set_scale_y(float scale);
  	void update_scale_x(float scale);
	void restart_count();
};

#endif
