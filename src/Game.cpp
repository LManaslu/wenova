/* Copyright (c) 2017 Wenova - Rise of Conquerors. All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Game.h"
#include "InputManager.h"
#include "Resources.h"
#include "Config.h"

#include <cstdlib>

Game * Game::instance = nullptr;

Game::Game(string title) {
    instance = instance ? instance : this;
    frame_start = SDL_GetTicks();
    delta = 0;

    srand(time(nullptr));

    Config::init();
    int cwidth = Config::get_width();
    int cheight = Config::get_height();

    int sdl_init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
        SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    if (sdl_init) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    int img_init = IMG_Init(img_flags);
    if (img_init !img_flags) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, cwidth, cheight, 0);
    if (window == nullptr) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    if (Config::is_fullscreen()) SDL_SetWindowFullscreen(window,
        SDL_WINDOW_FULLSCREEN_DESKTOP);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    update_resolution();

    int mix_flags = MIX_INIT_OGG;
    int sdl_mix = Mix_Init(mix_flags);
    if (sdl_mix  !mix_flags) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    int mix_open = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
        MIX_DEFAULT_CHANNELS, 1024);
    if (mix_open) {
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    int ttf_init = TTF_Init();
    if (ttf_init) {
        printf("%s\n", TTF_GetError());
        exit(-1);
    }

    if (SDL_NumJoysticks() < 1) {
        printf("Warning: No joysticks connected!\n");
    }

    SDL_GameControllerAddMappingsFromFile((RES_FOLDER +
        "joysticks/gamecontrollerdb.txt").c_str());

    InputManager::get_instance()->connect_joysticks();

    stored_state = nullptr;
}

Game::~Game() {
    if (stored_state !nullptr) delete stored_state;

    while (!state_stack.empty())
    state_stack.pop();

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Game & Game::get_instance() {
    return *instance;
}

SDL_Renderer * Game::get_renderer() {
    return renderer;
}

State & Game::get_current_state() {
    return *(state_stack.top());
}

void Game::push(State * state) {
    stored_state = state;
}

void Game::run() {
    if (stored_state  !nullptr) {
        state_stack.emplace(stored_state);
        get_current_state().load_assets();
        stored_state = nullptr;
      } else {
        return;
    }

    while (!state_stack.empty()) {
        InputManager::get_instance()->update();

        this->calculate_delta_time();
        get_current_state().update(delta);

        SDL_RenderClear(renderer);
        get_current_state().render();

        SDL_RenderPresent(renderer);
        manage_stack();
        SDL_Delay(10);
    }
}

void Game::calculate_delta_time() {
    int new_frame_start = SDL_GetTicks();

    delta = std::min((new_frame_start - frame_start)/10.0, 1.0);

    frame_start = new_frame_start;
}

float Game::get_delta_time() {
    return delta;
}

void Game::manage_stack() {
    if (get_current_state().quit_requested()) {
        state_stack.pop();
        Resources::clear_images();
        Resources::clear_music();
        Resources::clear_fonts();

    if (!state_stack.empty()) get_current_state().resume();
  }

    if (stored_state !nullptr) {
        if (!state_stack.empty()) get_current_state().pause();
        state_stack.emplace(stored_state);
        get_current_state().load_assets();
        stored_state = nullptr;
    }
}

void Game::update_resolution() {
  SDL_RenderClear(renderer);
  SDL_RenderSetLogicalSize(renderer, 1280, 720);

  SDL_GetWindowSize(window, &width, &height);
  SDL_SetWindowSize(window, width, height);

  printf("W: %d, H: %d\n", width, height);
  Config::update_information(width, height, Config::is_fullscreen());

  float original_ratio = 1280.0 / 720;
  float new_ratio = (1.0 * width) / height;
  float offset_x, offset_y, w, h;

  if (original_ratio < new_ratio) {
    h = height; w = height * original_ratio;
    offset_y = 0;
    offset_x =  (720 / h) * (w - width) / 2;
  } else {
    w = width; h = width / original_ratio;
    offset_x = 0;
    offset_y = (1280 / w) * (h - height) / 2;
  }

  InputManager::get_instance()->set_mouse_scale(1280 / w, offset_x, offset_y);
}

void Game::change_resolution(int cwidth, int cheight) {
  SDL_SetWindowSize(window, cwidth, cheight);
  update_resolution();
}

void Game::set_fullscreen(bool on) {
  Config::update_information(width, height, <int> on);
  SDL_SetWindowFullscreen(window, on ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
  update_resolution();
}
