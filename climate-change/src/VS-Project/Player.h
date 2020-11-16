#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>

namespace godot {
	class Player : public KinematicBody {
		GODOT_CLASS(Player, KinematicBody)
	private:
		Vector3 motion;
		Vector3 rotation;
		const double SPEED_T = 3;
		const double SPEED_R = 20;
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
	};
}
