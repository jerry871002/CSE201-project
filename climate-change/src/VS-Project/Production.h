#pragma once

#include "obj.h"
#include <core/Godot.hpp>

/// <summary>
/// SUPER CLASS PRODUCTION
/// </summary>

namespace godot {
	class Production : public Structure {
		GODOT_SUBCLASS(Production, Structure)

	public:
		virtual String class_name();
		Production();
		~Production();

		virtual String get_object_type() { return "Production"; };
		String get_main_type() { return "Production"; };

		static void _register_methods();
		virtual String get_object_info();

		bool subsidy = false; //needed to check whether the subsidy is given to the factory in the info display

		// Harmful chemicals emitted :

		double SO2Emission = 0; //kg of sulfure dioxide emitted 
		double NOxEmission = 0; //kg of nitrogen oxides emitted 
		double PMEmission = 0; //particulate matter emitted
		double NH3Emission = 0; //kg of ammonia emitted
		double VOCsEmission = 0; // kg of volatile organic compounds emitted

		//Heavy metals released (in kg) :

		double mercuryEmission = 0;
		double arsenicEmission = 0;
		double cadmiumEmission = 0;
		double leadEmission = 0;
		double nickelEmission = 0;

		//All harmful chemicals emitted which have a negative impact on the environment 
		virtual double get_PMEemission();
		virtual double get_NH3emission();
		virtual double get_SO2emission();
		virtual double get_NOxemission();
		virtual double get_VOCsemission();

		//All heavy metals emitted which have a negative impact on the environment 
		virtual double get_mercury_emission();
		virtual double get_arsenic_emission();
		virtual double get_lead_emission();
		virtual double get_cadmium_emission();
		virtual double get_nickel_emission();

	};

	/// <summary>
	/// AGRICULTURAL PRODUCTION
	/// </summary>

	class AgriculturalProduction : public Production {
		GODOT_SUBCLASS(AgriculturalProduction, Production)
	public:
		virtual String class_name();
		virtual String get_object_type() { return "Agricultural Production"; };

		AgriculturalProduction();
		~AgriculturalProduction();
        AgriculturalProduction(int type);
		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		//void test();
		double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
		void agriculture_type(int type);
		//prohibites or authorises pesticides with inpit from user
		void prohibite_pesticide();
		//prohibites or authorises GMO with inpit from user
		void prohibite_GMO();
		//prohibites or authorises fertilizer with inpit from user
		void prohibite_fertilizer();
		double waterConsumption; // per day 
		int agricultureType; // 0 - wheat, 1 - meat, 2 - vegetables
		double requiredLand; //size of the field in km^2
		double fertility; //per km^2 output in kg
		double production; //how much output per day in kg
	};

	/// <summary>
	/// GOODS FACTORIES
	/// </summary>
	
	/* This class models the manufacturing and industry secotrs.
	Its purpose is to simulate the factories/companies in enough detail in order to see a change in pollution, employment and energy usage given policies 
	related to it (taxes, subsidies, charges, prohibitions...) */

	class GoodsFactories : public Production {
		GODOT_SUBCLASS(GoodsFactories, Production)
	public:
		virtual String class_name();
		GoodsFactories();
		~GoodsFactories();
		virtual String get_object_type() { return "Goods Factory"; };

		double factoryGDP; //Total money made by the factory over one year in euros

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// SERVICES
	/// </summary>

	/* This class models to the following sectors : education and health services, trade, professional and business services, leisure 
	and hospitality, financial activities, public administration, information and other services.
	Its purpose is to model primarly the employment, emissions and energy usage of this sector in an overall way */

	class Services : public Production {
		GODOT_SUBCLASS(Services, Production)
	public:
		virtual String class_name();
		Services();
		~Services();
		virtual String get_object_type() { return "Services"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

}

