#include "City.h"
#include "Transport.h"
#include "edit_text_files.h"

#include <Resource.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <PackedScene.hpp>
#include <Node.hpp>
#include <Input.hpp>
#include <Button.hpp>
#include <Viewport.hpp>
#include <HSlider.hpp>
#include <TextureProgress.hpp>
#include <Color.hpp>
#include <Label.hpp>
#include <DirectionalLight.hpp>
#include <WorldEnvironment.hpp>
#include <godot.hpp>

#include <PoolArrays.hpp>
#include <random>
#include <ctime>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

using namespace std;


int traffic_system[citysize][citysize][4][3] = { 0 }; //sets everything to non-existing for the traffic array : the first to things are coordinates of the building where  the car is
                 // the third coornidate indicates the side of the building and the forth one which way the car can turn



City::City() {

    income = 0;
    population = 0; //please don't change it, computed in 
    numberOfEmployees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
    environmentalCost = 0;
    totalSatisfaction = 10;

    time_speed = 1;

    //timer = 0;
    day_tick = 0;
    days_since_last_simulation = 0;

    // in order to find date
    daycount = 0;
    numberOfHouses = 0;

    // in order to write stats to csv files
    stat = 0;
	//policies for transport
    fuelTax = 0;
	weightTax = 0;
    bikeSubsidy = 0;
    electricCarSubsidy = 0;
    busSubsidy = 0;
    carProhibition = 0;
    srand((int)time(0));
}

City::~City()
{

};

int* return_date(int day_tick) {
    int static date[3];
    int Y = 1, M = 1, D = 1;
    int julian = (1461 * (Y + 4800 + (M - 14) / 12)) / 4 + (367 * (M - 2 - 12 / ((M - 14) / 12))) / 12 - (3 * ((Y + 4900 + (M - 14) / 12) / 100)) / 4 + D - 32075 + day_tick + 35;
    int gregorian = julian + 1401 + (int)((((int)(4 * day_tick + 274277) / 146097) * 3) / 4) - 38;
    int e = 4 * gregorian + 3;
    int h = 5 * ((int)(e % 1461) / 4) + 2;
    int day = ((int)(h % 153) / 5) + 1;
    int month = (((int)h / 153) + 2) % 12 + 1;
    int year = (int)(e / 1461) - 4716 + (int)((14 - month) / 12);
    date[0] = day;
    date[1] = month;
    date[2] = year;
    return date;
}

void City::_register_methods()
{
    register_method((char*)"_physics_process", &City::_physics_process);
    register_method((char*)"_physics_process", &City::_physics_process);
    register_method((char*)"_input", &City::_input);
    register_method((char*)"_ready", &City::_ready);
    register_method((char*)"_on_Menu_pressed", &City::_on_Menu_pressed);

    register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
    register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);
    register_method((char*)"_on_ResetButton_pressed", &City::_on_ResetButton_pressed);
    register_method((char*)"_on_Reset_confirmed", &City::_on_Reset_confirmed);
    register_method((char*)"_on_Reset_cancelled", &City::_on_Reset_cancelled);
    register_method((char*)"_on_ExitButton_pressed", &City::_on_ExitButton_pressed);
    register_method((char*)"_on_Exit_confirmed", &City::_on_Exit_confirmed);
    register_method((char*)"_on_Exit_cancelled", &City::_on_Exit_cancelled);
    register_method((char*)"_on_TransportMenuButton_pressed", &City::_on_TransportMenuButton_pressed);

    register_method((char*)"change_pie_chart", &City::change_pie_chart);
	register_method((char*)"change_pie_label", &City::change_pie_label);

    
    register_method((char*)"add_shop", &City::add_shop);
    register_method((char*)"add_energy", &City::add_energy);
    register_method((char*)"add_house", &City::add_house);
    register_method((char*)"add_production", &City::add_production);

    register_method((char*)"update_traffic", &City::update_traffic);
    register_method((char*)"traffic_preparation", &City::traffic_preparation);
    register_method((char*)"remove_type_car", &City::remove_type_car);

    register_property<City, float>("time_speed", &City::time_speed, 1.0);
    register_property<City, int>("day_tick", &City::day_tick, 0);
    register_property<City, double>("budget", &City::budget, 10000);

    //Transport policies methods :
    register_property<City, double>("fuelTax",&City::fuelTax, 0.0);
    register_property<City, double>("weightTax", &City::weightTax, 0.0);
    register_property<City, double>("bikeSubsidy", &City::bikeSubsidy,0.0);
    register_property<City, double>("electricCarSubsidy", &City::electricCarSubsidy,0.0);
    register_property<City, double>("busSubsidy", &City::busSubsidy, 0.0);
    register_property<City, double>("carProhibition", &City::carProhibition, 0.0);


    register_property<City, double>("income", &City::income, 0.0);
    register_property<City, int>("population", &City::population, 0);
    register_property<City, double>("numberOfEmployees", &City::numberOfEmployees, 0.0);
    register_property<City, double>("carbonEmission", &City::carbonEmission, 0.0);
    register_property<City, double>("energyDemand", &City::energyDemand, 0.0);
    register_property<City, double>("energySupply", &City::energySupply, 0.0);
    register_property<City, int>("totalSatisfaction", &City::totalSatisfaction, 0.0);

    //statistics:
    register_property<City, Array>("statsCarbonEmission", &City::statsCarbonEmission, {});
    register_property<City, Array>("statsIncome", &City::statsIncome, {});
    register_property<City, Array>("statsEnergy", &City::statsEnergy, {});
    register_property<City, Array>("statsUnemployment", &City::statsUnemployment, {});

};

void City::_init()
{

};

void City::_process(float)
{

};

template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}


/*
This function calls simulation() every second
`day_tick` contains the integer part of `delta_counter`
everytime the integer part of `delta_counter` changes
we update `day_tick` and execute simulation()
*/
void City::_physics_process(float delta) {

    // CALLED EVERY PHYSICS FRAME (60 FPS)
    // TO UPDATE TRANSPORT STATS
    /*
    if ((transportType != 6) && (transportType != 5) && (transportType != 4)) {
        satisfaction *=1- carProhibition / 7;
        energyUse *= 1-carProhibition / 7;
        CO2Emission *=1- carProhibition / 7;
    }
    */

   // initialize stats arrays :
   Array title{};
   title.push_back(to_godot_string("Date"));

   title.push_back(to_godot_string("Carbon Emissions ()"));
   statsCarbonEmission.push_back(title);

    if (bool(this->time_speed))
    {
        (*structures_iterator)->set("updatable", true);
        ++structures_iterator;
        if (structures_iterator == all_structures.end()) { structures_iterator = all_structures.begin(); };

        this->simulation_counter += (double)delta;

        this->date_counter += double(delta) * this->time_speed;
    }
    
    if (simulation_counter > 5)
    {   
        // CALLED EVERY 5 SECONDS

        add_car();

        (this->simulation_counter) -= 5;
        
        std::cout << "income = " << (double)(this->get("income")) << std::endl;
        std::cout << "population = " << (double)(this->get("population")) << std::endl;
        std::cout << "numberOfEmployees = " << (double)(this->get("numberOfEmployees")) << std::endl; 
        std::cout << "carbonEmission = " << (double)(this->get("carbonEmission")) << std::endl;
        std::cout << "energyDemand = " << (double)(this->get("energyDemand")) << std::endl;
        std::cout << "energySupply = " << (double)(this->get("energySupply")) << std::endl;
        std::cout << "environmentalCost = " << (double)(this->get("environmentalCost")) << std::endl;
        
    }

    if (this->date_counter > 1)
    {
        // CALLED EVERY GAME DAY

        write_stat_history_to_file();
        
        /*
        for (int i = 0; i < statsCarbonEmission.size(); ++i) {
            Godot::print(String(statsCarbonEmission[i]));
        }
        Godot::print("Array should be printed now");
        */
        
        day_tick++;
        std::cout << "Day tick : " << (this->day_tick) << endl;
        this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_word_date_godot());


        int* datenumber = return_date(int(this->day_tick));

        
        if (datenumber[0] == 1) 
        {   
            // CALLED EVERY MONTH

           transport_to_add();
        }

        if (datenumber[0] == 1 && datenumber[1] == 1) 
        { 
            // CALLED EVERY YEAR

            this->budget += 1000*all_structures.size(); 
        }
        
        (this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment")->get_node("DirectionalLight"))->set("rotation_degrees", Vector3(-45 - sin((M_PI * (2 * (double)(day_tick % 365)) / 365)) * 25 / 2 - 12.5, 45, 0));
        ((WorldEnvironment*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment")))->set("fog_color", Color(0.77, 0.8, 0.86, (1 - airQuality) * 0.11));
        
        (this->date_counter)--;

    }

    if (this->notification_active)
    {
        (this->notification_counter)++;
        if (this->notification_counter > this->notification_timeout)
        {
            this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
            this->notification_active = false;
            this->notification_counter = 0;
        }
    }
}


void City::update_date() {
    
    
}

void City::_input(InputEvent*)
{

    Input* i = Input::get_singleton();


    if (i->is_action_pressed("ui_test"))
    {
        transport_to_add();
        add_car();
    }

    if (i->is_action_pressed("ui_turn"))
    {
        hide_menus();

        this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
        this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

        this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);

        this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
        this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);

        this->_on_Reset_cancelled();
        this->_on_Exit_cancelled();

        this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
        this->notification_active = false;
        this->notification_counter = 0;
        this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", true);
    }

    if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->get("visible")) {
        this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
        this->notification_active = false;
        this->notification_counter = 0;
        this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuTransport")->set("visible", true);
    }

    if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible"))
    {
        this->_on_Validate_pressed();
    }
};


