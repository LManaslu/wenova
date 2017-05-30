#include "Floor.h"

#include "InputManager.h"
#include "Camera.h"

#define LAYER 0
#define PI 3.14159265358979

#define HEIGHT 20

//TODO reavaliar se precisa ou não de Camera
Floor::Floor(float x, float y, float width, float crotation){
  rotation = crotation * PI / 180.0;
  box = Rectangle(x, y, width, HEIGHT);
}

Floor::~Floor(){
}

void Floor::update(float delta){
	//printf("%.f %.f %.f %.f %.f\n", box.x, box.y, box.width, box.height, rotation);
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
