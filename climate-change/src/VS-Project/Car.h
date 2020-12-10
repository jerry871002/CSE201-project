#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <Mesh.hpp>
#include <Area.hpp>
#include <KinematicCollision.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <ClippedCamera.hpp>
#include <WorldEnvironment.hpp>
#include <Environment.hpp>
#include "City.h"

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
		Vector3 prevPosition = Vector3(0, 0, 0);
		double Acc = 0.5;
		Vector3 center;
		City* myCity;
		int traffic[2][3][4][3] = { {{{0, 1, 0},{0, 0, 1},{0, 0, 1},{0, 0, 1}},		{{0, 0, 1},{0, 1, 0},{0, 0, 1},{0, 0, 1}},		{ {0, 1, 1},{0, 1, 1},{0, 0, 1},{0, 0, 1}}},
									{ {{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 1, 0}}	,		{{0, 0, 1},{0, 0, 1},{1, 1, 0},{0, 1, 1}},		 { {0, 0, 1},{0, 0, 1},{0, 1, 1},{0, 0, 1}}} };
	public:

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _ready();
		void _physics_process(float delta);;
		void turn(int dir, float delta);
		void straight(float delta);
		int get_direction(Vector3 pos, double rot);

		Car();
		~Car();
	};
}
