#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <Mesh.hpp>
#include <Area.hpp>
#include <KinematicCollision.hpp>

namespace godot {
	class Car : public KinematicBody {
		GODOT_CLASS(Car, KinematicBody)
	private:
		Vector3 motion;
		double position;
		double rot;
		double SPEED_T;
		int Turn_R = 4;
		int dir;

		double Acc = 0.5;
		Vector3 center;

	public:

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _ready();
		void _physics_process(float delta);
		void _on_Area_area_entered();
		void turn(int dir, float delta);
		void straight(float delta);





		Car();
		~Car();

	};
}