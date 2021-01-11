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

namespace godot {
	class Player : public KinematicBody {
		GODOT_CLASS(Player, KinematicBody)
	private:
		Vector3 motion;
		Vector3 rotation;
		Vector2 mouse_p;

		double SPEED_T = 1;
		const double SPEED_R = 20;
		const double VSPEED_INVERSE = 512;
		const double VSPEED_POWER = 1.2;

		const double MaxCameraAngle{ 50 };
		const double MinCameraAngle{ 20 };
		double CameraAngleDeg;
		const Vector3 StartPosition{ Vector3(200, 100, 200) };
		const Vector3 StartRotation{ Vector3(0, 0, 0) };
		const double MaxHeight{ 300 };
		const double MinHeight{ 20 };
		void update_camera_angle();

		//WorldEnvironment* Env;
		
		bool movable{ true };
		

	public:
		
		int counter;
		
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _physics_process(float delta);
		void _input(InputEvent* e);
		void _ready();

		Player();
		~Player();

		void UpdateMotionFromInput(float delta);
		void UpdateRotationFromInput(InputEventMouseMotion* e);
		void ChangeMouseMode();
	};
}
