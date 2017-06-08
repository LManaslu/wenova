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

	unordered_map<int, bool> joystick_state[4];
	unordered_map<int, int> joystick_update[4];


	unordered_map<int, bool> key_state;
	unordered_map<int, int> key_update;
	map<ii, int> event_responded;

	unordered_map<int, int> button_map;
	unordered_map<int, int> controllers_id;
	SDL_GameController * controllers[4];

	bool m_quit_requested;
	int update_counter;

	int mouse_x;
	int mouse_y;

	bool can_respond(int key, int operation, bool response);
	int offset_x;
	int offset_y;
	float scale;

	int analogic_value = 20000;
	int trigger_value = 32000;

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

	bool joystick_button_press(int button, int joystick);
	bool joystick_button_release(int button, int joystick);
	bool is_joystick_button_down(int button, int joystick);

	int get_mouse_x();
	int get_mouse_y();

	bool quit_requested();

	static InputManager * get_instance();

	void set_mouse_scale(float scale, int offset_x, int offset_y);
	void set_analogic_value(int value);
	void connect_joysticks();

	static const int UP = SDL_CONTROLLER_BUTTON_DPAD_UP;
	static const int RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	static const int DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	static const int LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	static const int A = SDL_CONTROLLER_BUTTON_A;
	static const int B = SDL_CONTROLLER_BUTTON_B;
	static const int X = SDL_CONTROLLER_BUTTON_X;
	static const int Y = SDL_CONTROLLER_BUTTON_Y;
	static const int LB = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	static const int RB = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	static const int LT = 0x20;
	static const int RT = 0x21; //Valor arbitrário, mas tá certo
	static const int SELECT = SDL_CONTROLLER_BUTTON_BACK;
	static const int START = SDL_CONTROLLER_BUTTON_START;
	static const int L3 = SDL_CONTROLLER_BUTTON_LEFTSTICK;
	static const int R3 = SDL_CONTROLLER_BUTTON_RIGHTSTICK;



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
