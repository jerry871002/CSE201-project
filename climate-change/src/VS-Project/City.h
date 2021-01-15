#pragma once

#include "obj.h"
#include "Shop.h"
#include "Housing.h"
#include "Energy.h"
#include "Production.h"
#include "edit_text_files.h"

#include <core/Godot.hpp>
#include <Spatial.hpp>

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <Array.hpp>

#include <StaticBody.hpp>

#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>


using namespace std;


/* use main fct defined in City.cpp for mac
current test fct using main_loop.cpp on mac:
g++ -std=c++17 City.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64
then run:
./a.out
*/

namespace godot {
    
    const int citysize{ 40 };

    class Transport;

    class City : public Spatial {

        GODOT_CLASS(City, Spatial)

    public:

        // CONSTRUCTOR, DESTRUCTOR

        City();
        ~City();

        // GODOT FUNCTIONAL METHODS

        static void _register_methods();
        void _init();
        void _process(float);
        void _physics_process(float);
        void _input(InputEvent*);
        void _ready();

        // SIGNALS

        void _on_Menu_pressed(String Name);

        void _on_Validate_pressed();
        void _on_Game_Speed_changed();
        void _on_ResetButton_pressed();
        void _on_Reset_confirmed();
        void _on_Reset_cancelled();
        void _on_ExitButton_pressed();
		void _on_3dButton_pressed();
        void _on_Exit_confirmed();
        void _on_Exit_cancelled();
        void _on_TransportMenuButton_pressed();
        void _on_GraphsButton_pressed();
        void _on_GraphsExit_pressed();
        bool MenuVisibility;



        // INITIAL GRAPHICAL SETUP

        
        void initialize_stats();
        void generate_initial_city_graphics();
        void set_initial_visible_components();

        // TIME AND OVERALL SIMULATION

        bool loopbegin {false}; // loop begin in order to set counters to 0 again
        float time_speed = 1; // 1 for regular speed (1 in-game day per second)
        double simulation_counter{ 0 }; // counter used to cast per-frame processing to a time_speed related frequency
        double date_counter{ 0 };
        int day_tick; // keeps track of the in-game days
        int days_since_last_simulation;
        int daycount = 0; //keeps track of the in-game days per year


        Array statsCarbonEmission{}; //written stats for carbon emissions

        double HousingCO2;
        double ShopsCO2;
        double ProductionCO2;
        double EnergyCO2;
        Array statsCarbonEmissionHousing{};
        Array statsCarbonEmissionProduction{};
        Array statsCarbonEmissionEnergy{};
        Array statsCarbonEmissionShops{};

        Array statsEnvironmentalCost{}; //written stats for environmental cost
        Array statsIncome{}; //written stats for GDP
        Array statsEnergy{}; //written stats for energy demand and supply
        Array statsUnemployment{}; //written stats for population - number of employees
        Array statsTotalSatisfaction{}; //written stats for population - number of employees
        Array statsPopulation{}; //written stats for population



        void update_date();
        // not needed anymore: String return_game_date2();  returns the date :day/month/year as a string
        // date leap years implementation
        string return_word_date(int days); // returns the date from days :day/month/year as a string with words
        String return_word_date_godot(); // returns date from day_tick in godot string
        string return_number_date(int day, int month, int year); // returns the date :day/month/year as a string with numbers
        //void simulation(); // updates all the stats and calls the simulation for the buildings

        int rolling_simulation_counter{ -1 };

        

        void change_pie_chart(int value, NodePath name, bool isPositive);
		void change_pie_label(int value, NodePath name);

        bool ClickActive{ false };

        //TRAFFIC

        //const int citysize = 26;

        int sizeOfCity = citysize; // buildings are placed only on a square sizeOfCity * sizeOfCity
        int positionOfBuildings[citysize][citysize] = { 0 }; // sets  everything to non-existing for the traffic array 

        // following functions handle adding structures to the city, takes a position and the required scene
        void add_shop(Vector3 pos, Ref<PackedScene> scene); // adds a shop and updates the traffic array with the shop
        void add_house(Vector3 pos, Ref<PackedScene> scene); // adds a house and updates the traffic array with the shop
        void add_energy(Vector3 pos, Ref<PackedScene> scene);
        void add_production(Vector3 pos, Ref<PackedScene> scene);


        //int* building_coordinates_identification(int x, int y, int number);//returns coordinates of a center for the upper left square of any buiding  
        void update_traffic(int x, int y, bool newBuilding, int number);// updates the traffic array 
        void traffic_preparation(double x, double y); // identifies  if the building is 2x2 or 1x1 and calls the update_traffic function
        
        // ARRAYS CONTAINING ALL ACTIVE ELEMENTS

        std::vector<Structure*>::iterator structures_iterator;
        std::vector<Structure*> all_structures;

        std::vector<Shop*> all_shops;
        std::vector<Housing*> all_houses;
        std::vector<Energy*> all_energies;
        std::vector<Production*> all_production;
        std::vector<Transport*> all_transports;



        // trees attributes
        std::vector<Structure*> trees_vector;
        std::vector<Structure*>::iterator trees_iterator;
        int houses_with_trees{ 0 };



        String active_button;
        void implement_policies(double);
        bool notification_active{ false };
        int notification_counter{ 0 };
        int notification_timeout{ 180 };
        void trigger_notification(String);
        String get_button_info_text();
        void hide_menus();

        void add_car(Vector3 pos = Vector3(24 * 15 ,0, 24 * 15));

        void write_stat_history_to_file();    //writes all the stats to a file so that the interface team can make graphs 


