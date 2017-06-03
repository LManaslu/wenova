#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "SDL2/SDL.h"

#include "Vector.h"

class Rectangle{
public:

	float x;
	float y;
	float width;
	float height;

	Rectangle();
	Rectangle(float x, float y, float width, float height);
	bool is_inside(float mx, float my);
	float get_x() const;
	float get_y() const;
	void set_x(float cx);
	void set_y(float cy);
	float get_draw_x() const;
	float get_draw_y() const;
	float get_width() const;
	float get_height() const;
	void set_width(float w);
	void set_height(float h);
	Vector get_center() const;
};

#endif
