#include "Fighter.h"

#include "InputManager.h"
#include "Camera.h"

#define LAYER 0

//TODO reavaliar se precisa ou não de Camera
Fighter::Fighter(string name, float x, float y){
  sprite = Sprite(name + "/idle.png", 6, 1);
  rotation = 0;
  box = Rectangle(x, y, sprite.get_width(), sprite.get_height());
}

Fighter::~Fighter(){
}

void Fighter::update(float delta){
  InputManager inputManager = InputManager::get_instance();

  if(inputManager.is_key_down(SDLK_a)){
    //esquerda
  }
  if(inputManager.is_key_down(SDLK_d)){
    //direita
  }


  sprite.update(delta);

  // speed.transform(linear_speed, rotation);
  //
  // float x = box.get_x() + speed.x * delta;
  // if(x < 0) x = 0;
  // if(x > 1407) x = 1408;
  //
  // float y = box.get_y() + speed.y * delta;
  // if(y < 0) y = 0;
  // if(y > 1280) y = 1280;
  //
  // box.set_x(x);
  // box.set_y(y);
}

void Fighter::render(){
  int x = box.get_draw_x()  + Camera::pos[LAYER].x;
  int y = box.get_draw_y() + Camera::pos[LAYER].y;
  sprite.render(x, y, rotation);
}

bool Fighter::is_dead(){
 return false;
}

void Fighter::notify_collision(GameObject &){
}

bool Fighter::is(string type){
  return type == "fighter";
}
