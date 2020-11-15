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
		motion.z -= SPEED / (sqrt(2));
		motion.x += SPEED / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right"))) {
		motion.z += SPEED / (sqrt(2));
		motion.x += SPEED / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_left"))) {
		motion.z -= SPEED / (sqrt(2));
		motion.x -= SPEED / (sqrt(2));
	}
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_left"))) {
		motion.z += SPEED / (sqrt(2));
		motion.x -= SPEED / (sqrt(2));
	}
	else if (i->is_action_pressed("ui_up")) {
		motion.z -= SPEED;
	}
	else if (i->is_action_pressed("ui_down")) {
		motion.z += SPEED;
	}
	else if (i->is_action_pressed("ui_right")) {
		motion.x += SPEED;
	}
	else if (i->is_action_pressed("ui_left")) {
		motion.x -= SPEED;
	}

}
