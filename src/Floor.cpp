#include "Floor.h"

#include "InputManager.h"
#include "Camera.h"

#define LAYER 0

#define IDLE Floor::FloorState::IDLE
#define LEFT Floor::FloorState::LEFT
#define RIGHT Floor::FloorState::RIGHT
#define JUMPING Floor::FloorState::JUMPING
#define FALLING Floor::FloorState::FALLING
#define SLIDING Floor::FloorState::SLIDING


//TODO reavaliar se precisa ou não de Camera
Floor::Floor(string path, float x, float y){
  sprite = Sprite(path);

  box = Rectangle(x, y, sprite.get_width(), 20);
}

Floor::~Floor(){
}

void Floor::update(float delta){
  sprite.update(delta);
}

void Floor::render(){
  int x = box.get_draw_x();
  int y = box.get_draw_y();
  sprite.render(x, y, rotation);
}

bool Floor::is_dead(){
 return false;
}

void Floor::notify_collision(GameObject &){
}

bool Floor::is(string type){
  return type == "floor";
}
