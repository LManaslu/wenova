#include "Camera.h"
#include "InputManager.h"

/** @file arquivo.h */

/**
 * Camera implementation functions follow and unfollow the player
 * and update the camera.
 */
GameObject * Camera::focus = nullptr;
Vector Camera::speed;
Vector Camera::pos[LAYERS];
float Camera::layer_speed[LAYERS] = {4, 6, 16, 32};

#define PENGUIN_LAYER 0
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

/**
 * Camera follows the player.
 */
void Camera::follow(GameObject * new_focus) {
	focus = new_focus;
}

/**
 * Camera unfollow the player.
 */
void Camera::unfollow() {
	focus = nullptr;
}

/**
 * Camera updates position based on pressed joystick buttons.
 */
void Camera::update(float delta) {
	InputManager * input_manager = InputManager::get_instance();

	if (focus == nullptr) {
		bool directions[] = {
			input_manager->is_joystick_button_down(0, InputManager::LEFT),
			/**< Verify if left button is NOT pressed. */
			input_manager->is_joystick_button_down(0, InputManager::UP),
			/**< Verify if up button is NOT pressed. */
			input_manager->is_joystick_button_down(0, InputManager::RIGHT),
			/**< Verify if right button is NOT pressed. */
			input_manager->is_joystick_button_down(0, InputManager::DOWN)
			/**< Verify if down button is NOT pressed. */
		};

		speed.x = 0; /**< Speed on axis X is set to 0. */
		speed.y = 0; /**< Speed on axis Y is set to 0. */

		/**
 		* Camera updates position based on pressed joystick buttons.
 		*/
		for (int i = 0; i < 4; ++i) {
			speed.x += ((i+1) % 2) * (i - 1) * directions[i] * delta;
			speed.y += (i % 2) * (i - 2) * directions[i] * delta;
		}

		for (int i = 0; i < LAYERS; ++i) {
			pos[i].x += speed.x * layer_speed[i];
			pos[i].y += speed.y * layer_speed[i];
		}
		
	} else {
		for (int i = 0; i < LAYERS; ++i) {
			pos[i].x = (-focus->box.get_x() + SCREEN_WIDTH/2.0) *
			layer_speed[i]/layer_speed[PENGUIN_LAYER];
			pos[i].y = (-focus->box.get_y() + SCREEN_HEIGHT/2.0) *
			layer_speed[i]/layer_speed[PENGUIN_LAYER];
		}
	}
}