void City::generate_initial_city_graphics()
{

    // srand((int)time(0));

    Vector3 center = Vector3(15.0 * citysize + 62, 0, 15.0 * citysize + 62);

    bool hasnuclear = false;
    bool hascoal = false;
    bool hasgeo = false;
    double dist;

    for (int x = 1; x < (citysize / 2) + 1; x++)
    {
        for (int z = 1; z < (citysize / 2) + 1; z++)
        {
            Vector3 pos = Vector3(60.0 * x, 0, 60.0 * z);


            Vector3 bigbuildingpos = pos + Vector3(15.0, 0, 15.0);
            dist = pos.distance_to(center);



            // This if condition keeps the building as a circle. if a 2x2 building cant be put it checks if a 1 x 1 building can

            if (dist <= citysize * 15.0) {

                //  probability functions for buildings
                // calculate building prob takes the first appearance of the building, last appearance of the building, proprtion and distance
                // values are calibrated for a citysize of 20 but adjusts to any size
                // maximum possible distance from center is 300

                std::cout << "DEBUG: about to calculate probability" << std::endl;
                // 2x2 buildings
                float mallprob = calculate_building_prob(20, 130, 0.4, dist);
                float nuclearprob = calculate_building_prob(190, 240, 0.8, dist);
                float coalprob = calculate_building_prob(190, 240, 0.8, dist);
                float geoprob = calculate_building_prob(190, 240, 0.8, dist);
                float fieldprob = calculate_building_prob(230, 320, 1, dist);
                float pastureprob = calculate_building_prob(230, 320, 2, dist);
                float factoryprob = calculate_building_prob(170, 200, 0.01, dist);

                if (hasgeo) { geoprob = 0; }
                if (hasnuclear) { nuclearprob = 0; }
                if (hascoal) { coalprob = 0; }

                std::cout << "DEBUG: distance : " << dist << std::endl;


                // 1 x 1 buildings

                dist = pos.distance_to(center);

                float restaurantprob = calculate_building_prob(0, 150, 1.2, dist) + calculate_building_prob(260, 300, 0.2, dist);
                float shopprob = calculate_building_prob(0, 200, 1.2, dist) + calculate_building_prob(260, 300, 0.2, dist);
                float buildingprob = calculate_building_prob(180, 210, 1.2, dist) + calculate_building_prob(-100, 150, 1, dist);
                float windmillprob = calculate_building_prob(0, 160, 0.05, dist) + calculate_building_prob(270, 315, 0.3, dist);
                float lowhouseprob = calculate_building_prob(-200, 200, 5, dist) + calculate_building_prob(270,310,0.25, dist);
                float highhouseprob = calculate_building_prob(-140, 200, 4, dist) + calculate_building_prob(270, 310, 0.7, dist);

                //std::cout << "DEBUG: highhouseprob  : " << highhouseprob << std::endl;
                //std::cout << "DEBUG: lowhouseprob  : " << lowhouseprob << std::endl;
                //std::cout << "DEBUG: restaurantprob  : " << restaurantprob << std::endl;


                float smallerprob = restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob;

                //std::cout << "DEBUG: About to create a random shop" << std::endl;

                std::cout << "DEBUG: done calculate probability" << std::endl;

                double bigbuildingmaybe = (double((double)rand() / (double)RAND_MAX) * double((mallprob + coalprob + geoprob+nuclearprob + fieldprob + factoryprob + pastureprob + smallerprob)));
                //std::cout << "DEBUG: Add buildings" << std::endl;
                if (bigbuildingmaybe < (double)(mallprob)) { std::cout << "DEBUG: Add mall" << std::endl;  add_shop(bigbuildingpos, MallScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob)) { std::cout << "DEBUG: Add NuclearPowerPlantScene" << std::endl;  add_energy(bigbuildingpos, NuclearPowerPlantScene); hasnuclear = true; }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob)) { std::cout << "DEBUG: Add FieldScene" << std::endl;  add_production(bigbuildingpos, FieldScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob / 2)) { std::cout << "DEBUG: Add SheepPastureScene" << std::endl;  add_production(bigbuildingpos, SheepPastureScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob)) { std::cout << "DEBUG: Add PigsPastureScene" << std::endl;  add_production(bigbuildingpos, PigsPastureScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob)) { std::cout << "DEBUG: Add FactoryScene" << std::endl;  add_production(bigbuildingpos, FactoryScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob + geoprob)) { std::cout << "DEBUG: Add GeothermalScene" << std::endl;  add_energy(bigbuildingpos, GeothermalPowerPlantScene); hasgeo = true; }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob + geoprob + coalprob)) { std::cout << "DEBUG: Add Coalpowerplantscene " << std::endl;  add_energy(bigbuildingpos, CoalPowerPlantScene); hascoal = true; }
                else {
                    
                    //std::cout << "DEBUG: Add else" << std::endl;
                    srand(int((x + 1) * (z + 1) * (int)(time(0))));

                    for (int x1 = 0; x1 < 2; x1++)
                    {
                        for (int z1 = 0; z1 < 2; z1++) {

                            Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);

                            restaurantprob = calculate_building_prob(0, 150, 1.2, dist) + calculate_building_prob(260, 300, 0.2, dist);
                            shopprob = calculate_building_prob(0, 200, 1.2, dist) + calculate_building_prob(260, 300, 0.2, dist);
                            buildingprob = calculate_building_prob(180, 210, 1.2, dist) + calculate_building_prob(-100, 150, 1, dist);
                            windmillprob = calculate_building_prob(0, 160, 0.05, dist) + calculate_building_prob(270, 315, 0.3, dist);
                            lowhouseprob = calculate_building_prob(-200, 200, 5, dist) + calculate_building_prob(270, 310, 0.25, dist);
                            highhouseprob = calculate_building_prob(-140, 200, 4, dist) + calculate_building_prob(270, 310, 0.7, dist);

                            double type = (double((double)rand() / (double)RAND_MAX) * (restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob));

                            if (type < (double)(restaurantprob)) { 
                                add_shop(pos + pos1, RestaurantScene); 
                                int randompdint = rand() % 3;
                                if (randompdint == 0) {
                                    if (PDScene.is_valid()) {
                                        std::cout << "DEBUG: adding pedestrian" << std::endl;
                                        Node* node;

                                        node = PDScene->instance();

                                        node->set("translation", (pos + pos1 + Vector3(-7, 0, -9)));

                                        this->add_child(node);
                                    }
                                }
                            }
                            else if (type < (double)(restaurantprob + shopprob)) { 
                                add_shop(pos + pos1, ShopScene);
                                int randompdint = rand() % 3;
                                if (randompdint == 0) {
                                    if (PDScene.is_valid()) {
                                        std::cout << "DEBUG: adding pedestrian" << std::endl;
                                        Node* node;

                                        node = PDScene->instance();

                                        node->set("translation", (pos + pos1 + Vector3(-7, 0, -9)));

                                        this->add_child(node);
                                    }
                                }
                            }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob)) { 
                                add_house(pos + pos1, LowHouseScene); 
                                int randompdint = rand() % 5;
                                if (randompdint == 0) {
                                    if (PDScene.is_valid()) {
                                    std::cout << "DEBUG: adding pedestrian" << std::endl;
                                    Node* node;

                                    node = PDScene->instance();

                                    node->set("translation", (pos + pos1 + Vector3(-7, 0, -9)));

                                    this->add_child(node);
                                }
                            }
                            }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob + buildingprob)) { 
                                add_house(pos + pos1, BuildingScene);
                                int randompdint = rand() % 5;
                                if (randompdint == 0) {
                                    if (PDScene.is_valid()) {
                                        std::cout << "DEBUG: adding pedestrian" << std::endl;
                                        Node* node;

                                        node = PDScene->instance();

                                        node->set("translation", (pos + pos1 + Vector3(-7, 0, -9)));

                                        this->add_child(node);
                                    }
                                }
                            }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob + buildingprob + windmillprob)) { add_energy(pos + pos1, WindmillScene); }
                            else { add_house(pos + pos1, HighHouseScene); }


                            

                        }
                    }
                    
                }
            }
            /*    this stuff was for that ring around the fields
            
            else
            {
                
                for (int x1 = 0; x1 < 2; x1++)
                {
                    for (int z1 = 0; z1 < 2; z1++) {
                        srand(int((x1 + 1) * (z1 + 1) * (x + 1) * (z + 1)));
                        Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1) ;

                        Vector3 pos2 = pos + pos1;
                        dist = pos2.distance_to(center);
                        //dist = pow(pow(center.x - pos1.x, 2) + pow(center.z - pos1.z, 2), 0.5);

                        if (dist <= citysize * 15) {
                            // plops down a building if theres no space for a 2x2 but there is for a 1x1  ... this should be on the outside of the city so if we want only one or two types on the edge we can set that here


                            float restaurantprob = calculate_building_prob(0, 150, 1, dist) + calculate_building_prob(260, 300, 0.2, dist);
                            float shopprob = calculate_building_prob(0, 200, 1.2, dist) + calculate_building_prob(260, 300, 0.2, dist);
                            float buildingprob = calculate_building_prob(180, 210, 1.5, dist) + calculate_building_prob(-100, 150, 1, dist);
                            float windmillprob = calculate_building_prob(0, 160, 0.05, dist) + calculate_building_prob(270, 315, 0.3, dist);
                            float lowhouseprob = calculate_building_prob(-200, 200, 5, dist) + calculate_building_prob(270, 310, 0.25, dist);
                            float highhouseprob = calculate_building_prob(-140, 200, 4, dist) + calculate_building_prob(270, 310, 0.7, dist);


                            double type = (double((double)rand() / (double)RAND_MAX) * double(restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob));

                            if (type < (double)(restaurantprob)) { add_shop(pos + pos1, RestaurantScene); }
                            else if (type < (double)(restaurantprob + shopprob)) { add_shop(pos + pos1, ShopScene); }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob)) { add_house(pos + pos1, LowHouseScene); }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob + buildingprob)) { add_house(pos + pos1, BuildingScene); }
                            else if (type < (double)(restaurantprob + shopprob + lowhouseprob + buildingprob + windmillprob)) { add_energy(pos + pos1, WindmillScene); }
                            else { add_house(pos, HighHouseScene); }

                        }
                    }
                }
            }*/
           
        }
    }
    transport_to_add();

    for (std::vector<Structure*>::iterator it = all_structures.begin(); it != all_structures.end(); ++it)
    {
        int randomcarint = rand() % 3;
        if (randomcarint == 0) { std::cout << "DEBUG: adding car" << std::endl;  add_car((Vector3)((Node*)(*it))->get("translation")); }
    }
   

    //   shuffles all the vectors so that buildings arent updated in a noticeable pattern
    std::random_shuffle(all_structures.begin(), all_structures.end());
    std::random_shuffle(all_energies.begin(), all_energies.end());
    std::random_shuffle(all_houses.begin(), all_houses.end());
    std::random_shuffle(all_shops.begin(), all_shops.end());
    std::random_shuffle(all_production.begin(), all_production.end());

    std::random_shuffle(trees_vector.begin(), trees_vector.end());

    trees_iterator = trees_vector.begin();



    this->budget += 1000 * all_structures.size();

    // loop to ensure population and employees match up by tweaking the population
    
    while (population < numberOfEmployees * 1.02 || population > numberOfEmployees * 1.10) {
        if (population < numberOfEmployees * 1.02) {
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                this->population -= (int)(*it)->get("numberOfInhabitants");
                (*it)->set("numberOfInhabitants", (int)(*it)->get("numberOfInhabitants") + (int)(rand() % 3));
                this->population += (int)(*it)->get("numberOfInhabitants");
            }
        }
        if (population > numberOfEmployees * 1.10) {
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                this->population -= (int)(*it)->get("numberOfInhabitants");
                (*it)->set("numberOfInhabitants", (int)(*it)->get("numberOfInhabitants") - (int)(rand() % 2));
                this->population += (int)(*it)->get("numberOfInhabitants");
            }
        }
        

    }

    std::cout << "DEBUG: CITY GENERATION DONE" << std::endl;
}

