#include "Player.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

Player::Player() {
	// INITIALIZE MOTION AND ROTATION VECTORS USED FOR PLAYER CONTROL
	motion = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
}

Player::~Player() {
}

void Player::_register_methods() {
	register_method((char*)"_process", &Player::_process);
	register_method((char*)"_input", &Player::_input);
	register_method((char*)"_ready", &Player::_ready);
}

void Player::_init() {

}

void Player::_ready() {
	Input* i = Input::get_singleton();
	i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);


}

void Player::_process(float delta) {
	UpdateMotionFromInput();
	this->translate(motion);
	set_rotation_degrees(rotation);
}

void Player::_physics_process(float delta) {
	
}

void Player::_input(InputEvent* e)
{
	
	motion = Vector3(0, 0, 0);
	
	// MOUSE MOTION EVENTS
	
	if (e->get_class() == "InputEventMouseMotion") {
		UpdateRotationFromInput((InputEventMouseMotion*)e); // Rotation and vertical motion using relative mouse coordinates
	}
	
	Input* i = Input::get_singleton();

	if (e->is_action_pressed("ui_turn")) {
		
		mouse_p = this->get_viewport()->get_mouse_position();
		i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);
	}

	if (e->is_action_released("ui_turn")) {
		i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);
		this->get_viewport()->warp_mouse(mouse_p);
	}

	this->translate(motion);

	if (e->is_action_pressed("ui_cancel")) {
		get_tree()->quit();								// EXIT GAME
	}
	
}

void Player::UpdateMotionFromInput() {
	motion = Vector3(0, 0, 0);							// RESET MOTION VECTOR TO ZERO

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
	Vector2 rot = e->get_relative();						// RELATIVE ROTATION VECTOR OBTAINED FROM MOUSE MOTION
	Input* i = Input::get_singleton();						// INPUT USED FOR MOUSE MODE
	
	if (i->get_mouse_mode() == i->MOUSE_MODE_CAPTURED)		// CHANGE PLAYER ROTATION ONLY IF MOUSE IS CAPTURED
	{
		// LOOK LEFT/RIGHT
		rotation.y -= rot.x * (SPEED_R / 360);				//Must depends on the screen size to avoid slower rotation on high def screens
		
		// ZOOM-IN MOTION 
		if (rot.y <= 0) {
			if (this->get_global_transform().get_origin().y <= MaxHeight)	// Set maximum height
			{
				motion.z -= (rot.y * cos(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight/2, VSPEED_POWER));
				motion.y -= (rot.y * sin(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
			}
		}
		else {
			// Set minimum height
			if (this->get_global_transform().get_origin().y >= MinHeight) 
			{
				motion.z -= (rot.y * cos(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
				motion.y -= (rot.y * sin(CameraAngleDeg * (2 * M_PI) / 180)) / (VSPEED_INVERSE / pow(this->get_global_transform().get_origin().y - MinHeight / 2, VSPEED_POWER));
			}
		}
	}

}

void Player::ChangeMouseMode() {
	Input* i = Input::get_singleton();

	// CHANGE MOUSE MODE
	if (i->get_mouse_mode() == i->MOUSE_MODE_CAPTURED) 
	{
		i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);
	}
	else {
		i->set_mouse_mode(i->MOUSE_MODE_CAPTURED);
	}
}
