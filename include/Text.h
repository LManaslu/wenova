#ifndef TEXT_H
#define TEXT_H

#include "SDL_ttf.h"

#include "Rectangle.h"

#include <memory>
#include <string>

using std::string;
using std::shared_ptr;

class Text{
public:
	enum TextStyle {SOLID, SHADED, BLENDED};

private:
	shared_ptr<TTF_Font> font;
	SDL_Texture * texture;

	string text;
	TextStyle style;
	int font_size;
	SDL_Color color;
	Rectangle box;
	SDL_Rect clip_rect;

public:
	Text();
	Text(string cfont_file, int cfont_size, TextStyle cstyle, string ctext, SDL_Color ccolor, int x = 0, int y = 0);
	~Text();

	void render(int camera_x = 0, int camera_y = 0);

	void set_pos(int x, int y, bool center_x = false, bool center_y = false);

	void set_text(string ctext);
	void set_color(SDL_Color ccolor);
	void set_style(TextStyle cstyle);
	void set_font_size(int cfont_size);

	float get_x();
	float get_y();
	float get_width();
	float get_height();
	string get_text();

	void remake_texture();
	void open(string font_name, int cfont_size);
};

#endif
