#pragma once

#include "obj.h"
#include <core/Godot.hpp>

namespace godot {
    class Housing : public Structure {
        GODOT_SUBCLASS(Housing, Structure)

    public:
        Housing();
        ~Housing();

		static void _register_methods();
		virtual void _ready();
		virtual void _process(float delta);

		virtual String get_object_type() { return "Housing"; };
		String get_main_type() { return "Housing"; };

		virtual String get_object_info();

		virtual void simulate_step(double days);

		double panel_probability{ 0.0 };
		double double_glazing_probability{ 0.0 };
		double roof_wind_turbines_probability{ 0.0 };

		void panel_added_probability();
		void double_glazing_added_probability();
		void roof_wind_turbines_added_probability();
		
		int housingIncome;
		bool doubleGlazingOn{ false };
		bool rooftopWindTurbineOn{ false };;
		int windowCost = 200;
		int windowNumber;
		int solarPanelCost = 1500;
		int numberOfInhabitants = 0;
		int maxIncome=333; //max income per day, should be defined by sim team
		int minIncome=40; // min income per day, I put values for now so that it compiles


		int solarPanelAge = 0;
		int doubleGlazingAge = 0;
		int rooftopWindTurbineAge = 0;
		double normalGenerator(double mean, double stdDev);

		double set_max_income();
		double set_min_income();
		
		double get_min_income();
		double get_max_income();
		
		double get_solar_panel_age();
		double get_double_glazing_age();
		double get_satisfaction();
		
		int get_inhabitants();
		void set_inhabitants(int);


		int doubleGlazingSatisfaction = 5;

	private:
		double turnSpeed = 0.5;
		double rot = 0;

    };

	class House : public Housing {
		GODOT_SUBCLASS(House, Housing)
	public:
		House();
		~House();
		virtual String get_object_type() { return "House"; };

		static void _register_methods();
		virtual void _ready();

		void set_houseType(int type);
		int get_houseType();

		virtual void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		void house_type();
	
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		int houseType;

		void house_initialize();

		
	};

	class Building : public Housing {
		GODOT_SUBCLASS(Building, Housing)
	public:
		Building();
		~Building();

		
		static void _register_methods();
		virtual void _ready();

		virtual String get_object_type() { return "Building"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		
		
		int buildingType;
		
	};
}

