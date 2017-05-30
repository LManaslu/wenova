#include "BattleState.h"

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "Floor.h"

BattleState::BattleState(string stage){
  background = Sprite("stage_" + stage + "/background.png", 6, 1);

	add_object(new Floor("stage_" + stage + "/floor.png", 0, 700, 45));
	add_object(new Floor("stage_" + stage + "/floor.png", 500, 700, 0));
	add_object(new Floor("stage_" + stage + "/floor.png", 1000, 700, 0));
	add_object(new Floor("stage_" + stage + "/floor.png", 800, 500, 30));
	add_object(new Floor("stage_" + stage + "/floor.png", 800, 500, 120));
	add_object(new Floor("stage_" + stage + "/floor.png", 300, 400, 0));

  add_object(new Floor(0, 700, 500, 20, 45));
  add_object(new Floor(500, 700, 500, 20, 0));
  add_object(new Floor(1000, 700, 500, 20, 0));
  add_object(new Floor(800, 500, 500, 20, 30));
  add_object(new Floor(800, 500, 500, 20, 120));
  add_object(new Floor(300, 400, 500, 20, 0));

  add_object(new Fighter("fighter", 100, 100));
}

void BattleState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE) or inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	update_array(delta);
}

void BattleState::render(){
	background.render(0, 0);

	render_array();
}

void BattleState::pause(){

}

void BattleState::resume(){

}
