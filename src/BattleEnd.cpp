#include "BattleEnd.h"
#include "InputManager.h"

using std::to_string;

BattleEnd::BattleEnd(int id_winner){
	sprite = vector<Sprite>(N_SPRITES + 2);

	string path = "victory/" + to_string(id_winner) + "/"; 

	for(int i = 1; i <= N_SPRITES; i++){
		sprite[i] = Sprite(path + to_string(i) + ".png", 1, 10);
	}

	current_sprite = 1;
	printf("entrei\n");
}

void BattleEnd::update(float delta){
	sprite[current_sprite].update(delta);

	if(sprite[current_sprite].is_finished() && current_sprite < N_SPRITES)
		current_sprite++;
}

void BattleEnd::render(){
	sprite[current_sprite].render();
}

bool BattleEnd::is_dead(){
	return false;
}

void BattleEnd::notify_collision(GameObject &){

}
