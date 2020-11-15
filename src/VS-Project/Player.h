#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>

namespace godot {
	class Player : public KinematicBody {
		GODOT_CLASS(Player, KinematicBody)
	private:
		Vector3 motion;
	public:
		const int SPEED = 300;
		static void _register_methods();
		void _init();
		void _process(float delta);

		Player();
		~Player();

		void UpdateMotionFromInput();
	};
}
