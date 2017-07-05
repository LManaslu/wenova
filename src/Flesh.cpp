#include "Flesh.h"
#include "Game.h"
#include "UltimateEffect.h"

#define CROUCH_COOLDOWN 400.0

Flesh::Flesh(string skin, float x, float y, int cid, Fighter *cpartner) : Fighter(cid, x, cpartner){
	path = "flesh/" + skin + "/";

	sprite[IDLE] = Sprite(path + "idle.png", 8, 10);
	sprite[RUNNING] = Sprite(path + "running.png", 8, 10);
	sprite[JUMPING] = Sprite(path + "jumping.png", 6, 10);
	sprite[FALLING] = Sprite(path + "falling.png", 7, 10);
	sprite[CROUCH] = Sprite(path + "crouch.png", 6, 20);
	sprite[IDLE_ATK_NEUTRAL_1] = Sprite(path + "idle_atk_neutral_1.png", 4, 10);
	sprite[IDLE_ATK_NEUTRAL_2] = Sprite(path + "idle_atk_neutral_2.png", 3, 10);
	sprite[IDLE_ATK_NEUTRAL_3] = Sprite(path + "idle_atk_neutral_3.png", 5, 10);
	sprite[IDLE_ATK_UP] = Sprite(path + "idle_atk_up.png", 4, 10);
	sprite[IDLE_ATK_FRONT] = Sprite(path + "idle_atk_front.png", 4, 10);
	sprite[JUMP_ATK_DOWN_FALLLOOP] = Sprite(path + "jump_atk_down_fallloop.png", 3, 10);
	sprite[JUMP_ATK_DOWN_DMG] = Sprite(path + "jump_atk_down_dmg.png", 3, 10);
	sprite[JUMP_ATK_UP] = Sprite(path + "jump_atk_up.png", 4, 10);
	sprite[IDLE_ATK_DOWN] = Sprite(path + "idle_atk_down.png", 4, 10);
	sprite[CROUCH_ATK] = Sprite(path + "crouch_atk.png", 4, 10);
	sprite[SPECIAL_1] = Sprite(path + "special_1.png", 3, 30);

	crouching_size = Vector(84, 59);
	not_crouching_size = Vector(84, 84);

	tags["flesh"] = true;
	tags[skin] = true;
	box = Rectangle(x, y, 84, 84);
	additional_attack_damage = 0;
	additional_speed = 0;
}

void Flesh::update_machine_state(float delta){
	switch(state){
		case FighterState::JUMP_ATK_UP:
			attack_damage = 7 * (sprite[state].get_current_frame() == 1);
			attack_mask = get_attack_orientation();
			check_left(false);
			check_right(false);
			if(sprite[state].is_finished()){
				speed.y = 0.1;
				check_fall();
				check_idle();
			}
		break;

		case FighterState::IDLE_ATK_UP:
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_1:
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_2();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_2:
			attack_damage = 5 * (sprite[state].get_current_frame() == 1);
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_3();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_3:
			attack_damage = 10 * (sprite[state].get_current_frame() == 2);
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_FRONT:
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::CROUCH_ATK:
		attack_damage = 3 * (sprite[state].get_current_frame() == 1);
		attack_mask = get_attack_orientation() | AttackDirection::ATK_DOWN;
		if(sprite[state].is_finished()){
			check_idle();
			check_crouch();
		}

		case FighterState::JUMP_ATK_DOWN_FALLLOOP:
			speed.x = 3 * (orientation == LEFT ? -1 : 1);
			speed.y = 3;
			attack_damage = BASIC_ATTACK_DAMAGE + additional_attack_damage;
			attack_mask = get_attack_orientation();

			check_jump_atk_down_dmg();
			if(on_floor){
				printf("to no chao, parsa\n");
				speed.x = 0;
				speed.y = 0;
				check_idle();
				check_left();
				check_right();
				check_crouch();
			}
		break;

		case FighterState::JUMP_ATK_DOWN_DMG:
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_DOWN:
			attack_damage = BASIC_ATTACK_DAMAGE + additional_attack_damage;
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::STUNT:
			attack_damage = 0;
			attack_mask = 0;
			check_special_2();
			if(sprite[state].is_finished()){
				check_fall();
				check_idle();
				check_dead();
			}
		break;

		case FighterState::SPECIAL_1:
			attack_damage = SPECIAL_1_DAMAGE + additional_attack_damage;
			attack_mask = get_attack_orientation();
			if(sprite[state].is_finished()){
				check_fall();
				check_idle();
			}
		break;

		case FighterState::IDLE:
			combo = 0;
			attack_mask = attack_damage = 0;
			check_jump();
			check_left();
			check_right();
			check_idle_atk_down();
			check_crouch();
			check_fall();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_special_1();
			check_ultimate();
			check_defense();
			check_pass_through_platform();
			check_dead();
			check_idle_atk_up();
		break;

		case FighterState::JUMPING:
			check_left(false);
			check_right(false);
			check_fall();
			check_jump_atk_down_fallloop();
			check_idle();
			check_ultimate();
			check_jump_atk_up();
		break;

		case FighterState::FALLING:
			check_idle();
			check_left(false);
			check_right(false);
			check_fall();
			check_crouch();
			check_jump_atk_down_fallloop();
			check_ultimate();
			check_jump_atk_up();
		break;

		case FighterState::RUNNING:
			check_jump();
			check_left(false);
			check_right(false);
			check_idle();
			check_crouch();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_fall();
			check_special_1();
			check_ultimate();
			check_defense();
			check_pass_through_platform();
			check_idle_atk_up();
		break;

		case FighterState::CROUCH:
			check_idle();
			check_crouch_atk();
			check_fall();
		break;

		case FighterState::DEFENDING:
			attack_damage = 0;
			attack_mask = 0;
			check_idle();
			check_fall();
		break;

		case FighterState::DYING:
			if(sprite[state].is_finished()){
				remaining_life = 0;
			}
		break;

		case FighterState::LAST:
			printf("Invalid blood %d state\n", id);
			exit(-1);
		break;
	}
}

