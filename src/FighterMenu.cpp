#include "FighterMenu.h"

#define FRAME_TIME 13
#define YELLOW { 202, 236, 4, 1 }

FighterMenu::FighterMenu(){
}

FighterMenu::FighterMenu(string cname, int frames, bool cenabled){
	name = cname;
	name_text = new Text("font/8-BIT WONDER.ttf", 20, Text::TextStyle::SOLID, name, YELLOW);
	enabled = cenabled;
	n_frames = frames;

	for(int i=0;i<N_SKINS;i++){
		if(enabled)
			skin[i] = Sprite("characters/" + name + "/" + get_skin_name(i) + "/idle.png", n_frames, FRAME_TIME);
		else
			skin[i] = Sprite("characters/" + name + "/idle.png", n_frames, FRAME_TIME);

		skin[i].set_scale(SKIN_SCALE);
	}

	disabled = Sprite("characters/" + name + "/disabled.png", n_frames, FRAME_TIME);
	disabled.set_scale(SKIN_SCALE);

	memset(skin_available, true, sizeof skin_available);
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
