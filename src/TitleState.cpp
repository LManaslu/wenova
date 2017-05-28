#include "TitleState.h"

#include "InputManager.h"
#include "Game.h"
#include "MenuState.h"

TitleState::TitleState(){
	text = new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "PRESS SPACE TO PLAY", {0, 48, 255, 255}, 500, 500);
	background = Sprite("title/background.png");
}

void TitleState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE) or inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(inputManager.key_press(SDLK_SPACE)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}
}

void TitleState::render(){
	background.render(0, 0);
	text->render(0, 0);
}

void TitleState::pause(){

}

void TitleState::resume(){

}
