#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <Mesh.hpp>

namespace godot {
	class Car : public KinematicBody {
		GODOT_CLASS(Car, KinematicBody)
	private:
		Vector3 motion;
		double position;
		double rot;
		double SPEED_T = 0.5;
		int Turn_R = 4;
		int dir;

		double Acc = 1;
		Vector3 center;

	public:

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _ready();
		void _physics_process(float delta);
		void turn(int dir, float delta);
		void straight(float delta);


		Car();
		~Car();

	};
}
