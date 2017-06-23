#include "Attack.h"

Attack::Attack(int p_id_player, float x, float y, float p_width, float p_height, int p_damage){
	printf("entrei\n");
	hit = false;
	id_player = p_id_player;
	width = p_width;
	height = p_height;
	box = Rectangle(x, y, width, height);
	damage = p_damage;
	team = (id_player + 1)/2;
}

Attack::~Attack(){

}

void Attack::update(float delta){

}

void Attack::render(){

}

void Attack::notify_collision(GameObject &){
}