void City::set_initial_visible_components()
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);

    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);

    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetButton")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/ExitButton")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", false);

}




void City::_ready()
{

    //std::cout << "DEBUG: Ready started" << std::endl;
    // citysize = 10;

    this->generate_initial_city_graphics();
    structures_iterator = all_structures.begin();
    this->set_initial_visible_components();


}

void City::_on_ExitButton_pressed()
{

    this->_on_Reset_cancelled();
    this->time_speed = 0;

    this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", false);

}

void City::_on_Exit_cancelled()
{
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", false);
    this->_on_Game_Speed_changed();
}

void City::_on_Exit_confirmed()
{
    this->get_tree()->quit();
}


void City::_on_ResetButton_pressed()
{

    this->_on_Exit_cancelled();
    this->time_speed = 0;

    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", false);

}

void City::_on_Reset_cancelled()
{
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);
    this->_on_Game_Speed_changed();
}

void City::_on_Reset_confirmed()
{

    /*

    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);
    int city_child_count = this->get_child_count();
    for (int i = city_child_count - 1; i >= 0; --i) {
        if (this->get_child(i)->get("name") != String("WorldEnvironment") && this->get_child(i)->get("name") != String("Player")) {
            this->remove_child(this->get_child(i));
        }
    }
    Node* PlayerNode = this->get_node("Player");
    this->remove_child(PlayerNode);
    this->add_child(PlayerNode);
    income = 0;
    population = 50000;
    numberOfEmployees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
    environmentalCost = 0;
    totalSatisfaction = 50;
    time_speed = 1;
    //timer = 0;
    day_tick = 0;
    days_since_last_simulation = 0;
    // in order to find date
    daycount = 0;
    // in order to write stats to csv files
    stat = 0;
    srand((int)time(0));
    this->_init();
    this->_ready();
    */

    this->get_tree()->reload_current_scene();
}



void City::_on_TransportMenuButton_pressed() 
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", false);

    String transportInfo = String("INFORMATION") + String("\n");
    transportInfo += String("Transport") + String("\n");
    transportInfo += String((int)(current_car_quantities[0])) + String("Electric Cars") + String("\n");

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("text", transportInfo);
    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuTransport")->set("position", Vector2((get_viewport()->get_size().x) / 2, (get_viewport()->get_size().y) / 2));
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuTransport")->set("visible", true);
}


void City::_on_Menu_pressed(String name)
{

    active_button = name;

    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("text", String(""));

    String ButtonInfo = this->get_button_info_text();
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("text", ButtonInfo);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", true);


    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);

}


String City::get_button_info_text() {

    // This method returns text giving information about the various policies 
    // The placeholder text for the input box changed to correpsond to the old value of the "policy"

    // I DON'T KNOW HOW TO DO THE SECOND PART YET 
    // I GUESS THESE VALUES SHOULD BE STORED IN CITY AND NOT FETCHED FROM RANDOM OBJECTS

    if (this->active_button == String("PanelSubsidyForShops"))
    {
        //this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("placeholder_text", String(""));
        return String("Please input a value between 0 and 450. This will be a solar panel subsidy for shops (restaurants, small shops and malls) in euros.");
    }
    else if (this->active_button == String("ChangePanelProbabilityForRestaurants"))
    {
        return String("Please input a value between 0 and 800. This value will be the new wind Turbine subsidy for shops (restaurants, small shops and malls) in euros.");
    }
    else if (this->active_button == String("EfficiencySupercriticalCoalPlant"))
    {
        return String("This policy imposes coal power plants to go to a supercritical effciency (43%). Please input 1 in order to activate the policy or 0 to return to a subcritical efficiency (38%).");
    }
    else if (this->active_button == String("EfficiencyCogenerationCoalPlant"))
    {
        return String("This policy imposes coal power plants to go to a cogeneration effciency (47%). Please input 1 in order to activate the policy or 0 to return to a subcritical efficiency (38%).");
    }
    else if (this->active_button == String("NuclearProhibition"))
    {
        return String("This is a law prohibiting the use of nuclear power. Please input 1 in order to activate the policy or 0 to authorize nuclear power plants again.");
    }
    else if (this->active_button == String("CoalProhibition"))
    {
        return String("This is a law prohibiting the use of coal power. Please input 1 in order to activate the policy or 0 to authorize coal power plants again.");
    }
    else if (this->active_button == String("MaximumCarbonFactories"))
    {
        return String("This is a law imposing a maximum amount of carbon emissions for factories. Please input a value between 0 and 42 kg per day per factory.");
    }
    else if (this->active_button == String("SubsidyFactories"))
    {
        return String("This is a subsidy for green factories, promoting the reduction of harmful chemicals and heavy metals emissions. Please input a value between 1000 and 100 000 euros per factory per year or 0 to remove the policy");
    }
    else if (this->active_button == String("Pesticides"))
    {
        return String("This is a law prohibiting the use of pesticides. Please input 1 in order to activate the policy or 0 to authorize pesticides again.");
    }
    else if (this->active_button == String("GMO"))
    {
        return String("This is a law prohibiting the use of GMOs. Please input 1 in order to activate the policy or 0 to authorize GMOs again.");
    }
    else if (this->active_button == String("Fertilizers"))
    {
        return String("This is a law prohibiting the use of fertilizers. Please input 1 in order to activate the policy or 0 to authorize fertilizers again.");
    }
    else if (this->active_button == String("SolarHousing"))
    {
        return String("This is a subsidy for building solar panels on houses. Please input a value between 0 and 450 euros per family.");
    }
    else if (this->active_button == String("WindturbinesHousing"))
    {
        return String("This is a subsidy for building rooftop wind turbines on houses. Please input a value between 0 and 800 euros per family.");
    }
    else if (this->active_button == String("DoubleGlazingHousing"))
    {
        return String("This is a subsidy for implementing double glazing on houses. Please input a value between 0 and 1000 euros per family.");
    }
    else if (this->active_button == String("ConsumptionTransport"))
    {
        return String("This is a tax on car consumption. Please input a value between 0 and 2 euros per liter of fuel.");
    }
    else if (this->active_button == String("WeightTransport"))
    {
        return String("This is a tax on car weight. Please input a value between 0 and 5000 euros.");
    }
    else if (this->active_button == String("BikeSubsidy"))
    {
        return String("This is a subsidy for bicycles. Please input a value between 0 and 250 euros.");
    }
    else if (this->active_button == String("ElectricSubsidy"))
    {
        return String("This is a subsidy for electric cars. Please input a value between 0 and 30 000 euros.");
    }
    else if (this->active_button == String("BusSubsidy"))
    {
        return String("This is a subsidy for public transport. Please input a value between 0 and 60 000 euros.");
    }
    else if (this->active_button == String("CarsProhibition"))
    {
        return String("This is a law prohibiting the use of all cars during a certain number of days per week. Please input a value between 0 and 7 days per week.");
    }
    else if (this->active_button == String("TreesBudget"))
    {
        return String("This is a policy implementing a budget for planting and growing more trees in residential and commercial areas. It costs 5000 euros to surround a single building with trees. Please input the number of buildings you wish to plant trees around.");
    }
    else {
        return String("No information has been specified for this policy.");
    }
}

