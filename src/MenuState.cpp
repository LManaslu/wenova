#include "MenuState.h"
#include "InputManager.h"
#include "TitleState.h"
#include "BattleState.h"
#include "Game.h"

MenuState::MenuState(){
	background = Sprite("menu/background.png");
}

void MenuState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE) or inputManager.quit_requested()){
		m_quit_requested = true;
		Game::get_instance().push(new TitleState());
		return;
	}

	if(inputManager.key_press(SDLK_SPACE)){
		m_quit_requested = true;
		Game::get_instance().push(new BattleState("1"));
		return;
	}
}

void MenuState::render(){
	background.render(0, 0);
}

void MenuState::pause(){

}

void MenuState::resume(){

}
