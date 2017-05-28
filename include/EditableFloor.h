#ifndef EDITABLEFLOOR_H
#define EDITABLEFLOOR_H

#include "Floor.h"
#include "Sprite.h"

class EditableFloor : public Floor{
private:
  enum FloorState{SELECTED, NOT_SELECTED};
  Sprite sprite;
  FloorState state;

public:

  EditableFloor(float x, float y, float crotation);
  ~EditableFloor();

  void update(float delta);
  void render();
  bool is_dead();

  void notify_collision(GameObject & object);
  bool is(string type);
};

#endif
