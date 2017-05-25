#include "InputManager.h"

#include <cstring>
#include <ctime>

#define FIRST_TIME 1492356064

InputManager::InputManager(){
  memset(mouse_state, false, sizeof mouse_state);
  memset(mouse_update, 0, sizeof mouse_update);
  m_quit_requested = false;
  update_counter = 0;
  mouse_x = 0;
  mouse_y = 0;
}

InputManager::~InputManager(){
  key_state.clear();
  key_update.clear();
}

void InputManager::update(){
  SDL_Event event;

  SDL_GetMouseState(&mouse_x, &mouse_y);

  m_quit_requested = false;

  update_counter++;

  while(SDL_PollEvent(&event)){

    int key_id, button_id;
    switch (event.type) {
      case SDL_KEYDOWN:
        if(event.key.repeat) break;
        key_id = event.key.keysym.sym;
        //printf("Apertou %d\n", key_id);
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

int InputManager::get_mouse_x(){
  return mouse_x;
}

int InputManager::get_mouse_y(){
  return mouse_y;
}

bool InputManager::quit_requested(){
  return m_quit_requested;
}

InputManager & InputManager::get_instance(){
  static InputManager input_manager;
  return input_manager;
}
