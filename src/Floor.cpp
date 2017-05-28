#include "Floor.h"

#include "InputManager.h"
#include "Camera.h"

#define LAYER 0
#define PI 3.14159265358979


//TODO reavaliar se precisa ou não de Camera
Floor::Floor(float x, float y, float height, float width, float crotation){
  rotation = crotation * PI / 180.0;
  box = Rectangle(x, y, height, width);
}

Floor::~Floor(){
}

void Floor::update(float delta){
}

void Floor::render(){
}

bool Floor::is_dead(){
	return false;
}

void Floor::notify_collision(GameObject &){
}

bool Floor::is(string type){
	return type == "floor";
}
