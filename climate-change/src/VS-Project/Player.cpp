#include "Player.h"
#include "City.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Math.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <WorldEnvironment.hpp>
#include <Environment.hpp>
#include <iostream>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

Player::Player() {
	// INITIALIZE MOTION AND ROTATION VECTORS USED FOR PLAYER CONTROL
	motion = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	std::cout << "DEBUG: PLAYER CONSTRUCTOR CALLED" << std::endl;
	movable = true;
	counter = 0;
}

void Player::set_movable(bool b) 
{
	this->movable = b;
	movable = b;
}

bool Player::is_movable()
{
	return this->movable;
}

void Player::_on_MenuShop_visibility_changed() 
{
	this->set_movable(false);
}

void Player::_on_PoliciesInput_visibility_changed() 
{
	if (((bool)(this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible"))) == false) {
		this->set_movable(true);
	}
	
}

Player::~Player() {
}

void Player::_register_methods() {
	register_method((char*)"_physics_process", &Player::_physics_process);
	register_method((char*)"_input", &Player::_input);
	register_method((char*)"_ready", &Player::_ready);
	register_method((char*)"_on_MenuShop_visibility_changed", &Player::_on_MenuShop_visibility_changed);
	register_method((char*)"_on_PoliciesInput_visibility_changed", &Player::_on_PoliciesInput_visibility_changed);
}

void Player::_init() {

}

void Player::_ready() {
	Input* i = Input::get_singleton();
	i->set_mouse_mode(i->MOUSE_MODE_VISIBLE);
	WorldEnvironment* worldEnv = (WorldEnvironment*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment"));
	worldEnv->get_environment()->set_dof_blur_far_enabled(true);
	worldEnv->get_environment()->set_dof_blur_near_enabled(true);
	
}

void Player::_physics_process(float delta) 
{
	/*
	counter += 1;
	if (counter == 100) {
		
		std::cout << "DEBUG: MOVABLE SHOULD CHANGE" << std::endl;
		this->set_movable(!(this->is_movable()));
		counter = 0;
	}
	*/

	if (this->is_movable()) { UpdateMotionFromInput(delta); }
	
	
	WorldEnvironment* worldEnv = (WorldEnvironment*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment"));
	worldEnv->get_environment()->set_dof_blur_far_distance(2 * (this->get_global_transform().get_origin().y));
	worldEnv->get_environment()->set_dof_blur_far_amount(0.1 * pow((1 - (this->get_global_transform().get_origin().y - MinHeight) / (MaxHeight - MinHeight)), 3) );
	
	
	this->translate(motion); 
	set_rotation_degrees(rotation);
	
}

void Player::_process(float delta) 
{
	
}

void Player::_input(InputEvent* e)
{
	
	motion = Vector3(0, 0, 0);
	
	// MOUSE MOTION EVENTS
	
	if (e->get_class() == "InputEventMouseMotion") {
		
		
		if (this->is_movable()) {
			UpdateRotationFromInput((InputEventMouseMotion*)e); // Rotation and vertical motion using relative mouse coordinates
		}
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

void Player::UpdateMotionFromInput(float delta) {
	motion = Vector3(0, 0, 0);							// RESET MOTION VECTOR TO ZERO

	
		// INPUT USED FOR KEY CONTROLS
		Input* i = Input::get_singleton();

		SPEED_T = 2 * get_global_transform().get_origin().y * delta;

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
