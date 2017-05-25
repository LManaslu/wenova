#include "BattleState.h"

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"

BattleState::BattleState(string stage){
  background = Sprite("stage_" + stage + "/background.png");

  add_object(new Fighter("fighter", 500, 500));
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
