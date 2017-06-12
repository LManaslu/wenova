#include "CharacterSelectState.h"
#include "InputManager.h"

CharacterSelectState::CharacterSelectState(){
    background = Sprite("character_select/background.png");
    character_slots = Sprite("character_select/character_slots.png");

    for(int i=0;i<4;i++){
        character_base[i] = Sprite("character_select/charselect_base_" + to_string(i+1) + ".png");
    }

}

void CharacterSelectState::update(float delta){
    InputManager * input_manager = InputManager::get_instance();

    if(input_manager->quit_requested() || input_manager->key_press(SDLK_ESCAPE)){
		m_quit_requested = true;
		return;
	}

}

void CharacterSelectState::render(){
    background.render(0, 0);
    character_slots.render(0, 0);

    ii base_positions[] { ii(28, 314), ii(221, 638), ii(994, 314), ii(801, 638) };

    for(int i=0; i<4; i++){
        character_base[i].render(base_positions[i].first, base_positions[i].second);
    }

}

void CharacterSelectState::pause(){

}

void CharacterSelectState::resume(){

}
