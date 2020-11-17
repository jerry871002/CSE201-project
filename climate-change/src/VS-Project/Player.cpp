#include "Player.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Player::_register_methods()
{
	register_method((char*)"_process", &Player::_process);
	register_method((char*)"_input", &Player::_input);
	register_method((char*)"_ready", &Player::_ready);
}

void Player::_init()
{

}

void Player::_process(float delta)
{
	UpdateMotionFromInput();
	this->translate(motion);
	set_rotation_degrees(rotation);

}

void Player::_input(InputEvent* e)
{
	if (e->get_class() == "InputEventMouseMotion") {
		UpdateRotationFromInput((InputEventMouseMotion*)e);
	};
	if (e->get_class() == "InputEventMouseButton") {
		if (((InputEventMouseButton*)e)->is_doubleclick()) {
			ChangeMouseMode();
		}

	}
}

void Player::_ready()
{
	Input* i = Input::get_singleton();
	i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);

}


Player::Player()
{
	motion = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
}

Player::~Player()
{
}

void Player::UpdateMotionFromInput()
{
	motion = Vector3(0, 0, 0);
	Input* i = Input::get_singleton();
	if (i->is_action_pressed("ui_vup")) {
		if (this->get_global_transform().get_origin().y <= 20) { motion.y += SPEED_T; }
	}
	else if (i->is_action_pressed("ui_vdown")) {
		if (this->get_global_transform().get_origin().y >= 3) { motion.y -= SPEED_T; }
	}
	if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) {}
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) { motion.z -= SPEED_T; }
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) { motion.z += SPEED_T; }
	else if ((i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down"))) { motion.x += SPEED_T; }
	else if ((i->is_action_pressed("ui_left")) && (i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down"))) { motion.x -= SPEED_T; }
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down"))) {}
	else if ((i->is_action_pressed("ui_left")) && (i->is_action_pressed("ui_right"))) {}
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_right"))) {
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
	else if (i->is_action_pressed("ui_up")) { motion.z -= SPEED_T; }
	else if (i->is_action_pressed("ui_down")) { motion.z += SPEED_T; }
	else if (i->is_action_pressed("ui_right")) { motion.x += SPEED_T; }
	else if (i->is_action_pressed("ui_left")) { motion.x -= SPEED_T; }

}

void Player::UpdateRotationFromInput(InputEventMouseMotion* e) {

	Vector2 rot = e->get_relative();
	rotation.y -= rot.x * (SPEED_R / 360);    //Must depends on the screen size to avoid slower rotation on high def screens
	// rotation.x -= rot.y * (SPEED_R / 360);
}

void Player::ChangeMouseMode()
{
	Input* i = Input::get_singleton();
	if (i->get_mouse_mode() == i->MOUSE_MODE_CAPTURED) {
		i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);
	}
	else {
		i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);
	}
}

