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
const int InputManager::L3, InputManager::R3;
const int InputManager::K_L3, InputManager::K_R3;

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

const int InputManager::K_MENU_A, InputManager::K_MENU_B, InputManager::K_MENU_Y, InputManager::K_MENU_LB;
const int InputManager::MENU_MODE, InputManager::BATTLE_MODE;

InputManager::InputManager(){
	memset(mouse_state, false, sizeof mouse_state);
	memset(mouse_update, 0, sizeof mouse_update);

	keyboard_to_joystick_id = 0;
	map_keyboard_to_joystick(keyboard_to_joystick_id);

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
	keyboard_to_joystick.clear();
	key_state.clear();
	key_update.clear();
}

void InputManager::update(){
	SDL_Event event;

	m_quit_requested = false;

	update_counter++;

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
			emulate_joystick(key_id, true);
			break;

			case SDL_KEYUP:
			key_id = event.key.keysym.sym;
			key_state[key_id] = false;
			key_update[key_id] = update_counter;
			emulate_joystick(key_id, false);
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

bool InputManager::key_press(int key){
	return key_state[key] && key_update[key] == update_counter;
}

bool InputManager::key_release(int key){
	return !key_state[key] && key_update[key] == update_counter;
}

bool InputManager::is_key_down(int key){
	return key_state[key];
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

void InputManager::map_keyboard_to_joystick(int map_id){
	keyboard_to_joystick = {
		{K_LEFT , LEFT + 1},
		{K_RIGHT , RIGHT + 1},
		{K_UP , UP + 1},
		{K_DOWN , DOWN + 1},
		{K_A, A + 1},
		{K_B , B + 1},
		{K_X , X + 1},
		{K_Y , Y + 1},
		{K_LB , LB + 1},
		{K_RB , RB + 1},
		{K_LT , LT + 1},
		{K_RT , RT + 1},
		{K_SELECT , SELECT + 1},
		{K_START , START + 1},
		{K_L3, L3 + 1},
		{K_R3, R3 + 1}
	};

	if(map_id == MENU_MODE){
		keyboard_to_joystick[K_A] = 0;
		keyboard_to_joystick[K_B] = 0;
		keyboard_to_joystick[K_Y] = 0;
		keyboard_to_joystick[K_LB] = 0;
		keyboard_to_joystick[K_MENU_A] = A + 1;
		keyboard_to_joystick[K_MENU_B] = B + 1;
		keyboard_to_joystick[K_MENU_Y] = Y + 1;
		keyboard_to_joystick[K_MENU_LB] = LB + 1;
	}
}

void InputManager::emulate_joystick(int key_id, bool state){
	if(state){
		switch(key_id){
			case SDLK_0:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = -1;
			break;
			case SDLK_1:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = 0;
			break;
			case SDLK_2:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = 1;
			break;
			case SDLK_3:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = 2;
			break;
			case SDLK_4:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = 3;
			break;
			case SDLK_5:
				reset_keyboard_to_joystick();
				keyboard_to_joystick_id = 4;
			break;
		}
	}

	if(keyboard_to_joystick_id == 4){
		for(int i = 0; i < 4; ++i){
			joystick_state[i][keyboard_to_joystick[key_id] - 1] = state;
			joystick_update[i][keyboard_to_joystick[key_id] - 1] = update_counter;
		}
	}else if(keyboard_to_joystick_id >= 0){
		joystick_state[keyboard_to_joystick_id][keyboard_to_joystick[key_id] - 1] = state;
		joystick_update[keyboard_to_joystick_id][keyboard_to_joystick[key_id] - 1] = update_counter;
		//printf("Pressed or released %d\n", keyboard_to_joystick[key_id] - 1);
	}
}

void InputManager::reset_keyboard_to_joystick(){
	if(keyboard_to_joystick_id < 0 or keyboard_to_joystick_id > 3) return;
	for(auto & c : joystick_state[keyboard_to_joystick_id]){
		c.second = false;
	}
}
