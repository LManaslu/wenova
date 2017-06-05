#include "InputManager.h"

#include "Game.h"

#include <SDL2/SDL_render.h>
#include <cstring>
#include <ctime>

#define FIRST_TIME 1492356064

InputManager * InputManager::input_manager;

InputManager::InputManager(){
	memset(mouse_state, false, sizeof mouse_state);
	memset(mouse_update, 0, sizeof mouse_update);
	memset(joystick_state, false, sizeof joystick_state);
	memset(joystick_update, 0, sizeof joystick_update);
	m_quit_requested = false;
	update_counter = 0;
	mouse_x = 0;
	mouse_y = 0;
}

InputManager::~InputManager(){
	key_state.clear();
	key_update.clear();
	event_responded.clear();
}

void InputManager::update(){
	SDL_Event event;

	m_quit_requested = false;

	update_counter++;

	event_responded.clear();
	SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_x = mouse_x * scale + offset_x;
	mouse_y = mouse_y * scale + offset_y;
	mouse_x = max(0, mouse_x); mouse_x = min(mouse_x, 1280);
	mouse_y = max(0, mouse_y); mouse_y = min(mouse_y, 720);


	while(SDL_PollEvent(&event)){
		int key_id, button_id, joystick_id;
		switch (event.type) {
			case SDL_KEYDOWN:
			if(event.key.repeat) break;
			key_id = event.key.keysym.sym;
			key_state[key_id] = true;
			key_update[key_id] = update_counter;
			break;

			case SDL_KEYUP:
			key_id = event.key.keysym.sym;
			key_state[key_id] = false;
			key_update[key_id] = update_counter;
			break;

			case SDL_MOUSEBUTTONDOWN:
			button_id = event.button.button;
			mouse_state[button_id] = true;
			mouse_update[button_id] = update_counter;
			break;

			case SDL_MOUSEBUTTONUP:
			button_id = event.button.button;
			mouse_state[button_id] = false;
			mouse_update[button_id] = update_counter;
			break;

			case SDL_JOYBUTTONDOWN:
			button_id = event.jbutton.button + 4;
			joystick_id = event.cdevice.which;
			joystick_state[joystick_id][button_id] = true;
			joystick_update[joystick_id][button_id] = update_counter;
			printf("apertou joystick: %d, joystick: %d %d\n", button_id, joystick_id, joystick_button_press(button_id, joystick_id));
			break;

			case SDL_JOYBUTTONUP:
			button_id = event.jbutton.button + 4;
			joystick_id = event.cdevice.which;
			joystick_state[joystick_id][button_id] = false;
			joystick_update[joystick_id][button_id] = update_counter;
			printf("soltou joystick: %d, joystick: %d %d\n", button_id, joystick_id, joystick_button_press(button_id, joystick_id));
			break;

			case SDL_JOYHATMOTION:
			button_id = event.jhat.value;
			joystick_id = event.cdevice.which;
			printf("apertou d-pad: %d, joystick: %d\n", button_id, joystick_id);
			for(int i = 0; i < 4; i++) {
				if(button_id & (1<<i)) {
					joystick_state[joystick_id][i] = true;
					joystick_update[joystick_id][i] = update_counter;
				}
			}
			break;

			case SDL_QUIT:
			m_quit_requested = true;
			break;

			default:
			break;
		}
	}
}

bool InputManager::key_press(int key, bool response){
	if(not can_respond(key, 0, response)) return false;
	bool v = (key_state[key] && key_update[key] == update_counter);
	return v;
}

bool InputManager::key_release(int key, bool response){
	if(not can_respond(key, 1, response)) return false;
	bool v = (!key_state[key] && key_update[key] == update_counter);
	return v;
}

bool InputManager::is_key_down(int key, bool response){
	if(not can_respond(key, 2, response)) return false;
	bool v = key_state[key];
	return v;
}

bool InputManager::mouse_press(int button){
	return mouse_state[button] && mouse_update[button] == update_counter;
}

bool InputManager::mouse_release(int button){
	return !mouse_state[button] && mouse_update[button] == update_counter;
}
bool InputManager::is_mouse_down(int button){
	return mouse_state[button];
}

bool InputManager::joystick_button_press(int button, int joystick){
	return joystick_state[joystick][button] && joystick_update[joystick][button] == update_counter;
}

bool InputManager::joystick_button_release(int button, int joystick){
	return !joystick_state[joystick][button] && joystick_update[joystick][button] == update_counter;
}
bool InputManager::is_joystick_button_down(int button, int joystick){
	return joystick_state[joystick][button];
}

int InputManager::get_mouse_x(){
	return mouse_x;
}

int InputManager::get_mouse_y(){
	return mouse_y;
}

bool InputManager::quit_requested(){
	return m_quit_requested;
}

InputManager * InputManager::get_instance(){
	if(input_manager == nullptr) input_manager = new InputManager();
	return input_manager;
}

bool InputManager::can_respond(int key, int operation, bool response){
	//TODO mudar pra ser id de gameObject
	if(not response) return true;
	if(event_responded[ii(key, operation)])
		return false;
	else
		return (event_responded[ii(key, operation)] = true);
}

void InputManager::set_mouse_scale(float cscale, int coffset_x, int coffset_y){
	scale = cscale;
	offset_x = coffset_x;
	offset_y = coffset_y;
}
