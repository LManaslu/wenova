#ifndef CHARACTER_SELECT_STATE_H
#define CHARACTER_SELECT_STATE_H

#include "State.h"
#include "Sprite.h"
#include "InputManager.h"

#include <string>

using std::to_string;

class CharacterSelectState : public State {
private:
	Sprite background, character_slots;
	Sprite name_tag[4];
	int cur_selection[4];
	vector<ii> name_tag_positions;

public:
	CharacterSelectState();

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif
