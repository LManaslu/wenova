#include "Rectangle.h"

Rectangle::Rectangle(){}

Rectangle::Rectangle(float ax, float ay, float w, float h) :
                     x(ax), y(ay), width(w), height(h){}

bool Rectangle::is_inside(float mx, float my){
  bool horizontal_in = (mx >= x - width/2 and mx <= x + width/2);
  bool vertical_in = (my >= y - height/2 and my <= y + height/2);

  return horizontal_in and vertical_in;
}

float Rectangle::get_x() const{
  return x;
}

float Rectangle::get_y() const{
  return y;
}

void Rectangle::set_x(float cx){
  this->x = cx;
}

void Rectangle::set_y(float cy){
  this->y = cy;
}

float Rectangle::get_draw_x() const{
  return x - width/2;
}

float Rectangle::get_draw_y() const{
  return y - height/2;
}

float Rectangle::get_width() const{
  return width;
}

float Rectangle::get_height() const{
  return height;
}

void Rectangle::set_width(float w){
  width = w;
}

void Rectangle::set_height(float h){
  height = h;
}


Vector Rectangle::get_center() const{
  return Vector(x, y);
}
