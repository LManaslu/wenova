#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "SDL2/SDL.h"

#include <unordered_map>
#include <map>
#include <utility>
#include <algorithm>

#define ii pair<int, int>

using std::unordered_map;
using std::map;
using std::pair;
using std::min;
using std::max;

class InputManager{
private:
	static InputManager * input_manager;

	bool mouse_state[6];
	int mouse_update[6];

	unordered_map<int, bool> key_state;
	unordered_map<int, int> key_update;
	map<ii, int> event_responded;

	bool m_quit_requested;
	int update_counter;

	int mouse_x;
	int mouse_y;

	bool can_respond(int key, int operation, bool response);
	int offset_x;
	int offset_y;
	float scale;

public:
	InputManager();
	~InputManager();

	void update();

	bool key_press(int key, bool response = false);
	bool key_release(int key, bool response = false);
	bool is_key_down(int key, bool response = false);

	bool mouse_press(int button);
	bool mouse_release(int button);
	bool is_mouse_down(int button);

	int get_mouse_x();
	int get_mouse_y();

	bool quit_requested();

	static InputManager * get_instance();

	void set_mouse_scale(float scale, int offset_x, int offset_y);

	// FIXME not being used
	static const int LEFT_ARROW_KEY = SDLK_LEFT;
	static const int RIGHT_ARROW_KEY = SDLK_RIGHT;
	static const int DOWN_ARROW_KEY = SDLK_DOWN;
	static const int UP_ARROW_KEY = SDLK_UP;
	static const int ESCAPE_KEY = SDLK_ESCAPE;
  	static const int SPACE_KEY = SDLK_SPACE;
	static const int LEFT_MOUSE_BUTTON = SDL_BUTTON_LEFT;
	static const int RIGHT_MOUSE_BUTTON = SDL_BUTTON_RIGHT;
	static const int ENTER_KEY = SDLK_RETURN;
};

#endif
