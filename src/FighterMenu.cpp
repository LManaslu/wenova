#include "FighterMenu.h"

#define FRAME_TIME 13
#define WHITE { 255, 255, 255, 255 }

FighterMenu::FighterMenu(){
}

FighterMenu::FighterMenu(string cname, int frames, bool cenabled){
	name = cname;
	name_text = new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, name, WHITE);
	enabled = cenabled;
	n_frames = frames;

	for(int i=0;i<N_SKINS;i++){
		skin[i] = Sprite(name + "/" + get_skin_name(i) + "/idle.png", n_frames, FRAME_TIME);
		skin[i].set_scale(SKIN_SCALE);
	}

	disabled = Sprite(name + "/disabled.png", n_frames, FRAME_TIME);
	disabled.set_scale(SKIN_SCALE);

	memset(skin_available, true, sizeof skin_available);
	for(auto &letter : name) letter = toupper(letter);
}

bool FighterMenu::is_enabled(){
	return enabled;
}

bool FighterMenu::is_skin_available(int idx){
	return skin_available[idx];
}

void FighterMenu::lock_skin(int idx){
	skin_available[idx] = false;
}

void FighterMenu::unlock_skin(int idx){
	skin_available[idx] = true;
}

Text* FighterMenu::get_name_text(){
	return name_text;
}

string FighterMenu::get_name(){
	return name;
}

string FighterMenu::get_skin_name(int idx){
	vector<string> skins_names = { "default", "alt1", "alt2", "alt3" };
	return skins_names[idx];
}

Sprite& FighterMenu::get_skin(int idx){
	return skin[idx];
}

Sprite& FighterMenu::get_disabled(){
	return disabled;
}
