#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <ClippedCamera.hpp>

namespace godot {
	class Player : public KinematicBody {
		GODOT_CLASS(Player, KinematicBody)
	private:
		Vector3 motion;
		Vector3 rotation;
		
		const double SPEED_T = 0.1;
		const double SPEED_R = 20;
		const double VSPEED_INVERSE = 512;
		const double VSPEED_POWER = 1.2;

		const double CameraAngleDeg = 15;
		const double MaxHeight{ 30 };
		const double MinHeight{ 3 };


	public:
		
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		

		Player();
		~Player();

		void UpdateMotionFromInput();
		void UpdateRotationFromInput(InputEventMouseMotion* e);
		void ChangeMouseMode();
	};
}
