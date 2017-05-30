#ifndef EDITABLEFLOOR_H
#define EDITABLEFLOOR_H

#include "Floor.h"
#include "Sprite.h"

class EditableFloor : public Floor{
private:
	enum FloorState{SELECTED, NOT_SELECTED};
	Sprite normal_sprite, selected_sprite;
	FloorState state;
	bool deleted;
	bool selected;

public:

	EditableFloor(float x, float y, float crotation);
	EditableFloor(float x, float y, float width, float height, float crotation);

	~EditableFloor();

	void update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);
	bool is(string type);
};

#endif
