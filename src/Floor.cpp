#include "Floor.h"

#include "InputManager.h"
#include "Camera.h"

#define LAYER 0
#define PI 3.14159265358979

#define HEIGHT 20

int Floor::floor_id = 1;

//TODO reavaliar se precisa ou não de Camera
Floor::Floor(float x, float y, float width, float crotation, bool cplatform){
	is_platform = cplatform;
	rotation = crotation * PI / 180.0;
	box = Rectangle(x, y, width, HEIGHT);
	id = floor_id++;
	tags["floor"] = true;
	if(is_platform) tags["platform"] = true;
}

Floor::~Floor(){
}

void Floor::update(float){
	//printf("%.f %.f %.f %.f %.f\n", box.x, box.y, box.width, box.height, rotation);
}

void Floor::render(){
}

bool Floor::is_dead(){
	return false;
}

void Floor::notify_collision(GameObject &){
}

int Floor::get_id(){
	return id;
}
