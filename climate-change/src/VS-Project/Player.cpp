#include "Player.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Math.hpp>
# define M_PI 3.14159265358979323846  /* pi */

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
	motion = Vector3(0, 0, 0);

	// MOUSE MOTION EVENTS
	if (e->get_class() == "InputEventMouseMotion") {
		// Rotation and vertical motion using relative mouse coordinates
		UpdateRotationFromInput((InputEventMouseMotion*)e);
	};

	// MOUSE BUTTON EVENTS
	if (e->get_class() == "InputEventMouseButton") {
		// Double click to change mouse mode
		if (((InputEventMouseButton*)e)->is_doubleclick()) {
			ChangeMouseMode();
		}

	}

	this->translate(motion);
}

void Player::_ready()
{
	// SET INITIAL MOUSE MODE TO CAPTURED
	Input* i = Input::get_singleton();
	i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);

}


Player::Player()
{
	// INITIALIZE MOTION AND ROTATION VECTORS USED FOR PLAYER CONTROL
	motion = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);

	// CUSTOM CAMERA
	//camera.set_fov(35);
	//camera.set_rotation_degrees(Vector3(-15, 0, 0));
	
}

Player::~Player()
{
}

void Player::UpdateMotionFromInput()
{
	// RESET MOTION VECTOR TO ZERO
	motion = Vector3(0, 0, 0);

	// INPUT USED FOR KEY CONTROLS
	Input* i = Input::get_singleton();

	// VERTICAL MOTION

	if (i->is_action_pressed("ui_vup")) {
		if (this->get_global_transform().get_origin().y <= MaxHeight) { motion.y += SPEED_T; }
	}
	else if (i->is_action_pressed("ui_vdown")) {
		if (this->get_global_transform().get_origin().y >= MinHeight) { motion.y -= SPEED_T; }
	}

	// PLANAR MOTION

	// 4-key combinations
	if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) {}
	// 3-key combinations
	else if ((i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) { motion.z -= SPEED_T; }
	else if ((i->is_action_pressed("ui_down")) && (i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_left"))) { motion.z += SPEED_T; }
	else if ((i->is_action_pressed("ui_right")) && (i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down"))) { motion.x += SPEED_T; }
	else if ((i->is_action_pressed("ui_left")) && (i->is_action_pressed("ui_up")) && (i->is_action_pressed("ui_down"))) { motion.x -= SPEED_T; }
	// 2-key combinations
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
	// 1-key combinations
	else if (i->is_action_pressed("ui_up")) { motion.z -= SPEED_T; }
	else if (i->is_action_pressed("ui_down")) { motion.z += SPEED_T; }
	else if (i->is_action_pressed("ui_right")) { motion.x += SPEED_T; }
	else if (i->is_action_pressed("ui_left")) { motion.x -= SPEED_T; }

}

void Player::UpdateRotationFromInput(InputEventMouseMotion* e) {

	// RELATIVE ROTATION VECTOR OBTAINED FROM MOUSE MOTION
	Vector2 rot = e->get_relative();

	// INPUT USED FOR MOUSE MODE
	Input* i = Input::get_singleton();
	
	// CHANGE PLAYER ROTATION ONLY IF MOUSE IS CAPTURED
	if (i->get_mouse_mode() == i->MOUSE_MODE_CAPTURED)
	{
		// LOOK LEFT/RIGHT
		rotation.y -= rot.x * (SPEED_R / 360);    //Must depends on the screen size to avoid slower rotation on high def screens

		// LOOK UP/DOWN
		// rotation.x -= rot.y * (SPEED_R / 360);

		// ZOOM-IN MOTION 
		if (rot.y <= 0) {
			// Set maximum height
			if (this->get_global_transform().get_origin().y <= MaxHeight) {
				motion.z -= (rot.y * cos(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight/2, VSPEED_POWER));
				motion.y -= (rot.y * sin(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
			}
		}
		else {
			// Set minimum height
			if (this->get_global_transform().get_origin().y >= MinHeight) {
				motion.z -= (rot.y * cos(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
				motion.y -= (rot.y * sin(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
			}
		}

	}

}

void Player::ChangeMouseMode()
{
	Input* i = Input::get_singleton();
	// CHANGE MOUSE MODE
	if (i->get_mouse_mode() == i->MOUSE_MODE_CAPTURED) {
		i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);
	}
	else {
		i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);
	}
}

void Player::_physics_process(float delta) {


}

