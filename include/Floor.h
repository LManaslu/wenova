#ifndef FLOOR_H
#define FLOOR_H

#include "GameObject.h"
#include "Sprite.h"

class Floor : public GameObject{
public:
	Floor(string name, float x, float y, float crotation);
	~Floor();

  Floor(float x, float y, float width, float crotation);
  ~Floor();

  void update(float delta);
  void render();
  bool is_dead();

	void notify_collision(GameObject & object);
	bool is(string type);
};

#endif
