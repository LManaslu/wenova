#include "CharacterSelectState.h"
#include "Game.h"
#include "MenuState.h"
#include "BattleState.h"

CharacterSelectState::CharacterSelectState(){
    background = Sprite("character_select/background.png");
    character_slots = Sprite("character_select/character_slots.png");
    disabled_selector = Sprite("character_select/disabled_selector.png");

    for(int i=0;i<4;i++){
        character_base[i] = Sprite("character_select/charselect_base_" + to_string(i+1) + ".png");
        character_selector[i] = Sprite("character_select/character_selector_" + to_string(i+1) + ".png");
    }

    memset(cur_selection, 0, sizeof cur_selection);

    base_positions = { ii(28, 314), ii(221, 638), ii(994, 314), ii(801, 638) };
    box_positions = { ii(510, 55), ii(645, 55), ii(510, 217), ii(645, 217) };
}

void CharacterSelectState::update(float delta){
    InputManager * input_manager = InputManager::get_instance();

    // inputs
    if(input_manager->quit_requested() || input_manager->key_press(SDLK_ESCAPE) || input_manager->joystick_button_press(InputManager::B, 0)){
        m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

    if(input_manager->key_press(SDLK_RETURN) || input_manager->joystick_button_press(InputManager::START, 0)){
        m_quit_requested = true;
        Game::get_instance().push(new BattleState("1", "swamp_song.ogg"));
		return;
	}

    for(int i=0;i<4;i++){
        // FIXME magic numbers
        if((input_manager->key_press(SDLK_LEFT) || input_manager->joystick_button_press(InputManager::LEFT, i)) && cur_selection[i] != 0){
            cur_selection[i]--;
        }

        // FIXME magic numbers
        if((input_manager->key_press(SDLK_RIGHT) || input_manager->joystick_button_press(InputManager::RIGHT, i)) && cur_selection[i] != 1){
            cur_selection[i]++;
        }
    }

}

void CharacterSelectState::render(){
    background.render(0, 0);
    character_slots.render(0, 0);

    for(int i=0;i<4;i++){
        character_base[i].render(base_positions[i].first, base_positions[i].second);
    }

    for(int i=0; i<2; i++){
        character_selector[i].render(box_positions[cur_selection[i]].first, box_positions[cur_selection[i]].second);
    }

    for(int i=2; i<4; i++){
        character_selector[i].render(box_positions[cur_selection[i]].first, box_positions[cur_selection[i]].second);
        disabled_selector.render(box_positions[i].first, box_positions[i].second);
    }
}

void CharacterSelectState::pause(){

}

void CharacterSelectState::resume(){

}