void City::hide_menus()
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuEnergy")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuHousing")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuTransport")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuProduction")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", true);
}

void City::_on_Validate_pressed()
{

    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
    String mytext = this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->get("text");
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/TransportMenuButton")->set("visible", true);

    (this->get_tree()->get_root()->get_node("Main/3Dworld/Player"))->set("movable", true);
    this->_on_Game_Speed_changed();

    if (mytext.is_valid_float()) {
        this->implement_policies((double)mytext.to_float());
    }
    else {
        this->trigger_notification(String("You did not enter an appropriate value for the policy."));
    }
}

void City::trigger_notification(String text = String("It appears that some sort of mistake has occured. Please try again."))
{
    //std::cout << "INVALID INPUT: EXPECTED FLOAT IN SPECIFIED RANGE" << std::endl;
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("text", text);
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", true);

    this->notification_active = true;
}

void City::implement_policies(double value) {

    Godot::print(this->active_button);

    if (this->active_button == String("PanelSubsidyForShops")) {
        if (value >= 0 && value <= 450) {
            Godot::print("PANEL SUBSIDY WILL BE CHANGED FOR ALL SHOPS");
            for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
            {
                (*it)->set("solar_panel_subsidies", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
   else if (this->active_button == String("ChangePanelProbabilityForRestaurants")) {    ///I can Changed the name here
		if (value >= 0 && value <= 800) {
			Godot::print("ROOFTOP WINDTURBINES SUBSIDIES ON SHOPS IMPLEMENTED");
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				//if ((String)(*it)->get("object_type") == (String)("Restaurant")) {}
				(*it)->set("wind_turbine_subsidies", value);
				
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }

	//##########
	else if (this->active_button == String("WindturbinesHousing")) {
        if (value >= 0 && value <= 800) {
            Godot::print("ROOFTOP WINDTURBINES SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                (*it)->set("wind_turbine_subsidies", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }

	///######

    else if (this->active_button == String("EfficiencySupercriticalCoalPlant")) {
        if (value == 0 || value == 1) {
            Godot::print("THE COAL POWER PLANTS WILL CHANGE THEIR EFFICIENCY");
            for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Coal Power Plant")) {
                    (*it)->set("efficiency_supercritical", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("EfficiencyCogenerationCoalPlant")) {
        if (value == 0 || value == 1) {
            Godot::print("THE COAL POWER PLANTS WILL CHANGE THEIR EFFICIENCY");
            for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Coal Power Plant")) {
                    (*it)->set("efficiency_cogeneration", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("NuclearProhibition")) {
        if (value == 0 || value == 1) {
            Godot::print("THE LAW PROHIBITING OR ALLOWING THE USE OF NUCLEAR POWER WILL BE IMPLEMENTED");
            for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
            {
                 (*it)->set("nuclear_prohibited", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("CoalProhibition")) {
        if (value == 0 || value == 1) {
            Godot::print("THE LAW PROHIBITING OR ALLOWING THE USE OF COAL POWER WILL BE IMPLEMENTED");
            for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
            {
                (*it)->set("coal_prohibited", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("MaximumCarbonFactories")) {
        if (42>=value && value>= 0) {
            Godot::print("MAXIMUM EMISSIONS ON FACTORIES IMPLEMENTED");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Goods Factory")) {
                    (*it)->set("maximum_CO2", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("SubsidyFactories")) {
        if ((100000 >= value && value >= 1000) || value == 0) {
            Godot::print("GREEN SUBSIDY FOR FACTORIES IMPLEMENTED");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Goods Factory")) {
                    (*it)->set("subsidy_green", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("Pesticides")) {
        if (value == 1 || value == 0) {
            Godot::print("PROHIBITION OF PESTICIDES");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Agricultural Production")) {
                    (*it)->set("pesticideProhibited", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("GMO")) {
        if (value == 1 || value == 0) {
            Godot::print("PROHIBITION OF GMOs");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Agricultural Production")) {
                    (*it)->set("GMOProhibited", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("Fertilizers")) {
        if (value == 1 || value == 0) {
            Godot::print("PROHIBITION OF FERTILIZERS");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if ((String)(*it)->get("object_type") == (String)("Agricultural Production")) {
                    (*it)->set("fertilizerProhibited", value);
                }
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("SolarHousing")) {
        if (value >= 0 && value<=450) {
            Godot::print("SOLAR PANELS SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                 (*it)->set("solar_panel_subsidies_housing", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("WindturbinesHousing")) {
        if (value >= 0 && value <= 800) {
            Godot::print("ROOFTOP WINDTURBINES SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                (*it)->set("wind_turbine_subsidies", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("DoulbleGlazingHousing")) {
        if (value >= 0 && value <= 1000) {
            Godot::print("DOUBLE GLAZING SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                (*it)->set("double_glazing_subsidies", value);
            }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("ConsumptionTransport")) {
        if (value >= 0 && value <= 2) { 
            Godot::print("TAX ON CAR CONSUMPTION IMPLEMENTED");
            fuelTax = value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("WeightTransport")) {
        if (value >= 0 && value <= 5000) { 
            Godot::print("TAX ON CAR WEIGHT IMPLEMENTED");
            weightTax = value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("BikeSubsidy")) {
        if (value >= 0 && value <= 250) { 
            Godot::print("BIKE SUBSIDY IMPLEMENTED");
            bikeSubsidy = value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("ElectricSubsidy")) {
        if (value >= 0 && value <= 30000) { 
            Godot::print("ELECTRIC CAR SUBSIDY IMPLEMENTED");
            electricCarSubsidy =  value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("BusSubsidy")) {
        if (value >= 0 && value <= 60000) { 
            Godot::print("BUS SUBSIDY IMPLEMENTED");
            busSubsidy = value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("CarsProhibition")) {
        if (value >= 0 && value <= 7) { 
            Godot::print("CARS PROHIBITION ON CERTAIN DAYS IMPLEMENTED");
            carProhibition = value;
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("TreesBudget")) {
    if (trees_iterator == trees_vector.end()){ this->trigger_notification(String("All possible buildings now have trees ! This is a good thing. However, you cannot add any more.")); }
    if (value >= 0) {
        Godot::print("TREES ARE BEING ADDED");
        this->houses_with_trees += int(value);

        for (int i = 0; i < int(value); ++i) {
            (*trees_iterator)->get_node("MeshComponents/Trees")->set("visible", true);
            trees_iterator++;
            budget -= 5000;
            houses_with_trees++;
            if (trees_iterator == trees_vector.end()) { this->trigger_notification(String("All possible buildings now have trees ! This is a good thing. However, you cannot add any more.")); break; }
        }
    }
    else {
        this->trigger_notification(String("The value you provided was not possible. Please try planting trees again."));
    }
    }
}

void City::_on_Game_Speed_changed()
{
    time_speed = round(pow(2, (int)(this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->get_child(0)->get("value")) - 1) - 0.1);

    hide_menus();
    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    (this->get_tree()->get_root()->get_node("Main/3Dworld/Player"))->set("movable", true);
    change_pie_chart(100*(totalSatisfaction/(10*all_structures.size())), "PieSatisfaction", true); 
    change_pie_chart(value_pie_chart_C02(carbonEmission, 1000000), "PieCO2", false); 
	change_pie_chart(100*(income/population), "PieIncome", true); 
    change_pie_chart(energyDemand, "PieUnemployement", false); //EnergyDemand variable is temporary
    change_pie_chart(value_pie_chart_C02(energyDemand, 100000), "PiePowerDemand", false);

	change_pie_label(totalSatisfaction, "PieSatisfaction");
	change_pie_label(carbonEmission, "PieCO2");
	change_pie_label(income, "PieIncome");
	change_pie_label(numberOfEmployees, "PieEmployees");
	change_pie_label(energyDemand, "PieUnemployement");
	change_pie_label(energyDemand, "PiePowerDemand");


}


void City::add_car(Vector3 pos) { //adds a car at a location given by the vector with a shift
    std::cout << "CURRENT air quality is: " << airQuality << std::endl;

    const Ref<PackedScene> OldCarScene = ResourceLoader::get_singleton()->load("res://Resources/Bugatti.tscn", "PackedScene");
    const Ref<PackedScene> SportCarScene = ResourceLoader::get_singleton()->load("res://Resources/Chiron.tscn", "PackedScene");
    const Ref<PackedScene> MotoScene = ResourceLoader::get_singleton()->load("res://Resources/Moto.tscn", "PackedScene");
    const Ref<PackedScene> BusScene = ResourceLoader::get_singleton()->load("res://Resources/Bus.tscn", "PackedScene");
    const Ref<PackedScene> AmericanCarScene = ResourceLoader::get_singleton()->load("res://Resources/Raptor.tscn", "PackedScene");
    const Ref<PackedScene> BikeScene = ResourceLoader::get_singleton()->load("res://Resources/Bike.tscn", "PackedScene");
    const Ref<PackedScene> ElectricCarScene = ResourceLoader::get_singleton()->load("res://Resources/Cybertruck.tscn", "PackedScene");
    const Ref<PackedScene> NormalCarScene = ResourceLoader::get_singleton()->load("res://Resources/Clio.tscn", "PackedScene");

    if (OldCarScene.is_valid() && SportCarScene.is_valid() && MotoScene.is_valid() && BusScene.is_valid() && AmericanCarScene.is_valid() && BikeScene.is_valid() && ElectricCarScene.is_valid() && NormalCarScene.is_valid())
    {
        int type = most_missing_type();
        std::cout << "The most missing type is currently : " << type << std::endl;
        if (type != -1) {

            //int type = rand() % 3;
            Node* node;
            std::cout << "INSTANCE A CAR OF TYPE: " << type << std::endl;

            switch (type) {
            case 0: node = ElectricCarScene->instance(); current_car_quantities[0] += 1; break;
            case 1: node = AmericanCarScene->instance(); current_car_quantities[1] += 1; break;
            case 2: node = NormalCarScene->instance(); current_car_quantities[2] += 1; break;
            case 3: node = OldCarScene->instance(); current_car_quantities[3] += 1; break;
            case 4: node = BikeScene->instance(); current_car_quantities[4] += 1; break;
            case 5: node = MotoScene->instance(); current_car_quantities[5] += 1; break;
            case 6: node = BusScene->instance(); current_car_quantities[6] += 1; break;
            case 7: node = SportCarScene->instance(); current_car_quantities[7] += 1;  break;
            default: node = SportCarScene->instance(); current_car_quantities[7] += 1;  break;
            }

            std::cout << "INSTANCE DONE A CAR OF TYPE: " << type << std::endl;
            node->set("scale", Vector3(10, 10, 10));
            node->set("translation", pos + Vector3(-13, 0.2, -13));

            std::cout << "ADD A CAR OF TYPE: " << type << std::endl;
            this->add_child((Node*)node);
            ((Transport*)node)->set("transportType", type);

            
            //income -= node->cost;
            //all_transports.push_back((Transport*)node);         THE TRANSPORTS VECTOR STILL NEEDS TO BE IMPLEMENTED 
        }
    }
}



void City::add_shop(Vector3 pos, Ref<PackedScene> scene) {

    //std::cout << "DEBUG: add shop called" << std::endl;

    //std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
    if (scene.is_valid()) {
        //std::cout << "DEBUG: creating node" << std::endl;
        Node* node;
        //std::cout << "DEBUG: instanciating" << std::endl;
        node = scene->instance();
        //std::cout << "DEBUG: setting scale and translation" << std::endl;
        node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
        node->set("translation", pos);
        node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
        node->get_node("MeshComponents/Trees")->set("visible", false);
        //std::cout << "DEBUG: add child" << std::endl;
        this->add_child(node);
        //std::cout << "DEBUG: add shop to vector" << std::endl;
        all_shops.push_back((Shop*)node);
        all_structures.push_back((Structure*)node);
        if (scene != MallScene){ trees_vector.push_back((Structure*)node); }

        //std::cout << "DEBUG: traffic stuff called" << std::endl;

        // traffic stuff
        //std::cout << "DEBUG: traffic stuff called" << std::endl;
        //double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
        //double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
        double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
        double y = pos.z / 30; // can be int only for small building

        //std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
        //std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
        //std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
        traffic_preparation(x, y);
    }
}

void City::add_house(Vector3 pos, Ref<PackedScene> scene) {
    numberOfHouses += 1;
    //std::cout << "DEBUG: add house called" << std::endl;

    //std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
    if (scene.is_valid()) {
        //std::cout << "DEBUG: creating node" << std::endl;
        Node* node;
        //std::cout << "DEBUG: instanciating" << std::endl;
        node = scene->instance();
        if (scene == LowHouseScene) {
            ((House*)node)->set("houseType", 1.0); //((House*)node)->house_type();
        } 
        if (scene == HighHouseScene) {
            ((House*)node)->set("houseType", 2.0); //((House*)node)->house_type();
        }
        //std::cout << "DEBUG: setting scale and translation" << std::endl;
        node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
        node->set("translation", pos);
        node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
        node->get_node("MeshComponents/Trees")->set("visible", false);
        //std::cout << "DEBUG: add child" << std::endl;
        this->add_child(node);
        //std::cout << "DEBUG: add house to vector" << std::endl;
        all_houses.push_back((Housing*)node);
        all_structures.push_back((Structure*)node);
        trees_vector.push_back((Structure*)node);
        
        

         
        // traffic stuff
        //std::cout << "DEBUG: traffic stuff called" << std::endl;
        //double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
        //double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
        double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
        double y = pos.z / 30; // can be int only for small building

        //std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
        //std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
       // std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
        traffic_preparation(x, y);
        //std::cout << "DEBUG: add house done" << std::endl;

        std::cout << "DEBUG: ADD HOUSE DONE" << std::endl;
    }
}

void City::add_energy(Vector3 pos, Ref<PackedScene> scene) {

    //std::cout << "DEBUG: add shop called" << std::endl;

    //std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
    if (scene.is_valid()) {
        //std::cout << "DEBUG: creating node" << std::endl;
        Node* node;
        //std::cout << "DEBUG: instanciating" << std::endl;
        node = scene->instance();
        //std::cout << "DEBUG: setting scale and translation" << std::endl;
        node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
        node->set("translation", pos);
        if (scene != WindmillScene) { node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0)); }
        //std::cout << "DEBUG: add child" << std::endl;
        this->add_child(node);
        //std::cout << "DEBUG: add shop to vector" << std::endl;
        all_energies.push_back((Energy*)node);
        all_structures.push_back((Structure*)node);

        //std::cout << "DEBUG: traffic stuff called" << std::endl;
        // traffic stuff
        //std::cout << "DEBUG: traffic stuff called" << std::endl;
        //double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
        //double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
        double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
        double y = pos.z / 30; // can be int only for small building

        //std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
        //std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
       // std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
        traffic_preparation(x, y);
        //std::cout << "DEBUG: add energy done" << std::endl;
    }
}

void City::add_production(Vector3 pos, Ref<PackedScene> scene) {

    //std::cout << "DEBUG: add shop called" << std::endl;

    //std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
    if (scene.is_valid()) {
        //std::cout << "DEBUG: creating node" << std::endl;
        Node* node;
        //std::cout << "DEBUG: instanciating" << std::endl;
        node = scene->instance();
        //std::cout << "DEBUG: setting scale and translation" << std::endl;
        node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
        node->set("translation", pos);
        node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
        //std::cout << "DEBUG: add child" << std::endl;
        this->add_child(node);
        //std::cout << "DEBUG: add shop to vector" << std::endl;
        all_production.push_back((Production*)node);
        all_structures.push_back((Structure*)node);



        // traffic stuff
        //std::cout << "DEBUG: traffic stuff called" << std::endl;
        //double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
        //double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
        double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
        double y = pos.z / 30; // can be int only for small building

        //std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
        //std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
        //std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
        traffic_preparation(x, y);
        //std::cout << "DEBUG: add production done" << std::endl;
    }
}

void City::traffic_preparation(double x, double y) {
    if (x < sizeOfCity && y < sizeOfCity) {
        if (x > int(x) - 0.1 && x < int(x) + 0.1) { // check that it's a small building
            positionOfBuildings[int(x)][int(y)] = 1;
            //std::cout << " SMALL BUILDING CREATED" << std::endl;
        }
        else {
            positionOfBuildings[int(x)][int(y)] = 2; // assign numbers to the four squares of the 2 by 2 buidling to know it's position by knowing just the coordinates and the number of one square
            positionOfBuildings[int(x) + 1][int(y)] = 3;
            positionOfBuildings[int(x) + 1][int(y) + 1] = 4;
            positionOfBuildings[int(x)][int(y) + 1] = 5;
        }
        //std::cout << "DEBUG: call the function update traffic" << std::endl;
        update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);
    }
}

void City::update_traffic(int x, int y, bool newBuilding, int number) {
    //std::cout << "DEBUG: UPDATE TRAFFIC STARTED for coordinates" << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
    if (positionOfBuildings[x][y] != 0) { // nothing happens if the building isn't there
        //std::cout << "DEBUG: updating traffic for coordinates : " << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
        if (number == 1) {  // the case when it's a 1 by 1 buidling
            traffic_system[x][y][0][2] = 1;
            traffic_system[x][y][1][2] = 1;
            traffic_system[x][y][2][2] = 1;
            traffic_system[x][y][3][2] = 1;
            if (x + 1 < sizeOfCity && y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y + 1] == 1 || positionOfBuildings[x + 1][y + 1] == 2 || positionOfBuildings[x + 1][y + 1] == 3)) {
                traffic_system[x][y][3][0] = 1;
            }
            if (y + 1 < sizeOfCity && (positionOfBuildings[x][y + 1] == 1 || positionOfBuildings[x][y + 1] == 2 || positionOfBuildings[x][y + 1] == 5)) {
                traffic_system[x][y][3][1] = 1;
            }
            if (x - 1 >= 0 && y + 1 < sizeOfCity && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 3 || positionOfBuildings[x - 1][y + 1] == 4)) {
                traffic_system[x][y][2][0] = 1;
            }
            if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
                traffic_system[x][y][2][1] = 1;
            }
            if (x - 1 >= 0 && y - 1 >= 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
                traffic_system[x][y][1][0] = 1;
            }
            if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
                traffic_system[x][y][1][1] = 1;
            }
            if (x + 1 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
                traffic_system[x][y][0][0] = 1;
            }
            if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
                traffic_system[x][y][0][1] = 1;
            }
            if (newBuilding == true) {  // update all the possible buildings around
                // no need to check if they exist since if they don't the function checks that with the first line
                if (x - 1 >= 0) {
                    if (y - 1 >= 0) {
                        update_traffic(x - 1, y - 1, false, positionOfBuildings[x - 1][y - 1]);
                    }
                    update_traffic(x - 1, y, false, positionOfBuildings[x - 1][y]);
                    if (y + 1 < sizeOfCity) {
                        update_traffic(x - 1, y + 1, false, positionOfBuildings[x - 1][y + 1]);
                    }
                }
                if (y - 1 >= 0) {
                    update_traffic(x, y - 1, false, positionOfBuildings[x][y - 1]);
                }
                if (y + 1 < sizeOfCity) {
                    update_traffic(x, y + 1, false, positionOfBuildings[x][y + 1]);
                }
                if (x + 1 < sizeOfCity) {
                    if (y - 1 >= 0) {
                        update_traffic(x + 1, y - 1, false, positionOfBuildings[x + 1][y - 1]);
                    }
                    update_traffic(x + 1, y, false, positionOfBuildings[x + 1][y]);
                    if (y + 1 < sizeOfCity) {
                        update_traffic(x + 1, y + 1, false, positionOfBuildings[x + 1][y + 1]);
                    }
                }
            }
        }
        else { // the case when it's a 2 by 2 building
             std::cout << "DEBUG: updating traffic for BIG building coordinates : " << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
            if (number == 3) {
                x = x - 1;
            }
            if (number == 4) {
                x = x - 1;
                y = y - 1;
            }
            if (number == 5) {
                y = y - 1;
            }


            //left top
            traffic_system[x][y][1][2] = 1;
            if (x - 1 >= 0 && y - 1 >= 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
                traffic_system[x][y][1][0] = 1;
            }
            if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
                traffic_system[x][y][1][1] = 1;
            }
            traffic_system[x][y][0][1] = 1;
            traffic_system[x][y][0][2] = 1;
            if (y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
                traffic_system[x][y][0][0] = 1;
            }

            //right top
            traffic_system[x + 1][y][0][2] = 1;
            if (x + 2 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 2][y - 1] == 1 || positionOfBuildings[x + 2][y - 1] == 2 || positionOfBuildings[x + 2][y - 1] == 5)) {
                traffic_system[x + 1][y][0][0] = 1;
            }
            if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y] == 1 || positionOfBuildings[x + 2][y] == 2 || positionOfBuildings[x + 2][y] == 3)) {
                traffic_system[x + 1][y][0][1] = 1;
            }

            traffic_system[x + 1][y][3][1] = 1;
            if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 1] == 1 || positionOfBuildings[x + 2][y + 1] == 2 || positionOfBuildings[x + 2][y + 1] == 3)) {
                traffic_system[x + 1][y][3][0] = 1;
            }

            //right bottom
            traffic_system[x + 1][y + 1][3][2] = 1;
            if (x + 2 < sizeOfCity && y + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 2] == 1 || positionOfBuildings[x + 2][y + 2] == 2 || positionOfBuildings[x + 2][y + 2] == 3)) {
                traffic_system[x + 1][y + 1][3][0] = 1;
            }
            if (y + 2 < sizeOfCity && (positionOfBuildings[x + 1][y + 2] == 1 || positionOfBuildings[x + 1][y + 2] == 3 || positionOfBuildings[x + 1][y + 2] == 4)) {
                traffic_system[x + 1][y + 1][3][1] = 1;
            }

            traffic_system[x + 1][y + 1][2][1] = 1;
            if (y + 2 < sizeOfCity && (positionOfBuildings[x][y + 2] == 1 || positionOfBuildings[x][y + 2] == 3 || positionOfBuildings[x][y + 2] == 4)) {
                traffic_system[x + 1][y + 1][2][0] = 1;
            }

            //left bottom
            traffic_system[x][y + 1][2][2] = 1;
            if (x - 1 >= 0 && y + 2 < sizeOfCity && (positionOfBuildings[x - 1][y + 2] == 1 || positionOfBuildings[x - 1][y + 2] == 3 || positionOfBuildings[x - 1][y + 2] == 4)) {
                traffic_system[x][y + 1][2][0] = 1;
            }
            if (x - 1 >= 0 && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 4 || positionOfBuildings[x - 1][y + 1] == 5)) {
                traffic_system[x][y + 1][2][1] = 1;
            }

            traffic_system[x][y + 1][1][1] = 1;
            if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
                traffic_system[x + 1][y][1][0] = 1;
            }
            if (newBuilding == true) {  // update all the possible buildings around
                // no need to check if they exist since if they don't the function checks that with the first line
                if (x - 1 >= 0) {
                    if (y - 1 >= 0) {
                        update_traffic(x - 1, y - 1, false, positionOfBuildings[x - 1][y - 1]);
                    }
                    update_traffic(x - 1, y, false, positionOfBuildings[x - 1][y]);
                    update_traffic(x - 1, y + 1, false, positionOfBuildings[x - 1][y + 1]);
                    if (y + 2 < sizeOfCity) {
                        update_traffic(x - 1, y + 2, false, positionOfBuildings[x - 1][y + 2]);
                    }
                }
                if (y - 1 >= 0) {
                    update_traffic(x, y - 1, false, positionOfBuildings[x][y - 1]);
                    update_traffic(x + 1, y - 1, false, positionOfBuildings[x + 1][y - 1]);
                }
                if (y + 2 < sizeOfCity) {
                    update_traffic(x, y + 2, false, positionOfBuildings[x][y + 2]);
                    update_traffic(x + 1, y + 2, false, positionOfBuildings[x + 1][y + 2]);
                }
                if (x + 2 < sizeOfCity) {
                    if (y - 1 >= 0) {
                        update_traffic(x + 2, y - 1, false, positionOfBuildings[x + 2][y - 1]);
                    }
                    update_traffic(x + 2, y, false, positionOfBuildings[x + 2][y]);
                    update_traffic(x + 2, y + 1, false, positionOfBuildings[x + 2][y + 1]);
                    if (y + 2 < sizeOfCity) {
                        update_traffic(x + 2, y + 2, false, positionOfBuildings[x + 2][y + 2]);
                    }
                }
            }
        }
    }
}




void City::simulation_shops()
{
    //std::cout << "Simulation SHOPS" << std::endl;

    day_tick += this->time_speed * 5;
    this->days_since_last_simulation = 0;
    Godot::print(return_word_date_godot());
    this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_word_date_godot());


    for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
    {
        (*it)->set("updatable", true);
    }

}

void City::simulation_production()
{
    //std::cout << "Simulation PRODUCTION" << std::endl;

    for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
    {
        (*it)->set("updatable", true);
    }

}

void City::simulation_energy()
{
    //std::cout << "Simulation ENERGY" << std::endl;


    for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
    {
        (*it)->set("updatable", true);
    }

}

void City::simulation_housing()
{

    //std::cout << "Simulation HOUSING" << std::endl;

    for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
    {
        (*it)->set("updatable", true);
    }

    /*
    //   function which looks at a single random house at gives it the correct satisfaction then updates the total satisfaction
    Housing* h = all_houses.at(rand() % all_houses.size());
    double initialval = (double)(h->get("satisfaction"));
    Vector3 pos = ((Structure*)h)->get_position();
    double dist = 300;   // the distance where houses take into account the satisfaction
    int obj_count = 0;
    double tothouseSat = 0.0;
    this->totalSatisfaction = (totalSatisfaction * all_houses.size() - initialval);
    for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
    {
        if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
            tothouseSat += (double)((*it)->get("satisfaction")) * 10;
            obj_count++;
        }
    }
    for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
    {
        if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
            tothouseSat += (double)((*it)->get("satisfaction")) * 10;
            obj_count++;
        }
    }
    for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
    {
        if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
            tothouseSat += (double)((*it)->get("satisfaction")) * 10;
            obj_count++;
        }
    }
    tothouseSat /= obj_count;
    this->totalSatisfaction = (totalSatisfaction + tothouseSat) / all_houses.size();
    */

}




std::string return_number_date(int day, int month, int year) {
    return std::to_string(day) + ", " + std::to_string(month) + ", " + std::to_string(year);
}

/*
String return_number_date(int day, int month, int year) {
    return to_godot_string(day) + String(", ") + to_godot_string(month) + String(", ") + to_godot_string(year);
}
*/

double find_avg(double array[], int leap) {
    int size;
    double sum = 0;
    if (leap == 0) {
        size = 366;
    }
    else {
        size = 365;
    }
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

 std::string City::return_word_date(int days) {

    int* datenumber = return_date(int(days));
    std::string date = "Year ";
    date += std::to_string(datenumber[2]);
    date += ", ";

    if (datenumber[1] == 1) {
        date += "January ";
    }
    if (datenumber[1] == 2) {
        date += "February ";
    }
    if (datenumber[1] == 3) {
        date += "March ";
    }
    if (datenumber[1] == 4) {
        date += "April ";
    }
    if (datenumber[1] == 5) {
        date += "May ";
    }
    if (datenumber[1] == 6) {
        date += "June ";
    }
    if (datenumber[1] == 7) {
        date += "July ";
    }
    if (datenumber[1] == 8) {
        date += "August ";
    }
    if (datenumber[1] == 9) {
        date += "September ";
    }
    if (datenumber[1] == 10) {
        date += "October ";
    }
    if (datenumber[1] == 11) {
        date += "November ";
    }
    if (datenumber[1] == 12) {
        date += "December ";
    }

    date += std::to_string(datenumber[0]);
    return date;
}


String City::return_word_date_godot() {

    int* datenumber = return_date(int(this->day_tick));
    String date = String("Year ");
    date += to_godot_string(datenumber[2]);
    date += String(", ");

    if (datenumber[1] == 1) {
        date += String("January ");
    }
    if (datenumber[1] == 2) {
        date += String("February ");
    }
    if (datenumber[1] == 3) {
        date += String("March ");
    }
    if (datenumber[1] == 4) {
        date += String("April ");
    }
    if (datenumber[1] == 5) {
        date += String("May ");
    }
    if (datenumber[1] == 6) {
        date += String("June ");
    }
    if (datenumber[1] == 7) {
        date += String("July ");
    }
    if (datenumber[1] == 8) {
        date += String("August ");
    }
    if (datenumber[1] == 9) {
        date += String("September ");
    }
    if (datenumber[1] == 10) {
        date += String("October ");
    }
    if (datenumber[1] == 11) {
        date += String("November ");
    }
    if (datenumber[1] == 12) {
        date += String("December ");
    }

    date += to_godot_string(datenumber[0]);
    return date;
}



/// edit text files found in /addons/file.samples

std::string get_path(std::string documentName) {
    //std::ofstream log("C:\\logs\\performance.log", std::ofstream::app | std::ofstream::out);
    return "user://" + documentName + ".csv";
    //return "../../addons/easy_charts/file.samples/" + documentName + ".csv";
}

// Function to add a line of the form "2015;76" to the csv file named documentName.
// To do so, call add_data("pollution", "2015", "76");
void add_data(std::string documentName, std::string year, std::string value) {
    fstream file;
    std::string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
    file << year << ";" << value << '\n';
    file.close();
}

// Suppresses all data stored in the file documentName
void clear_completely(std::string documentName) {
    fstream file;
    std::string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

// Suppresses all data stored in the file documentName except the first line.
void clear(std::string documentName) {
    fstream file;
    std::string path = get_path(documentName);

    file.open(path);
    std::string line;
    getline(file, line);
    file.close();

    file.open(path, ios::out | ios::trunc);
    file.close();

    file.open(path);
    file << line << '\n';
    file.close();
}

// Copies the data stored in documentNameFrom to an empty file documentNameTo.
void copy(std::string documentNameFrom, std::string documentNameTo) {
    fstream fileFrom;
    fstream fileTo;
    std::string path1 = get_path(documentNameFrom);
    std::string path2 = get_path(documentNameTo);
    fileFrom.open(path1);
    fileTo.open(path2);
    while (fileFrom.good()) {
        std::string line;
        getline(fileFrom, line, '\n');
        if (line.length() > 0) {
            fileTo << line << '\n';
        }
    }
    fileFrom.close();
    fileTo.close();
}

// Function to modify a line of the csv file named documentName.
// For example, if you want to change the line "2015;76" into "2015;01" of the pollution.csv file, call change_data("pollution", "2015", "01");
// By calling change_data("pollution", "2015", "01");, you will change all lines of the form "2015;**" into "2015;01".
// If no line in the file is of the form "2015;**", calling change_data("pollution", "2015", "01") won't do anything.
void change_data(std::string documentName, std::string dataToChange, std::string newValue) {
    fstream file;
    fstream temp;
    std::string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        std::string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            std::string sub = line.substr(0, pos);
            if (sub == dataToChange) {
                temp << sub << ";" << newValue << '\n';
            } else {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}

// Function to delete a line of the csv file named documentName.
// By calling delete_line("pollution", "2015");, you will delete all lines of the form "2015;**".
// If no line in the file is of the form "2015;**", calling delete_line("pollution", "2015") won't do anything.
void delete_line(std::string documentName, std::string dataToDelete) {
    fstream file;
    fstream temp;
    std::string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        std::string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            std::string sub = line.substr(0, pos);
            if (sub != dataToDelete) {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}



void City::write_stat_history_to_file() {

    Array newCarbonEmission{};
    newCarbonEmission.push_back(return_word_date_godot());
    newCarbonEmission.push_back((int)(carbonEmission + 0.5));

    if (statsCarbonEmission.size() > 1462) { //4years
            statsCarbonEmission.pop_front();
        }
    statsCarbonEmission.push_back(newCarbonEmission);


    Array newIncome{};  //GDP
    newIncome.push_back(return_word_date_godot());
    newIncome.push_back((int)(income + 0.5));

    if (statsIncome.size() > 1462) { //4years
            statsIncome.pop_front();
        }
    statsIncome.push_back(newIncome);


    Array newEnergy{};
    newEnergy.push_back(return_word_date_godot());
    newEnergy.push_back((int)(energyDemand + 0.5));
    newEnergy.push_back((int)(energySupply + 0.5));

    if (statsEnergy.size() > 1462) { //4years
            statsEnergy.pop_front();
        }
    statsEnergy.push_back(newEnergy);


    Array newUnemployment{};
    newUnemployment.push_back(return_word_date_godot());
    newUnemployment.push_back((int)(100-100*(numberOfEmployees/population) + 0.5));

    if (statsUnemployment.size() > 1462) { //4years
            statsUnemployment.pop_front();
        }
    statsUnemployment.push_back(newUnemployment);


    Array newTotalSatisfaction{}; //to be finished
    newTotalSatisfaction.push_back(return_word_date_godot());
    newTotalSatisfaction.push_back((int)(10*totalSatisfaction + 0.5));

    if (statsTotalSatisfaction.size() > 1462) { //4years
            statsTotalSatisfaction.pop_front();
        }
    statsTotalSatisfaction.push_back(newTotalSatisfaction);

    Array newPopulation{};
    newPopulation.push_back(return_word_date_godot());
    newPopulation.push_back((int)(population + 0.5));

    if (statsPopulation.size() > 1462) { //4years
            statsPopulation.pop_front();
        }
    statsPopulation.push_back(newPopulation);

}



double City::return_income() {
    return income;
}

double City::return_totalSatisfaction() {
    return totalSatisfaction;
}

double City::return_numberOfEmployees() {
    return numberOfEmployees;
}

double City::return_carbonEmission() {
    return carbonEmission;
}

double City::return_energyDemand() {
    return energyDemand;
}

double City::return_energySupply() {
    return energySupply;
}

void City::remove_type_car(int type){
    if (current_car_quantities[type] > 0) {
        current_car_quantities[type] -= 1;
    }
}

int City::most_missing_type() {
    int min = 0;
    std::cout << "missing_car_quantities   ";
    for (int i = 0; i < 8; i++) {
        if(missing_car_quantities[min] > missing_car_quantities[i]){
            min = i;
        }
        std::cout << missing_car_quantities[i] << "  ";
    }
    if (missing_car_quantities[min] < 0) {
        missing_car_quantities[min] += 1;
        return min;
    }
    else {
        return -1;
    }

}

void City::transport_to_add() { //now the old finction transport_probabilities updates the missing_car_quantities with the relavent numbers
                               // the finction goes through all the buidlings  to get the wage as income
  /*
* 0 - electic car
* 1 - big american car
* 2 - normal car
* 3 - old collection cars
* 4 - bike
* 5 - motorcycle
* 6 - bus
* 7 - sports car
*/
    std::random_device rd;
    std::mt19937 gen(rd());
	Transport electicCar = Transport(0);
	Transport bigCar = Transport(1);
	Transport car = Transport(2);
	Transport collectionCar = Transport(3);
	Transport bike = Transport(4);
	Transport motorcycle = Transport(5);
	Transport bus = Transport(6);
	Transport sportsCar = Transport(7);

    double satisfactions[8] = { electicCar.satisfaction * ((double)(7 - carProhibition) / 7), bigCar.satisfaction * ((double)(7 - carProhibition) / 7), car.satisfaction * ((double)(7 - carProhibition) / 7), collectionCar.satisfaction * ((double)(7 - carProhibition) / 7), bike.satisfaction * pow(((double)(1+ carProhibition) / 7), 0.2), motorcycle.satisfaction , bus.satisfaction, sportsCar.satisfaction * ((double)(7 - carProhibition) / 7) };
    double satisfactionsSum = 0;
    double alpha[8] = { 0 };

	for (int i =0 ; i < 8; i++){
		satisfactionsSum += satisfactions[i];
	}
   
    airQuality = pow(1.01, -carbonEmission/ (all_structures.size() * 30 * 30 * 10));

    std::cout << "Current air quality is: " << airQuality << std::endl;

    
    for (int i = 0; i < 8; i++) {
        alpha[i] = (satisfactions[i] / satisfactionsSum) * all_houses.size();
        if (i == 4) {
            alpha[i] *= sqrt(airQuality);
        }   
    }

    double costs[8] = { electicCar.cost - electricCarSubsidy , bigCar.cost + weightTax*bigCar.weight, car.cost,collectionCar.cost + weightTax * collectionCar.weight , bike.cost - bikeSubsidy , motorcycle.cost, bus.cost - busSubsidy, sportsCar.cost };
    double pricesPerMonth[8] = {electicCar.pricePerMonth, bigCar.pricePerMonth+fuelTax*bigCar.fuelInput*30, car.pricePerMonth+fuelTax*car.fuelInput*30,collectionCar.pricePerMonth+fuelTax*collectionCar.fuelInput*30, bike.pricePerMonth, 
	motorcycle.pricePerMonth+fuelTax*motorcycle.fuelInput*30, bus.pricePerMonth/(bus.capacity*30)+fuelTax*bus.fuelInput*30, sportsCar.pricePerMonth+fuelTax*sportsCar.fuelInput*30};
    double probabilities[8] = {0};
    double quantities[8] = { 0 };
    double alphaSum = 0;

    for (int i = 0; i < 8; i++) {
        alphaSum += alpha[i];
    }
	for (int i = 0;i<8; i++){
   		alpha[i] =0.01*alpha[i]/alphaSum;
	}
    alphaSum = 0;
    for (int i = 0; i < 8; i++) {
        alphaSum += alpha[i];
    }

    for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it) {
 
        double choice[8] = { 0 };
        for (int i = 0; i < 8; i++) {

            probabilities[i] = fmax(normalGenerator(alpha[i], all_houses.size() / 100), 0.01) * (30*((double)((*it)->get("housingIncome")) / pricesPerMonth[i])) / alphaSum;
            std::cout << "Housing Incomr is : " << (double)((*it)->get("housingIncome")) << std::endl;
            if (probabilities[i] > 1) {
                choice[i] = alpha[i];
            }
            else {
                choice[i] = 0;
            }
        }
        int maxIndex = 0;
        double maxChoice = choice[0];
        for (int i = 1; i < 8; i++) {
            if (choice[i] > maxChoice) {
                maxChoice = choice[i];
                maxIndex = i;
            }
        }
        quantities[maxIndex] += 1;
    }

    double quantitiesSum = 0;
    for (int i = 0; i < 8; i++) {
        quantitiesSum += quantities[i];
    }
    std::cout << quantitiesSum << endl;

    // NO NEED FOR THOSE
    probabilityElectricCar = quantities[0] / quantitiesSum;
    probabilityBigCar = quantities[1] / quantitiesSum;
    probabilityCar = quantities[2] / quantitiesSum;
    probabilityCollectionCar = quantities[3] / quantitiesSum;
    probabilityBike = quantities[4] / quantitiesSum;
    probabilityMotorcycle = quantities[5] / quantitiesSum;
    probabilityBus = quantities[6] / quantitiesSum;
    probabilitySportsCar = quantities[7] / quantitiesSum;

    /*
    for (int i = 0; i < 8; i++) {
        quantities[i] = quantities[i] / quantitiesSum;
    }
    */
    for (int i = 0; i < 8; i++) {
        missing_car_quantities[i] = current_car_quantities[i] - quantities[i];

        if (missing_car_quantities[i] > 0) {
            missing_car_quantities[i] = 0;
        }
    } 
}

//generate random alphas for previous function
double City::normalGenerator(double mean, double stdDev){
    static default_random_engine generator(time(0));
    normal_distribution<double> distribution(mean,stdDev);
    return distribution(generator); 
}
// auxiliary function to be able to have values between 1 and 100 in the pie charts
// 100000 pour power demand et 1 million pour C02
int City::value_pie_chart_C02(int value, int growth) {
    int newvalue = (int)(100 * (1- pow(value/growth,-1)));
    return newvalue;
}

void City::change_pie_chart(int value, NodePath name, bool isPositive)
{
    TextureProgress* node = ((TextureProgress*)this->get_parent()->get_child(1)->get_node("Infographics")->get_node(name));
    if (isPositive) {
        node->set_tint_progress(Color(min(2 - (double)value / 50, 1.0), min((double)value / 50, 1.0), 0, 1.0));
    }
    else {
        node->set_tint_progress(Color(min((double)value / 50, 1.0), min(2 - (double)value / 50, 1.0), 0, 1.0));  
    }
    node->set("value", value);
}

void City::change_pie_label(int value, NodePath name){
	Label* label = ((Label*)this->get_parent()->get_child(1)->get_node("Infographics")->get_node(name)->get_child(1));
	std::string standardString = std::to_string((int)value);
    godot::String godotString = godot::String(standardString.c_str());
	label->set("text", godotString);
}




// auxiliary function to simplify ring city generation

float City::calculate_building_prob(float roota, float rootb, float proportion, double dist) {
    if (dist < (citysize*roota/20) || dist > (citysize*rootb/20)) { return 0; }
    else {

        // function which calculates coefficients of quadratic equation for a building distribution
        double a = double(proportion / (((rootb - roota) / 6) * (-pow(rootb, 2) - pow(roota, 2) + 2 * roota * rootb)));

        double b = double(-(roota + rootb) * a);

        double c = double(a * roota * rootb);

        // returns specific probability 
        return float(a * pow(((20*dist/(citysize))), 2) + b * (20 * dist / (citysize)) + c);


        // test line to verify that the integral is the proportion

        // return float((a / 3) * (pow(rootb, 3) - pow(roota, 3)) + (b / 2) * (pow(rootb, 2) - pow(roota, 2)) + (rootb - roota) * c);
    }


}


double City::return_unemployment_rate() {
    if (population != 0) {
        return (1 - this->numberOfEmployees / this->population);
    }
    else { return 0; }
}




//in order to check for errors on mac
int main() {
    City c = City();
	while (true) {
    	c._physics_process(1);
	}
    std::cout << "DEBUG: TOTAL CARBON EMISSION = " << c.return_carbonEmission() << std::endl;
    return 0;
}