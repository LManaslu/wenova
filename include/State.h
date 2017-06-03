#ifndef STATE_H
#define STATE_H

#include "GameObject.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class State{
protected:
	bool m_pop_requested;
	bool m_quit_requested;

	vector<unique_ptr<GameObject>> object_array;

	virtual void update_array(float delta);
	virtual void render_array();

public:
	State();
	virtual ~State();

	virtual void update(float delta) = 0;
	virtual void render() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void add_object(GameObject * object);

	virtual void load_assets();

	bool pop_requested();
	bool quit_requested();
};

#endif
