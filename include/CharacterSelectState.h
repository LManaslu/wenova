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
	Sprite name_tag[4], number[4];
	int cur_selection_row[4], cur_selection_col[4];
	vector<ii> name_tag_positions, number_delta, name_delta, sprite_pos;
	vector<int> col_slots, row_slots;
	vector< vector<string> > names;

	map<string, Sprite> char_name;
	unordered_map<string, vector<Sprite> > char_sprite;

	int cur_skin[4];
	map<string, vector<bool> > available_skin;
	bool selected[4];

public:
	CharacterSelectState();

	void update(float delta);
	void render();

	void pause();
	void resume();

	bool character_enabled(int row, int col);
	pair<string, int> get_char_info(int idx);
	bool all_players_selected();
};

#endif
