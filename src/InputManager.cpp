#include "InputManager.h"

#include "Game.h"

#include <SDL2/SDL_render.h>
#include <cstring>
#include <ctime>

#define FIRST_TIME 1492356064

InputManager * InputManager::input_manager;

const int InputManager::UP, InputManager::DOWN, InputManager::RIGHT, InputManager::LEFT;
const int InputManager::A, InputManager::B, InputManager::X, InputManager::Y;
const int InputManager::LB, InputManager::RB, InputManager::START, InputManager::SELECT;
const int InputManager::LT, InputManager::RT;

const int InputManager::K_UP, InputManager::K_RIGHT, InputManager::K_DOWN;
const int InputManager::K_LEFT, InputManager::K_A, InputManager::K_B, InputManager::K_X;
const int InputManager::K_Y, InputManager::K_LB, InputManager::K_RB, InputManager::K_LT;
const int InputManager::K_RT, InputManager::K_SELECT, InputManager::K_START;
const int InputManager::LEFT_MOUSE_BUTTON, InputManager::RIGHT_MOUSE_BUTTON;
const int InputManager::ENTER_KEY, InputManager::K_RANDOM;

const int InputManager::K_ARROW_UP, InputManager::K_ARROW_RIGHT, InputManager::K_ARROW_DOWN;
const int InputManager::K_ARROW_LEFT, InputManager::K_CTRL, InputManager::K_C, InputManager::K_F;
const int InputManager::K_P, InputManager::K_O, InputManager::K_ROT_LEFT;
const int InputManager::K_ROT_RIGHT, InputManager::K_ROT_RESET, InputManager::K_INC_W;
const int InputManager::K_DEC_W, InputManager::K_DEL;

const int InputManager::K_MENU_A, InputManager::K_MENU_B, InputManager::K_MENU_Y;

InputManager::InputManager(){
	memset(mouse_state, false, sizeof mouse_state);
	memset(mouse_update, 0, sizeof mouse_update);

	for(int i = 0; i < 4; ++i) controllers[i] = nullptr;
	m_quit_requested = false;
	update_counter = 0;
	mouse_x = 0;
	mouse_y = 0;

}

InputManager::~InputManager(){
	for(int i = 0; i < 4; ++i){
		joystick_state[i].clear();
		joystick_update[i].clear();
	}
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
		int key_id, button_id;
		int joystick_id = controllers_id[event.cdevice.which];
		//printf("%d\n", event.type);
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

			case SDL_JOYAXISMOTION:
			//printf("Joy axis changed %d\n", update_counter);
			break;

			case SDL_CONTROLLERAXISMOTION:
			//case SDL_JOYAXISMOTION:
			if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX){
				joystick_state[joystick_id][RIGHT] = event.caxis.value > analogic_value;
				joystick_state[joystick_id][LEFT] = event.caxis.value < -analogic_value;
				joystick_update[joystick_id][RIGHT] = update_counter;
				joystick_update[joystick_id][LEFT] = update_counter;
			}else if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY){
				joystick_state[joystick_id][DOWN] = event.caxis.value > analogic_value;
				joystick_state[joystick_id][UP] = event.caxis.value < -analogic_value;
				joystick_update[joystick_id][DOWN] = update_counter;
				joystick_update[joystick_id][UP] = update_counter;
			}else if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT){
				joystick_state[joystick_id][LT] = event.caxis.value > trigger_value;
				joystick_update[joystick_id][LT] = update_counter;
			}else if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT){
				joystick_state[joystick_id][RT] = event.caxis.value > trigger_value;
				joystick_update[joystick_id][RT] = update_counter;
			}

			//printf("botao: %d\n", event.caxis.axis);
			// SDL_Log("Controller axis %s (%d) changed to %d\n", SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)event.caxis.axis), event.caxis.axis, event.caxis.value);
			break;

			case SDL_JOYBUTTONDOWN:
			//printf("Joy down %d\n", update_counter);
			break;

			case SDL_CONTROLLERBUTTONDOWN:
			//printf("Controller down %d\n", update_counter);
			//case SDL_JOYBUTTONDOWN:
			button_id = event.cbutton.button;
			joystick_state[joystick_id][button_id] = true;
			joystick_update[joystick_id][button_id] = update_counter;
			//printf("apertou joystick: %d (%s), joystick: %d %d\n", button_id, SDL_GameControllerGetStringForButton((SDL_GameControllerButton)button_id),joystick_id, joystick_button_press(button_id, joystick_id));
			break;

			case SDL_JOYBUTTONUP:
			//printf("Joy up %d\n", update_counter);
			break;

			case SDL_CONTROLLERDEVICEADDED:
			connect_joysticks();
			//printf("Conectou %d\n", event.cdevice.which);
			break;

			case SDL_CONTROLLERDEVICEREMOVED:
			connect_joysticks();
			//printf("Desconectou %d\n", event.cdevice.which);
			break;

			case SDL_CONTROLLERBUTTONUP:
			//printf("Controller up %d\n", update_counter);
			button_id = event.cbutton.button;
			joystick_state[joystick_id][button_id] = false;
			joystick_update[joystick_id][button_id] = update_counter;
			//printf("soltou joystick: %d, joystick: %d %d\n", button_id, joystick_id, joystick_button_press(button_id, joystick_id));
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

void InputManager::set_analogic_value(int value){
	analogic_value = value;
}

void InputManager::connect_joysticks(){
	int max = SDL_NumJoysticks();
	if(max > 4) max = 4;
	int n_controller = 0;

	for(int i = 0; i < max; ++i){
		if(controllers[i] != nullptr){
			SDL_GameControllerClose(controllers[i]);
			controllers[i] = nullptr;
		}
	}


	for(int i = 0; i < max; ++i){
		char guid[64];
		SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof (guid));
		//printf("Guid = %s\n", guid);

		if(SDL_IsGameController(i)){
			controllers[i] = SDL_GameControllerOpen(i);

			SDL_Joystick *j = SDL_GameControllerGetJoystick(controllers[i]);
			int instance_id = SDL_JoystickInstanceID(j);
			printf("Controller %d (%d real) connected\n", i, instance_id);
			controllers_id[instance_id] = i;
			n_controller++;
		}else{
			printf("WARNING: Joystick is not a game controller\n");
			SDL_JoystickOpen(i);

			//exit(11);
		}
	}
}
