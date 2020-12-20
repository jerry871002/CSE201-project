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

		// Harmful chemicals emitted :

		double SO2Emission = 0; //kg of sulfure dioxide emitted 
		double NOxEmission = 0; //kg of nitrogen oxides emitted 
		double PMEmission = 0; //particulate matter emitted
		double O3Emission = 0; //ozone formed from emitted chemicals
		double COEmission = 0; //kg of carbon monoxide emitted
		double NH3Emission = 0; //kg of ammonia emitted
		double benzeneEmission = 0; //kg of benzene released
		double VOCsEmission; // kg of volatile organic compounds emitted
		double CFCsEmission; // kg of Chlorofluorocarbons emitted

		//Heavy metals released (in kg) :

		double mercuryEmission = 0;
		double arsenicEmission = 0;
		double cadmiumEmission = 0;
		double leadEmission = 0;
		double nickelEmission = 0;
	};

	/// <summary>
	/// AGRICULTURAL PRODUCTION
	/// </summary>

	class AgriculturalProduction : public Production {
		GODOT_SUBCLASS(AgriculturalProduction, Production)
	public:
		virtual String class_name();
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
		double waterConsumption; // per day 
		int agricultureType; // 0 - wheat, 1 - meat, 2 - vegetables
		bool pesticide; // true if pesticides are used
		bool GMO; // true if GMO is used
		bool fertilizer; // true if fertilizer is used
		double requiredLand; //size of the field in km^2
		double fertility; //per km^2 output
		double production; //how much output per day




		
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

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// RECYCLING FACTORIES
	/// </summary>

	class RecyclingFactories : public Production {
		GODOT_SUBCLASS(RecyclingFactories, Production)
	public:
		virtual String class_name();
		RecyclingFactories();
		~RecyclingFactories();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};
}