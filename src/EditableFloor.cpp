#include "EditableFloor.h"

#include "InputManager.h"

#define LAYER 0
#define PI 3.14159265358979

EditableFloor::EditableFloor(float x, float y, float crotation) : sprite(Sprite("images/editable_floor.png")),
Floor(x, y, sprite.get_width(), sprite.get_height(), crotation){
  box = Rectangle(x, y, sprite.get_width(), sprite.get_height());
}

EditableFloor::~EditableFloor(){
}

void EditableFloor::update(float delta){
  sprite.update(delta);
}

void EditableFloor::render(){
  sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

bool EditableFloor::is_dead(){
 return false;
}

void EditableFloor::notify_collision(GameObject &){
}

bool EditableFloor::is(string type){
  return type == "floor";
}
