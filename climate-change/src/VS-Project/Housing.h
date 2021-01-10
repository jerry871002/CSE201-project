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

		virtual String get_object_type() { return "Housing"; };
		String get_main_type() { return "Housing"; };

		virtual String get_object_info();

		virtual void simulate_step(double days);
		double panel_probability{ 0.75 };
		double double_glazing_probability{ 0.75 };
		void panel_added_probability();
		void double_glazing_added_probability();
		void roof_wind_turbines_added_probability();
		double roof_wind_turbines_probability{ 0.75 };
		int housingIncome;
		bool doubleGlazingOn{ false };
		bool rooftopWindTurbineOn{ false };;
		int windowCost = 200;
		int windowNumber;
		int solarPanelCost = 1500;
		double numberOfInhabitants = 0;
		int maxIncome=333; //max income per day, should be defined by sim team
		int minIncome=53; // min income per day, I put values for now so that it compiles
		int solarPanelAge = 0;
		int doubleGlazingAge = 0;
		int rooftopWindTurbineAge = 0;
		double set_max_income();
		double set_min_income();
		double get_max_income();
		double get_min_income();
		double get_solar_panel_age();
		double get_double_glazing_age();	
		int doubleGlazingSatisfaction = 5;


    };

	class House : public Housing {
		GODOT_SUBCLASS(House, Housing)
	public:
		House();
		~House();
		virtual String get_object_type() { return "House"; };

		virtual void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		bool solar_panel(); 
		bool double_glazing(); // improve insulation
		bool rooftop_wind_turbines();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		int houseType;
		double houseIncome = 0;
		
	};

	class Building : public Housing {
		GODOT_SUBCLASS(Building, Housing)
	public:
		Building();
		~Building();
		virtual String get_object_type() { return "Building"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		bool solar_panel(); 
		bool double_glazing(); // improve insulation
		bool rooftop_wind_turbines();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		
		int buildingType;
		double coOwnershipBudget = 0;
	};
}

// #pragma once

// #include "obj.h"
// #include <core/Godot.hpp>

// namespace godot {
//     class Housing : public Structure {
//         GODOT_SUBCLASS(Housing, Structure)

//     public:
//         Housing();
//         ~Housing();

// 		static void _register_methods();
// 		virtual void _ready();

// 		virtual String get_object_type() { return "Housing"; };
// 		String get_main_type() { return "Housing"; };

// 		virtual String get_object_info();

// 		virtual void simulate_step(double days);


// 		int windowCost = 200;
// 		int windowNumber;
// 		int solarPanelCost = 1500;
// 		double numberOfInhabitants = 0;
// 		int maxIncome=333; //max income per day, should be defined by sim team
// 		int minIncome=53; // min income per day, I put values for now so that it compiles
// 		int solarPanelAge = 0;
// 		int doubleGlazingAge = 0;
// 		int rooftopWindTurbineAge = 0;
// 		double set_max_income();
// 		double set_min_income();
// 		double get_max_income();
// 		double get_min_income();
// 		double get_solar_panel_age();
// 		double get_double_glazing_age();	


//     };

// 	class House : public Housing {
// 		GODOT_SUBCLASS(House, Housing)
// 	public:
// 		House();
// 		~House();
// 		virtual String get_object_type() { return "House"; };

// 		virtual void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
// 		bool solar_panel(); 
// 		bool double_glazing(); // improve insulation
// 		bool rooftop_wind_turbines();
		

// 		int houseType;
// 		double houseIncome = 0;
		
// 	};

// 	class Building : public Housing {
// 		GODOT_SUBCLASS(Building, Housing)
// 	public:
// 		Building();
// 		~Building();
// 		virtual String get_object_type() { return "Building"; };

// 		virtual void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
// 		bool solar_panel(); 
// 		bool double_glazing(); // improve insulation
// 		bool rooftop_wind_turbines();
		
// 		int buildingType;
// 		double coOwnershipBudget = 0;
// 	};
// }













// // #pragma once

// // #include "obj.h"
// // #include <core/Godot.hpp>



// // namespace godot {
// //     class Housing : public Structure {
// //         GODOT_SUBCLASS(Housing, Structure)

// //     public:
// // 		virtual String class_name();
// //         Housing();
// //         ~Housing();
// //     };

// // 	class House : public Housing {
// // 		GODOT_SUBCLASS(House, Housing)
// // 	public:
// // 		virtual String class_name();
// // 		House();
// // 		~House();

// // 		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)

// // 		double numberOfInhabitants = 0;
// // 		double houseIncome = 0;
// // 	};
// // }