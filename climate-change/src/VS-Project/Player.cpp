#include "Player.h"
#include <Math.hpp>

using namespace godot;

void Player::_register_methods()
{
	register_method((char*)"_process", &Player::_process);
}

void Player::_init()
{
}

void Player::_process(float delta)
{
	UpdateMotionFromInput();
	move_and_slide(motion);
	UpdateRotationFromInput();
	set_rotation_degrees(rotation);

}


Player::Player()
{
	motion = Vector3(0, 0, 0);
}

Player::~Player()
{
}

void Player::UpdateMotionFromInput()
{
	motion = Vector3(0, 0, 0);
	Input* i = Input::get_singleton();
	if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_right"))) {
		motion.z -= SPEED_T / (sqrt(2));
		motion.x += SPEED_T / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right"))) {
		motion.z += SPEED_T / (sqrt(2));
		motion.x += SPEED_T / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_left"))) {
		motion.z -= SPEED_T / (sqrt(2));
		motion.x -= SPEED_T / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_left"))) {
		motion.z += SPEED_T / (sqrt(2));
		motion.x -= SPEED_T / (sqrt(2));
	}
	else if (i->is_action_pressed("ui_up")) {
		motion.z -= SPEED_T;
	}
	else if (i->is_action_pressed("ui_down")) {
		motion.z += SPEED_T;
	}
	else if (i->is_action_pressed("ui_right")) {
		motion.x += SPEED_T;
	}
	else if (i->is_action_pressed("ui_left")) {
		motion.x -= SPEED_T;
	}

}

void Player::UpdateRotationFromInput() {

}