        // getter functions for city indices
        double return_income();
        double return_numberOfEmployees();
        double return_carbonEmission();
        double return_energyDemand();
        double return_energySupply();
        double return_environmentalCost();
        double return_totalSatisfaction();
		int value_pie_chart_C02(int,int);

        float calculate_building_prob(float, float, float, double);

        //computes probailities for each type of transport that this type will be added
        void transport_to_add(); // updates the array of all missing cars - missing_car_quantities
        void remove_type_car(int type); //reduces by 1 current_car_quantities[type] 
        int most_missing_type();

        /* we can keep these vairables as floats as long as each StaticBody only computes the ADDITIONAL AMOUNT of energy, income etc.
        and we cannot have different consequences for diff sectors (e.g. housing, production and industry) and thus implement different policies for each*/

        /* other idea: implement arrays based on sector (housing, production, infrastructure), compute additional amounts but differences between sector
        (other more radical idea: array with all buildings, not necessarily needed?)
        float income_array[3];
        float population_array[3];
        float employed_array[3];
        float carbon_array[3];
        float energyDemand_array[3];
        float energySupply_array[3];
        */
        //policies for transport
        //tax on car consumption
        double fuelTax; // value per liter of fuel
        // tax on car weight
        double weightTax; // value per ton of car
        //bike subsidy
        double bikeSubsidy;
        //electic car subsidy
        double electricCarSubsidy;
        //bus subsidy
        double busSubsidy;
        //cars' prohibition on certain days
        double carProhibition; //number of days on which cars are prohibited

        //to generate random numbers
        double normalGenerator(double mean, double stdDev);

        double return_unemployment_rate();

    private:
        // city indices

        double income;
        int population;
        double numberOfEmployees;
        double carbonEmission;
        double energyDemand;
        double energySupply;
        double environmentalCost;
        int totalSatisfaction;


        double budget; //yearly budget that can be used t proomote the policies
                       // updated in the undate_time function

        //probability that a certain type of car will be added
        double airQuality;
        int numberOfHouses;
        int current_car_quantities[8] = { 0 }; //current quantities of cars by the type in the city 
        int missing_car_quantities[8] = { 0 }; //updated in the update_date function every month


        // method for power plants
        int workingPower{ 0 };
        int get_workingPower();
        void set_workingPower(int);

        
        const Ref<PackedScene> RestaurantScene = ResourceLoader::get_singleton()->load("res://Resources/Restaurant.tscn", "PackedScene");
        const Ref<PackedScene> ShopScene = ResourceLoader::get_singleton()->load("res://Resources/Shop.tscn", "PackedScene");
        const Ref<PackedScene> MallScene = ResourceLoader::get_singleton()->load("res://Resources/Mall.tscn", "PackedScene");


        const Ref<PackedScene> OldCarScene = ResourceLoader::get_singleton()->load("res://Resources/Bugatti.tscn", "PackedScene");
        const Ref<PackedScene> SportCarScene = ResourceLoader::get_singleton()->load("res://Resources/Chiron.tscn", "PackedScene");
        const Ref<PackedScene> MotoScene = ResourceLoader::get_singleton()->load("res://Resources/Moto.tscn", "PackedScene");
        const Ref<PackedScene> BusScene = ResourceLoader::get_singleton()->load("res://Resources/Bus.tscn", "PackedScene");
        const Ref<PackedScene> AmericanCarScene = ResourceLoader::get_singleton()->load("res://Resources/Raptor.tscn", "PackedScene");
        const Ref<PackedScene> BikeScene = ResourceLoader::get_singleton()->load("res://Resources/Bike.tscn", "PackedScene");
        const Ref<PackedScene> ElectricCarScene = ResourceLoader::get_singleton()->load("res://Resources/Cybertruck.tscn", "PackedScene");
        const Ref<PackedScene> NormalCarScene = ResourceLoader::get_singleton()->load("res://Resources/Clio.tscn", "PackedScene");

        
        const Ref<PackedScene> WindmillScene = ResourceLoader::get_singleton()->load("res://Resources/WindMill.tscn", "PackedScene");
        const Ref<PackedScene> LowHouseScene = ResourceLoader::get_singleton()->load("res://Resources/LowHouse.tscn", "PackedScene");
        const Ref<PackedScene> HighHouseScene = ResourceLoader::get_singleton()->load("res://Resources/HighHouse.tscn", "PackedScene");
        const Ref<PackedScene> CoalPowerPlantScene = ResourceLoader::get_singleton()->load("res://Resources/CoalPowerPlant.tscn", "PackedScene"); 
        const Ref<PackedScene> GeothermalPowerPlantScene = ResourceLoader::get_singleton()->load("res://Resources/GeothermalPowerPlant.tscn", "PackedScene");
        const Ref<PackedScene> NuclearPowerPlantScene = ResourceLoader::get_singleton()->load("res://Resources/NuclearPowerPlant.tscn", "PackedScene");
        const Ref<PackedScene> FactoryScene = ResourceLoader::get_singleton()->load("res://Resources/Factory.tscn", "PackedScene");
        const Ref<PackedScene> PigsPastureScene = ResourceLoader::get_singleton()->load("res://Resources/PasturePigs.tscn", "PackedScene");
        const Ref<PackedScene> SheepPastureScene = ResourceLoader::get_singleton()->load("res://Resources/PastureSheeps.tscn", "PackedScene");
        const Ref<PackedScene> FieldScene = ResourceLoader::get_singleton()->load("res://Resources/Field.tscn", "PackedScene");
        const Ref<PackedScene> BuildingScene = ResourceLoader::get_singleton()->load("res://Resources/Building.tscn", "PackedScene");



        const Ref<PackedScene> PDScene = ResourceLoader::get_singleton()->load("res://Resources/Pedestrian.tscn", "PackedScene");


    };
}