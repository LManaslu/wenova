#include "Text.h"

#include "Game.h"
#include "Resources.h"

#define SOLID Text::TextStyle::SOLID
#define SHADED Text::TextStyle::SHADED
#define BLENDED Text::TextStyle::BLENDED

Text::Text(){
	texture = nullptr;
}

Text::Text(string cfont_file, int cfont_size, TextStyle cstyle, string ctext, SDL_Color ccolor, int x, int y){
	font_size = cfont_size;
	style = cstyle;
	text = ctext;
	color = ccolor;
	box.set_x(x);
	box.set_y(y);
	texture = nullptr;
	open(cfont_file, font_size);
	remake_texture();
	set_pos(x, y, true, true);
}

Text::~Text(){
	if(texture != nullptr){
		SDL_DestroyTexture(texture);
	}
}

void Text::render(int camera_x, int camera_y){
	SDL_Rect dstrect = SDL_Rect{(int) box.get_x() + camera_x, (int) box.get_y() + camera_y, clip_rect.w, clip_rect.h};

	int render_copy = SDL_RenderCopy(Game::get_instance().get_renderer(), texture,
	&clip_rect, &dstrect);
	if(render_copy){
		printf("Render text: %s\n", SDL_GetError());
		exit(-1);
	}
}

void Text::set_pos(int x, int y, bool center_x, bool center_y){
	box.set_x(x - (center_x ? clip_rect.w * 0.5 : 0));
	box.set_y(y - (center_y ? clip_rect.h * 0.5 : 0));
}


void Text::set_text(string ctext){
	text = ctext;
	remake_texture();
}

void Text::set_color(SDL_Color ccolor){
	color = ccolor;
	remake_texture();
}

void Text::set_style(TextStyle cstyle){
	style = cstyle;
	remake_texture();
}

void Text::set_font_size(int cfont_size){
	font_size = cfont_size;
	remake_texture();
}

void Text::remake_texture(){
	if(texture != nullptr){
		SDL_DestroyTexture(texture);
	}

	SDL_Surface * surface;
	switch(style){
		case SOLID:

		surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);

		break;
		case SHADED:


		surface = TTF_RenderText_Shaded(font.get(), text.c_str(), color, {0, 0, 0, 255});

		break;
		case BLENDED:

		surface = TTF_RenderText_Blended(font.get(), text.c_str(), color);
		break;
	}

	if(surface == nullptr){
		printf("%s\n", SDL_GetError());
		exit(-3);
	}

	texture = SDL_CreateTextureFromSurface(Game::get_instance().get_renderer(), surface);
	SDL_FreeSurface(surface);

	int w, h;
	int query_texture = SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	if(query_texture){
		printf("Remake texture: %s\n", SDL_GetError());
		exit(-1);
	}

	box.set_width(w);
	box.set_height(h);
	clip_rect.w = w;
	clip_rect.h = h;
}

void Text::open(string file, int size){
	font = Resources::get_font("res/" + file, size);
}
