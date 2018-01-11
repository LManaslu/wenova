/* Copyright (c) 2017 Wenova - Rise of Conquerors. All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Animation.h"
#include "Camera.h"

#define LAYER 0

Animation::Animation(float x, float y, float crotation, string csprite,
    int frame_count, float frame_time, bool ends) {
    sprite = Sprite(csprite, frame_count, frame_time);
    rotation = crotation;
    box = Rectangle(x, y, sprite.get_width(), sprite.get_height());
    one_time_only = ends;
    time_limit = frame_count * frame_time;
}

void Animation::update(float delta) {
    end_timer.update(delta);
    sprite.update(delta);
}

void Animation::render() {
    int x = box.get_draw_x()  + Camera::pos[LAYER].x;
    int y = box.get_draw_y() + Camera::pos[LAYER].y;
    sprite.render(x, y, rotation);
}

bool Animation::is_dead() {
    return end_timer.get() >= time_limit || !one_time_only;
}

void Animation::notify_collision(GameObject &) {
    // Nothing to do
}