void Flesh::check_jump(bool change){
	if(pressed[JUMP_BUTTON]){
		if(change) temporary_state = FighterState::JUMPING;
		speed.y = -5;
		on_floor = false;
	}
}

void Flesh::check_fall(bool change){
	if(speed.y > 0){
		if(change) temporary_state = FighterState::FALLING;
	}
}

void Flesh::check_left(bool change){
	if(is_holding[LEFT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = -(INITIAL_SPEED + additional_speed);
		orientation = Orientation::LEFT;
	}
}

void Flesh::check_right(bool change){
	if(is_holding[RIGHT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = (INITIAL_SPEED + additional_speed);
		orientation = Orientation::RIGHT;
	}
}

void Flesh::check_idle(bool change){
	if(speed.x == 0 and on_floor and not is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE;
		printf("Temporary state = %d\n", temporary_state);
	}
}

void Flesh::check_crouch(bool change){
	if(is_holding[DOWN_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::CROUCH;
    }
}

void Flesh::check_idle_atk_neutral_1(bool change){
	if(pressed[ATTACK_BUTTON] and not is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_1;
	}
}

void Flesh::check_idle_atk_neutral_2(bool change){
	printf("Pressing: %d\n", is_holding[ATTACK_BUTTON]);
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_2;
	}
}

void Flesh::check_idle_atk_neutral_3(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_3;
	}
}

void Flesh::check_idle_atk_front(bool change){
	if(pressed[ATTACK_BUTTON] and (is_holding[LEFT_BUTTON] or is_holding[RIGHT_BUTTON])){
		if(change) temporary_state = FighterState::IDLE_ATK_FRONT;
		orientation = is_holding[LEFT_BUTTON] ? Orientation::LEFT : Orientation::RIGHT;
	}
}

void Flesh::check_jump_atk_down_fallloop(bool change){
	if(pressed[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN_FALLLOOP;
	}
}

void Flesh::check_jump_atk_down_dmg(bool change){
	if(grab){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN_DMG;
	}
}

void Flesh::check_idle_atk_down(bool change){
	if(is_holding[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE_ATK_DOWN;
	}
}

void Flesh::check_special_1(bool change){
	if(pressed[SPECIAL1_BUTTON]) {
		if(change) temporary_state = FighterState::SPECIAL_1;
	}
}

void Flesh::check_special_2(bool){
	additional_attack_damage = get_remaining_life() / Fighter::MAX_LIFE * BASIC_ATTACK_DAMAGE;
	additional_speed = get_remaining_life() / Fighter::MAX_LIFE * INITIAL_SPEED * 0.5;
}

void Flesh::check_ultimate(bool) {
	if(pressed[ULTIMATE_BUTTON] and special == MAX_SPECIAL){
		MAX_LIFE += 500;
		Game::get_instance().get_current_state().add_object(new UltimateEffect(this, path + "ult_effect.png", path + "aura.png", "has_sprite", 1));
	}
}

void Flesh::check_pass_through_platform(bool change) {
	if(pressed[DOWN_BUTTON] and not is_holding[ATTACK_BUTTON]){
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			if (change) temporary_state = FighterState::FALLING;
			pass_through_timer.restart();
		}
		crouch_timer.restart();
	}
}

void Flesh::check_defense(bool change){
	if(is_holding[BLOCK_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::DEFENDING;
    }
}

void Flesh::check_stunt(bool change){
	speed.x = 0;
	if(change) temporary_state = FighterState::STUNT;
}

void Flesh::check_dead(bool change) {
	if(is("dying")) {
		if(change) temporary_state = FighterState::DYING;
	}
}

void Flesh::check_crouch_atk(bool change){
	if(pressed[ATTACK_BUTTON]){
		if(change) temporary_state = FighterState::CROUCH_ATK;
	}
}

void Flesh::check_jump_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		speed.y = -5;
		if(change) temporary_state = FighterState::JUMP_ATK_UP;
	}
}

void Flesh::check_idle_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(change) temporary_state = FighterState::IDLE_ATK_UP;
	}
}
