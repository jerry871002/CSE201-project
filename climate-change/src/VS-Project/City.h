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

#include <StaticBody.hpp>

#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;


/* use main fct defined in City.cpp for mac
current test fct using main_loop.cpp on mac:
g++ -std=c++17 City.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64
then run:
./a.out
*/

namespace godot {

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

        void _on_MenuShop_pressed(String Name);
        void _on_Validate_pressed();
        void _on_Game_Speed_changed();
        void _on_ResetButton_pressed();
        void _on_Reset_confirmed();
        void _on_Reset_cancelled();
        void _on_ExitButton_pressed();
        void _on_Exit_confirmed();
        void _on_Exit_cancelled();

        // INITIAL GRAPHICAL SETUP

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
        double stat = 0; //stat example
        double stats[10][366]; //stats array
        void update_date();     
        // not needed anymore: String return_game_date2();  returns the date :day/month/year as a string
        // date leap years implementation
        string return_word_date(int days); // returns the date from days :day/month/year as a string with words
        String return_word_date_godot(); // returns date from day_tick in godot string
        string return_number_date(int day, int month, int year); // returns the date :day/month/year as a string with numbers
        //void simulation(); // updates all the stats and calls the simulation for the buildings

        int rolling_simulation_counter{ -1 };

        void simulation_shops();
        void simulation_housing();
        void simulation_energy();
        void simulation_production();
        void simulation_transport();

        void change_pie_chart(int value, NodePath name, bool isPositive);

        bool ClickActive{ false };

        //TRAFFIC
        int sizeOfCity = 10; // buildings are placed only on a square sizeOfCity * sizeOfCity
        int positionOfBuildings[10][10] = { 0 }; // sets  everything to non-existing for the traffic array 

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

        String active_button;
        void implement_shop_policies(double);
        bool notification_active{ false };
        int notification_counter{ 0 };
        int notification_timeout{ 180 };
        void trigger_notification(String);
        String get_button_info_text();

        void add_car();
        void add_car(Vector3);

        void write_stat_history_to_file();    //writes all the stats to a file so that the interface team can make graphs 


        // getter functions for city indices
        double return_income();
        double return_numberOfEmployees();
        double return_carbonEmission();
        double return_energyDemand();
        double return_energySupply();
        double return_environmentalCost();
        double return_totalSatisfaction();
		int value_pie_chart(int);

        float calculate_building_prob(float, float, float, double);

        //computes probailities for each type of transport that this type will be added
        int transport_to_add();
        void remove_type_car(int type);

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



    private:
        // city indices
        double income;
        double population;
        double numberOfEmployees;
        double carbonEmission;
        double energyDemand;
        double energySupply;
        double environmentalCost;
        int totalSatisfaction;


        double budget; //yearly budget that can be used t proomote the policies
                       // updated in the undate_time function
        int citysize;
        //probability that a certain type of car will be added
        double probabilityElectricCar, probabilityBigCar, probabilityCar, probabilityCollectionCar;
        double probabilityBike, probabilityMotorcycle, probabilityBus, probabilitySportsCar;
        double airQuality;
        double* incomes;
        int incomesLen;
        int current_car_quantities[8] = { 0 };
        

        const Ref<PackedScene> RestaurantScene = ResourceLoader::get_singleton()->load("res://Resources/Restaurant.tscn", "PackedScene");
        const Ref<PackedScene> ShopScene = ResourceLoader::get_singleton()->load("res://Resources/Shop.tscn", "PackedScene");
        const Ref<PackedScene> MallScene = ResourceLoader::get_singleton()->load("res://Resources/Mall.tscn", "PackedScene");


        const Ref<PackedScene> BugattiScene = ResourceLoader::get_singleton()->load("res://Resources/Bugatti.tscn", "PackedScene");
        const Ref<PackedScene> ChironScene = ResourceLoader::get_singleton()->load("res://Resources/Chiron.tscn", "PackedScene");
        const Ref<PackedScene> MotoScene = ResourceLoader::get_singleton()->load("res://Resources/Moto.tscn", "PackedScene");


        const Ref<PackedScene> WindmillScene = ResourceLoader::get_singleton()->load("res://Resources/Windmill.tscn", "PackedScene");
        const Ref<PackedScene> LowHouseScene = ResourceLoader::get_singleton()->load("res://Resources/LowHouse.tscn", "PackedScene");
        const Ref<PackedScene> HighHouseScene = ResourceLoader::get_singleton()->load("res://Resources/HighHouse.tscn", "PackedScene");
        const Ref<PackedScene> ParkScene = ResourceLoader::get_singleton()->load("res://Resources/Park.tscn", "PackedScene");
        const Ref<PackedScene> CoalPowerPlantScene = ResourceLoader::get_singleton()->load("res://Resources/CoalPowerPlant.tscn", "PackedScene");
        const Ref<PackedScene> NuclearPowerPlantScene = ResourceLoader::get_singleton()->load("res://Resources/NuclearPowerPlant.tscn", "PackedScene");
        const Ref<PackedScene> FactoryScene = ResourceLoader::get_singleton()->load("res://Resources/Factory.tscn", "PackedScene");
        const Ref<PackedScene> PigsPastureScene = ResourceLoader::get_singleton()->load("res://Resources/PasturePigs.tscn", "PackedScene");
        const Ref<PackedScene> SheepPastureScene = ResourceLoader::get_singleton()->load("res://Resources/PastureSheeps.tscn", "PackedScene");
        const Ref<PackedScene> FieldScene = ResourceLoader::get_singleton()->load("res://Resources/Field.tscn", "PackedScene");
        const Ref<PackedScene> BuildingScene = ResourceLoader::get_singleton()->load("res://Resources/Building.tscn", "PackedScene");



        const Ref<PackedScene> PDScene = ResourceLoader::get_singleton()->load("res://Resources/Pedestrian.tscn", "PackedScene");


    };
}