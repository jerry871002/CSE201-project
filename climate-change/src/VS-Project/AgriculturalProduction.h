#pragma once

#include "City.h"
#include "obj.h"
#include "Production.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot {
	class AgriculturalProduction : public Production {
		GODOT_SUBCLASS(AgriculturalProduction, Production)
	public:
		virtual String class_name();
		AgriculturalProduction();
		~AgriculturalProduction();
		//void test();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void simulate_step(double days);
		double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		double waterConsumption; // per day 
		int agricultureType; // 0 - wheat, 1 - vegetables, 2 - meat
		bool pesticide; // true if pesticides are used
		bool GMO; // true if GMO is used
		bool fertilizer; // true if fertilizer is used
		double size; //size of the field in m^2
		double fertility; //per m^2 output
		double production; //how output much per day
	};
}
