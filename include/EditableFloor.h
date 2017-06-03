#ifndef EDITABLEFLOOR_H
#define EDITABLEFLOOR_H

#include "Floor.h"
#include "Sprite.h"

class EditableFloor : public Floor{
private:
	enum FloorState{SELECTED, NOT_SELECTED};
	Sprite normal_sprite, platform_sprite, selected_sprite;
	FloorState state;
	bool deleted;
	bool selected;

public:

	EditableFloor(float x, float y, float crotation, bool cplatform);
	EditableFloor(float x, float y, float width, float crotation, bool cplatform);

	~EditableFloor();

	void update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);

	string get_information();
	void set_selected(bool cselected);
};

#endif
