#ifndef CHARACTER_SELECT_STATE_H
#define CHARACTER_SELECT_STATE_H

#include "State.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Sound.h"
#include "FighterMenu.h"

#include <string>

using std::to_string;
using std::make_pair;

#define N_CHARS 8

class CharacterSelectState : public State {
private:
	Sprite background[2], planet, character_slots;
	Sprite number[4], name_tag[4], selected_tag, ready_to_fight;
	Sound blocked, selected_sound, changed;
	int cur_row[4], cur_col[4], cur_skin[4];
	bool selected[4], ready;
	string selected_stage;

	enum Button { A, B, Y, LEFT, RIGHT, UP, DOWN, SELECT, START, LT, RT };
	bool pressed[4][15];

	vector<ii> number_delta, sprite_pos, name_tag_positions, name_delta;

	FighterMenu chars[N_CHARS];

public:
	CharacterSelectState(string cselected_stage);

	void update(float delta);
	void render();

	void pause();
	void resume();
	void process_input();

	bool character_enabled(int row, int col);
	pair<string, int> get_char_info(int idx);
	bool all_players_selected();
	vector< pair<string, string> > export_players();

	pair<int, int> get_slot(int row, int col);
};

#endif
