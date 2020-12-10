#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <Mesh.hpp>
#include <Area.hpp>
#include <KinematicCollision.hpp>
<<<<<<< Updated upstream
=======
#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <ClippedCamera.hpp>
#include <WorldEnvironment.hpp>
#include <Environment.hpp>
#include "City.h"
>>>>>>> Stashed changes

namespace godot {
	class Car : public KinematicBody {
		GODOT_CLASS(Car, KinematicBody)
	private:
		Vector3 motion;
		double position;
		double rot;
		double SPEED_T ;
		int Turn_R = 4;
		int dir;

		double Acc = 0.5;
		Vector3 center;
<<<<<<< Updated upstream

=======
		City* myCity ;
		int traffic[2][3][4][3] = { {{{0, 1, 0},{0, 0, 1},{0, 0, 1},{0, 0, 1}},		{{0, 0, 1},{0, 1, 0},{0, 0, 1},{0, 0, 1}},		{ {0, 1, 1},{0, 1, 1},{0, 0, 1},{0, 0, 1}}},
									{ {{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 1, 0}}	,		{{0, 0, 1},{0, 0, 1},{1, 1, 0},{0, 1, 1}},		 { {0, 0, 1},{0, 0, 1},{0, 1, 1},{0, 0, 1}}} };
>>>>>>> Stashed changes
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
