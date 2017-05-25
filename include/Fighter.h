#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"

class Fighter : public GameObject{
private:

  Sprite sprite;


public:

  Fighter(string name, float x, float y);
  ~Fighter();

  void update(float delta);
  void render();
  bool is_dead();

  void notify_collision(GameObject & object);
  bool is(string type);
};

#endif
