#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <String.hpp>


namespace godot {
	class Restaurant : public Production {
		GODOT_SUBCLASS(Restaurant, Production)

	public:

		Restaurant();

		String class_name();


		int income;

		void change_income_by_percent(float);
		

		bool restaurantStatus;    //True if open, False if closed
		int restaurantType;
		double energyUsePerSize;
		double diningSize;
		double averageWage;
		bool firstYearShock;

		void simulate_step(double days);
	};
}

