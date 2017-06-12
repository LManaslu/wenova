#ifndef CHARACTER_SELECT_STATE_H
#define CHARACTER_SELECT_STATE_H

#include "State.h"
#include "Sprite.h"

#include <string>

using std::to_string;

class CharacterSelectState : public State {
private:
	Sprite background, character_slots;
    Sprite character_base[4];

public:
    CharacterSelectState();

    void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif
