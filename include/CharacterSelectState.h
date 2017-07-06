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
	Sprite number[4], selected_tag, ready_to_fight;
	int cur_selection_row[4], cur_selection_col[4];
	string selected_stage;
	Sound blocked, selected_sound, changed;

	enum Button { A, B, Y, LEFT, RIGHT, UP, DOWN, SELECT, START, LT, RT };
	bool pressed[4][15];

	int cur_skin[4];
	bool selected[4], ready;

	vector<ii> number_delta, sprite_pos;
	vector<int> col_slots, row_slots;

	FighterMenu chars[N_CHARS];

	Sprite name_tag[4];
	Sprite char_name[8];
	// Sprite disabled[8];
	Sprite char_sprite[8][4];
	vector<ii> name_tag_positions, name_delta;
	vector<string> names;

	bool available_skin[8][4];

public:
	CharacterSelectState(string cselected_stage);

	void update(float delta);
	void render();

	void pause();
	void resume();

	bool character_enabled(int row, int col);
	pair<string, int> get_char_info(int idx);
	bool all_players_selected();
	string get_skin_name(int idx);
	vector< pair<string, string> > export_players();

	void process_input();
};

#endif
