#pragma once




#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <String.hpp>
#include <Mesh.hpp>
#include <cstdlib>
#include <ctime>
#include <random>
#include <Math.hpp>


namespace godot {
    class Structure : public StaticBody {
        GODOT_CLASS(Structure, StaticBody)
    private:

    public:

        Structure();
        ~Structure();

        bool Clickable;

        bool MenuVisible;
        bool PanelsOn{ false };

        static void _register_methods();
        virtual void _init();
        virtual void _process(float delta);
        virtual void _input(InputEvent* e);
        virtual void _ready();
        void _on_Area_mouse_entered();
        void _on_Area_mouse_exited();

        String object_type;

        virtual String get_object_type() { return "Structure"; };
        virtual String get_type() { return this->get_object_type(); };
        virtual void set_object_type(String type) { object_type = type; };

        virtual String get_main_type() { return "Default"; };
        virtual void show_menu();
        virtual String get_object_info();
        bool hover_animation_active{ false };
        int hover_animation_counter{ 0 };
        Vector3 object_scale;

        virtual void simulate_step(double days);
        bool updatable{ false };
        //void test_update();

        real_t MenuSize{ 300 };
        real_t InfoBoxWidth{ 300 };

        Vector3 get_position();

        bool is_other_structure_within_distance(Vector3, double);

        double employment = 0; // approximate number of employees per building
        double cost = 0; // cost in euros to build a new building
        double energyUse = 0; //amount of energy used by the building in kWh
        double energyperDay = 0; //amount of energy used by the building in kWh
        double maintenance = 0; //maintenace and working cost in euros
        double averageWage = 0; //average wage of the people in the building
        double CO2Emission{ 1 }; // kg of CO2 emitted
        double buildingTime = 0; // years needed to build a new building
        double satisfaction{ 1 }; // on scale of 10
        double environmentalCost = 0; // environmental and health costs in euros 
        double age = 0; //age of each particular object in days

        // The following will be city-wide counters that will be updated every day : 
        // income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply

        // income is the total wage (GDP), population is the population of the whole city, numberOfEmployees the total employed people,  
        // carbonEmission the total CO2, energyDemand the sum of all energy needed and energySupply the maximum production capacity

        // It would be great if someone could write here what variable we have to call to update each
        // income : we didn't take it into account for the moment
        // population : we don't have that, thought you would decide about it
        // numberOfEmployees: employment
        // carbonEmission : CO2Emission
        // energyDemand : that corresponds to energyUse for each class
        // energySupply : that corresponds to energyOutput for each object producing it in the energy class 

        // sim team will program these counters to grab the correct attributes within all structures
        // if needed within any object they will be passed on by the city object
        // don't use these specific variable names inside the structures pls or we will all get confused    

        double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

        
        // POLICIES (need to not be functions !) :

        // Changing the coal power plant efficiency (constructor creates subcritical plant of 38% efficiency) :
        double efficiency_supercritical = 0; // set to false when equals 0, true when given 1
                                          // improve efficiency to supercritical type of plant (42% energy converted to electricity)
        double efficiency_cogeneration = 0; // set to false when equals 0, true when given 1
                                          //improve efficiency to cogeneration type of plant (47% energy converted to electricity)

        //Law prohibiting coal power plants :
        double coal_prohibited = 0; //set to false when equals 0, true when given 1

        //Law prohibiting nuclear power plants :
        double nuclear_prohibited = 0; //set to false when equals 0, true when given 1
        
        //Law imposing maximum amount of CO2 emitted for each factory per day (Goods factories class), the imput value is in kg per day and per factory :
        double maximum_CO2 = 0; //default when there is no policy, should be between 0-42 kg when policy is implemented

        //Subsidies to "green" factories, so those which have a low emission of harmful chemicals and heavy metals : 
        double subsidy_green = 0; //default when policy is not on, should be between 1000 - 100 000 euros per factory

        //Subsidies for helping households, buildings to install solar panels (these help poorer households to have less environmental impact)
        
        
        double panel_subsidies;
        // range from 0 to solarCost


        //could be changed to a function that returns the amount of money the subsididy gives which will be compared to an income to decide if solar panels can be installed

         //Subsidies for helping households, buildings to install rooftop wind turbines  (these help poorer households to have less environmental impact)
        
        double wind_turbine_subsidies;
        
        // range from 0 to windCost

        //could be changed to a function thatreturns the amount of money the subsididy gives which will be compared to an income to decide if wind turbines can be installed

         //Subsidies for helping households, buildings to change their windows to double glazing (these help poorer households to have less environmental impact)
        
        double double_glazing_subsidies;
        //range from 0 to 1000

        //could be changed to a function that returns the amount of money the subsididy gives which will be compared to an income to decide if it is possible
        
        //law prohibiting pesticides
        double pesticideProhibited; // 0 if pestices are allowed, 1 if pesticides are prohibited
        //law prohibiting GMO
		double GMOProhibited; // 0 if GMO is allowed, 1 if pesticides GMO prohibited
        //law prohibiting fertilizer
		double fertilizerProhibited; // 0 if fertilizers are allowed, 1 if fertilizers are prohibited
        /*
        //Ads to promote vegetarianism, and reduce meat production in the city
        bool ad_vegetarian(); //not sure what form ads should take */

        /* SOLAR PANEL VARIABLES (add randomization for your own use)
        They describe the characteristics of one single average solar panel, taking as a model the most used one : The Canadian Solar
        CS3U-345P model of 1x2m in size.
        */
        double solarSatisfaction = 9; //ratings of these solar panels by surveys
        double solarCost = 450; //cost in euros for one new solar panel (product and installation)
        double solarEnergy = 7.45; //average kWh produced per day
        double solarLifetime = 9125; //years of energy production warranty
        // I think this solarLifeTime variable should be in days i.e. 9125 instead of 25. Can be changed back if you don't agree.



        /* ROOFTOP WIND TURBINE VARIABLES (add randomization for your own use)
        They describe the characteristics of one single average rooftop turbine
        */

        double windSatisfaction = 6; //public opinion on rooftop wind turbines
        double windCost = 800; //cost in euros for a new turbine with its tower
        double windEnergy = 1.1; //average kWh produced per day
        double windLifetime = 5; // years of energy production warranty


        virtual double get_maintenance();
        virtual void set_maintenance(double maintenance);
        virtual double get_age();
        virtual void set_age(double age);
        virtual double get_averageWage();
        virtual void set_averageWage(double averageWage);
        virtual double get_building_time();
        virtual void set_building_time(double buildingTime);
        virtual double get_cost();
        virtual void set_cost(double cost);
        virtual double get_employment();
        virtual void set_employment(double employment);
        virtual void set_energyuse(double energyUse);
        virtual double get_energyuse();
        virtual void set_energyperDay(double energyperDay);
        virtual double get_energyperDay();
        virtual double get_co2emissions();
        virtual void set_co2emissions(double);
        virtual double get_satisfaction();
        virtual void set_satisfaction(double sat);
        virtual double get_environmental_cost();
        virtual void set_environmental_cost(double environmentalCost);
    };
}