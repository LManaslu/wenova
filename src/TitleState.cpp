#include "TitleState.h"

#include "InputManager.h"
#include "Game.h"
#include "MenuState.h"

#define TEXT_TIMER_COOLDOWN 50
#include "BattleState.h"
#include "EditState.h"

TitleState::TitleState(){
	text = new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "PRESS ENTER", { 23, 28, 57, 1 }, 640, 640);
	background = Sprite("title/background.png");
	show_text = true;
}

void TitleState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE) or inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(inputManager.key_press(SDLK_RETURN)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(text_timer.get() > TEXT_TIMER_COOLDOWN){
		show_text = !show_text;
		text_timer.restart();
	}

	text_timer.update(delta);
}

void TitleState::render(){
	background.render(0, 0);

	if(show_text)
		text->render(0, 0);
}

void TitleState::pause(){

}

void TitleState::resume(){

}
