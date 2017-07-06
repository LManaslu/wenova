#ifndef FIGHTERMENU_H
#define FIGHTERMENU_H

#include "Sprite.h"
#include "Text.h"

#include <utility>
#include <string>
#include <vector>

using std::string;
using std::vector;

#define N_SKINS 4
#define SKIN_SCALE 3

class FighterMenu {
private:
	Sprite skin[N_SKINS];
	Sprite disabled;
	Text* name_text;
	string name;
	int n_frames;
	bool enabled;
	bool skin_available[N_SKINS];

public:
	FighterMenu();
	FighterMenu(string name, int frames, bool enabled = true);

	bool is_enabled();
	bool is_skin_available(int idx);
	void lock_skin(int idx);
	void unlock_skin(int idx);
	Text* get_name_text();
	string get_name();
	string get_skin_name(int idx);
	Sprite& get_skin(int idx);
	Sprite& get_disabled();
};

#endif
