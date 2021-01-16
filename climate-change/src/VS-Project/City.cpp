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
#include <Godot.hpp>
#include <Node2D.hpp>

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
    totalSatisfaction = 0;

    time_speed = 1;

    //timer = 0;
    day_tick = 0;
    days_since_last_simulation = 0;

    // in order to find date
    daycount = 0;
    
    //policies for transport
    fuelTax = 0;
    weightTax = 0;
    bikeSubsidy = 0;
    electricCarSubsidy = 0;
    busSubsidy = 0;
    carProhibition = 0;
    srand((int)time(0));

    // boolean for 3d menu button
    bool MenuVisibility = false;
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
    register_method((char*)"_input", &City::_input);
    register_method((char*)"_ready", &City::_ready);
    register_method((char*)"_on_Menu_pressed", &City::_on_Menu_pressed);

    register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
    register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);
    register_method((char*)"_on_ResetButton_pressed", &City::_on_ResetButton_pressed);
    register_method((char*)"_on_Reset_confirmed", &City::_on_Reset_confirmed);
    register_method((char*)"_on_Reset_cancelled", &City::_on_Reset_cancelled);
    register_method((char*)"_on_ExitButton_pressed", &City::_on_ExitButton_pressed);
    register_method((char*)"_on_3dButton_pressed", &City::_on_3dButton_pressed);
    register_method((char*)"_on_Exit_confirmed", &City::_on_Exit_confirmed);
    register_method((char*)"_on_Exit_cancelled", &City::_on_Exit_cancelled);
    register_method((char*)"_on_TransportMenuButton_pressed", &City::_on_TransportMenuButton_pressed);
    register_method((char*)"_on_GraphsButton_pressed", &City::_on_GraphsButton_pressed);
    register_method((char*)"_on_GraphsExit_pressed", &City::_on_GraphsExit_pressed);

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
    register_property<City, double>("budget", &City::set_budget, &City::get_budget, 10000);

    //Transport policies methods :
    register_property<City, double>("fuelTax", &City::fuelTax, 0.0);
    register_property<City, double>("weightTax", &City::weightTax, 0.0);
    register_property<City, double>("bikeSubsidy", &City::bikeSubsidy, 0.0);
    register_property<City, double>("electricCarSubsidy", &City::electricCarSubsidy, 0.0);
    register_property<City, double>("busSubsidy", &City::busSubsidy, 0.0);
    register_property<City, double>("carProhibition", &City::carProhibition, 0.0);

    register_property<City, double>("income", &City::income, 0.0);
    register_property<City, int>("population", &City::population, 0);
    register_property<City, double>("numberOfEmployees", &City::numberOfEmployees, 0.0);
    register_property<City, double>("carbonEmission", &City::carbonEmission, 0.0);
    register_property<City, double>("energyDemand", &City::energyDemand, 0.0);
    register_property<City, double>("energySupply", &City::energySupply, 0.0);
    register_property<City, int>("totalSatisfaction", &City::totalSatisfaction, 0.0);
    register_property<City, double>("environmentalCost", &City::environmentalCost, 0.0);

    //statistics:
    register_property<City, Array>("statsCarbonEmission", &City::statsCarbonEmission, {});
    register_property<City, Array>("statsCarbonEmissionSplit", &City::statsCarbonEmissionSplit, {});
    register_property<City, Array>("statsIncome", &City::statsIncome, {});
    register_property<City, Array>("statsEnergy", &City::statsEnergy, {});
    register_property<City, Array>("statsUnemployment", &City::statsUnemployment, {});
    register_property<City, Array>("statsTotalSatisfaction", &City::statsTotalSatisfaction, {});
    register_property<City, Array>("statsPopulation", &City::statsPopulation, {});
    register_property<City, Array>("statsEnvironmentalCost", &City::statsEnvironmentalCost, {});

    register_property<City, double>("HousingCO2", &City::HousingCO2, 0.0);
    register_property<City, double>("EnergyCO2", &City::EnergyCO2, 0.0);
    register_property<City, double>("ShopsCO2", &City::ShopsCO2, 0.0);
    register_property<City, double>("ProductionCO2", &City::ProductionCO2, 0.0);
    register_property<City, double>("TransportCO2", &City::TransportCO2, 0.0);

    register_property<City, Array>("statsCarbonEmissionHousing", &City::statsCarbonEmissionHousing, {});
    register_property<City, Array>("statsCarbonEmissionShops", &City::statsCarbonEmissionShops, {});
    register_property<City, Array>("statsCarbonEmissionEnergy", &City::statsCarbonEmissionEnergy, {});
    register_property<City, Array>("statsCarbonEmissionProduction", &City::statsCarbonEmissionProduction, {});
    register_property<City, Array>("statsCarbonEmissionTransport", &City::statsCarbonEmissionTransport, {});

    //powerplant method register: 
     register_property<City, int>("workingPower", &City::set_workingPower, &City::get_workingPower, 0);

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

        add_car(Vector3(-1,-1,-1)); // specific case, where cars are randomly assigned

        (this->simulation_counter) -= 5;

        std::cout << "income = " << (double)(this->get("income")) << std::endl;
        std::cout << "population = " << (double)(this->get("population")) << std::endl;
        std::cout << "numberOfEmployees = " << (double)(this->get("numberOfEmployees")) << std::endl;
        std::cout << "carbonEmission = " << (double)(this->get("carbonEmission")) << std::endl;
        std::cout << "energyDemand = " << (double)(this->get("energyDemand")) << std::endl;
        std::cout << "environmentalCost = " << (double)(this->get("environmentalCost")) << std::endl;
      
        change_pie_chart( (int)(10 * return_totalSatisfaction()), "PieSatisfaction", true);
        change_pie_chart(value_pie_chart_C02(carbonEmission, 20000), "PieCO2", false);
        change_pie_chart(income / (numberOfEmployees * 30), "PieIncome", true);
        change_pie_chart(value_pie_chart_C02(budget, pow(10, 4)), "PieBudget", true);
        change_pie_chart(4*(100 - 100 * numberOfEmployees / population), "PieUnemployement", false); //EnergyDemand variable is temporary
        change_pie_chart(value_pie_chart_C02(energyDemand / all_structures.size(), 25000), "PiePowerDemand", false);

        change_pie_label( (int)(10 * return_totalSatisfaction()), "PieSatisfaction");
        change_pie_label(carbonEmission, "PieCO2");
        change_pie_label(income / numberOfEmployees, "PieIncome");
        change_pie_label(budget, "PieBudget");
        change_pie_label(fmax(100 - 100 * numberOfEmployees / population, 0), "PieUnemployement");
        change_pie_label(energyDemand / all_structures.size(), "PiePowerDemand");

    }

    if (this->date_counter > 1)
    {
        // CALLED EVERY GAME DAY

        /*
        for (int i = 0; i < statsCarbonEmission.size(); ++i) {
            Godot::print(String(statsCarbonEmission[i]));
        }
        Godot::print("Array should be printed now");
        */

        day_tick++;
        std::cout << "Day tick : " << (this->day_tick) << endl;
        this->get_tree()->get_root()->get_node("Main/2Dworld/Date")->set("text", return_word_date_godot());


        int* datenumber = return_date(int(this->day_tick));

        if (datenumber[0] == 1 || datenumber[0] == 15) {
            write_stat_history_to_file();
        }


        if (datenumber[0] == 1)
        {
            // CALLED EVERY MONTH
            transport_to_add();
        }

        if (datenumber[0] == 1 && datenumber[1] == 1)
        {
            // CALLED EVERY YEAR

            if (budget < 0 && !(under_budget)) {
                this->trigger_notification(String("You have gone into debt ! This is a serious issue. If you are not in the black next year, your term as mayor will be abruptly ended!"));
                under_budget = true;
            }

            if (under_budget) {
                if (budget >= 0) {
                    under_budget = false;  this->trigger_notification(String("You have gone climbed out of debt ! Congratulations ! Does this mean re-election?"));
                }
                else {
                    this->_on_Reset_confirmed(); this->trigger_notification(String("Game over ! You overspent the town's budget in two consecutive years. Better luck this time."));
                }
            }

            if (!(factoryyearsubsidy) && (yearlyfactorysubsidy > 0)) {   // take off factory subsidies if they havent been changed
                for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
                {

                    if (((*it)->get("object_type")).operator String() == (String)("Goods Factory")) {

                        budget -= yearlyfactorysubsidy;

                    }
                }
                this->trigger_notification(String("The goods factory subsidies were taken from the budget. "));
            }
            if (factoryyearsubsidy) { factoryyearsubsidy = false; }

            this->budget += 1000 * all_structures.size();
        }

        (this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment")->get_node("DirectionalLight"))->set("rotation_degrees", Vector3(-45 - sin((M_PI * (2 * (double)(day_tick % 365)) / 365)) * 25 / 2 - 12.5, 45, 0));
        ((WorldEnvironment*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->get_node("WorldEnvironment")))->set("fog_color", Color(0.729412, 0.796078, 0.862745, fmax(0.99 - airQuality, 0) * 0.11));

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


void City::_input(InputEvent*)
{

    Input* i = Input::get_singleton();


    if (i->is_action_pressed("ui_test"))
    {
        transport_to_add();
        add_car();
    }

    if (i->is_action_pressed("ui_turn") && !(this->get_tree()->get_root()->get_node("Main/2Dworld/TabContainer")->get("visible")))
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


    }

    if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->get("visible")) {
        this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
        this->notification_active = false;
        this->notification_counter = 0;
        

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

    //booleans to ensure correct city creation
    bool hasnuclear = false;
    bool hascoal = false;
    bool hasgeo = false;
    bool minimumonefactory = false;
    bool minimumonemall = false;
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

                // 2x2 buildings
                float mallprob = calculate_building_prob(40, 90, 0.35, dist);
                float nuclearprob = calculate_building_prob(140, 180, 0.8, dist);
                float coalprob = calculate_building_prob(140, 180, 0.5, dist);
                float geoprob = calculate_building_prob(140, 180, 0.5, dist);
                float fieldprob = calculate_building_prob(150, 320, 1.25, dist);
                float pastureprob = calculate_building_prob(150, 320, 2, dist);
                float factoryprob = calculate_building_prob(100, 200, 0.4, dist);

                if (hasgeo) { geoprob = 0; }
                if (hasnuclear) { nuclearprob = 0; }
                if (hascoal) { coalprob = 0; }


                // 1 x 1 buildings

                dist = pos.distance_to(center);

                float restaurantprob = calculate_building_prob(0, 150, 1.2, dist) + calculate_building_prob(200, 260, 0.1, dist);
                float shopprob = calculate_building_prob(0, 170, 1.3, dist) + calculate_building_prob(200, 260, 0.15, dist);
                float buildingprob = calculate_building_prob(125, 170, 1.2, dist) + calculate_building_prob(-150, 150, 1, dist);
                float windmillprob = calculate_building_prob(40, 160, 0.02, dist) + calculate_building_prob(200, 270, 0.15, dist);
                float lowhouseprob = calculate_building_prob(-200, 170, 5, dist) + calculate_building_prob(200,260,0.12, dist);
                float highhouseprob = calculate_building_prob(-140, 170, 4, dist) + calculate_building_prob(200, 260, 0.35, dist);


                //std::cout << "DEBUG: highhouseprob  : " << highhouseprob << std::endl;
                //std::cout << "DEBUG: lowhouseprob  : " << lowhouseprob << std::endl;
                //std::cout << "DEBUG: restaurantprob  : " << restaurantprob << std::endl;


                float smallerprob = restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob;


                //std::cout << "DEBUG: done calculate probability" << std::endl;

                double bigbuildingmaybe = (double((double)rand() / (double)RAND_MAX) * double((mallprob + coalprob + geoprob + nuclearprob + fieldprob + factoryprob + pastureprob + smallerprob)));
                

                if (!(minimumonefactory) && factoryprob != 0) {  add_production(bigbuildingpos, FactoryScene); minimumonefactory = true; }
                else if (!(minimumonemall) && mallprob != 0) { add_shop(bigbuildingpos, MallScene); minimumonemall = true; }
                else if (bigbuildingmaybe < (double)(mallprob)) {   add_shop(bigbuildingpos, MallScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob)) {  add_energy(bigbuildingpos, NuclearPowerPlantScene); hasnuclear = true; }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob)) {  add_production(bigbuildingpos, FieldScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob / 2)) {   add_production(bigbuildingpos, SheepPastureScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob)) {   add_production(bigbuildingpos, PigsPastureScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob)) {   add_production(bigbuildingpos, FactoryScene); }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob + geoprob)) {   add_energy(bigbuildingpos, GeothermalPowerPlantScene); hasgeo = true; }
                else if (bigbuildingmaybe < (double)(mallprob + nuclearprob + fieldprob + pastureprob + factoryprob + geoprob + coalprob)) {   add_energy(bigbuildingpos, CoalPowerPlantScene); hascoal = true; }
                else {

                   //  in the case of a 1x1 building, plop down 4 1x1 in the slot



                    srand(int((x + 1) * (z + 1) * (int)(time(0))));

                    for (int x1 = 0; x1 < 2; x1++)
                    {
                        for (int z1 = 0; z1 < 2; z1++) {

                            Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);

                            restaurantprob = calculate_building_prob(0, 150, 1.2, dist) + calculate_building_prob(200, 260, 0.1, dist);
                            shopprob = calculate_building_prob(0, 170, 1.3, dist) + calculate_building_prob(200, 260, 0.15, dist);
                            buildingprob = calculate_building_prob(125, 170, 1.2, dist) + calculate_building_prob(-150, 150, 1, dist);
                            windmillprob = calculate_building_prob(40, 160, 0.02, dist) + calculate_building_prob(200, 270, 0.15, dist);
                            lowhouseprob = calculate_building_prob(-200, 170, 5, dist) + calculate_building_prob(200, 260, 0.12, dist);
                            highhouseprob = calculate_building_prob(-140, 170, 4, dist) + calculate_building_prob(200, 260, 0.35, dist);


                            double type = (double((double)rand() / (double)RAND_MAX) * (restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob));

                            if (type < (double)(restaurantprob)) {
                                add_shop(pos + pos1, RestaurantScene);
                                int randompdint = rand() % 3;
                                if (randompdint == 0) {
                                    if (PDScene.is_valid()) {
                                        
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
        }
    }
    trees_iterator = trees_vector.begin();
    transport_to_add();

    for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
    {
        int randomcarint = rand() % 2;
        if (randomcarint == 0) { std::cout << "DEBUG: adding car" << std::endl;  add_car((Vector3)((Node*)(*it))->get("translation")); }
    }


    //   shuffles all the vectors so that buildings arent updated in a noticeable pattern
    std::random_shuffle(all_structures.begin(), all_structures.end());
    std::random_shuffle(all_energies.begin(), all_energies.end());
    std::random_shuffle(all_houses.begin(), all_houses.end());
    std::random_shuffle(all_shops.begin(), all_shops.end());
    std::random_shuffle(all_production.begin(), all_production.end());

    std::random_shuffle(trees_vector.begin(), trees_vector.end());

    



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
                if ((*it)->get_object_type()=="Building") {
                    this->population -= (int)(*it)->get("numberOfInhabitants");
                    (*it)->set("numberOfInhabitants", (int)(*it)->get("numberOfInhabitants") - (int)(rand() % 2));
                    this->population += (int)(*it)->get("numberOfInhabitants");
                }
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

    this->get_tree()->get_root()->get_node("Main/2Dworld/TabContainer")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/GraphsExit")->set("visible", false);

}




void City::_ready()
{
    static default_random_engine generator(time(0));
    //std::cout << "DEBUG: Ready started" << std::endl;
    // citysize = 10;

    this->initialize_stats();

    this->generate_initial_city_graphics();
    structures_iterator = all_structures.begin();
    this->set_initial_visible_components();

    this->_on_Game_Speed_changed();

}

void City::initialize_stats() {
    // initialize stats arrays :
    Array titleCarbonEmission{};
    titleCarbonEmission.push_back(String("Date"));
    titleCarbonEmission.push_back(String("Carbon emissions in thousands of tons"));
    statsCarbonEmission.push_back(titleCarbonEmission);

    Array titleCarbonEmissionSplit{};
    titleCarbonEmissionSplit.push_back(String("Date"));
    titleCarbonEmissionSplit.push_back(String("Carbon emissions from Housing sector in thousands of tons"));
    titleCarbonEmissionSplit.push_back(String("Carbon emissions from Shop sector in thousands of tons"));
    titleCarbonEmissionSplit.push_back(String("Carbon emissions from Energy sector in thousands of tons"));
    titleCarbonEmissionSplit.push_back(String("Carbon emissions from Production sector in thousands of tons"));
    titleCarbonEmissionSplit.push_back(String("Carbon emissions from Transport sector in thousands of tons"));
    statsCarbonEmissionSplit.push_back(titleCarbonEmissionSplit);

    Array titleEnvironmentalCost{};
    titleEnvironmentalCost.push_back(String("Date"));
    titleEnvironmentalCost.push_back(String("Environmental cost in millions of euros"));
    statsEnvironmentalCost.push_back(titleEnvironmentalCost);

    Array titleIncome{};
    titleIncome.push_back(String("Date"));
    titleIncome.push_back(String("Budget in thousands of euros"));
    statsIncome.push_back(titleIncome);

    Array titleEnergy{};
    titleEnergy.push_back(String("Date"));
    titleEnergy.push_back(String("Energy usage in GWh"));
    statsEnergy.push_back(titleEnergy);

    Array titleUnemployment{};
    titleUnemployment.push_back(String("Date"));
    titleUnemployment.push_back(String("Unemployment in %"));
    statsUnemployment.push_back(titleUnemployment);

    Array titleTotalSatisfaction{};
    titleTotalSatisfaction.push_back(String("Date"));
    titleTotalSatisfaction.push_back(String("Total satisfaction in the city from 0 to 10"));
    statsTotalSatisfaction.push_back(titleTotalSatisfaction);

    Array titlePopulation{};
    titlePopulation.push_back(String("Date"));
    titlePopulation.push_back(String("Population in number of people"));
    statsPopulation.push_back(titlePopulation);

}

void City::_on_GraphsButton_pressed()
{
    this->_on_Reset_cancelled();
    this->_on_Exit_cancelled();
    this->time_speed = 0;
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);
    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/GraphsExit")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/TabContainer")->set("visible", true);

}

void City::_on_GraphsExit_pressed() {

    this->get_tree()->get_root()->get_node("Main/2Dworld/GraphsExit")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/TabContainer")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
    this->_on_Game_Speed_changed();
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


}

void City::_on_3dButton_pressed()
{
    this->get_parent()->get_node("Main/2Dworld")->set("visible", MenuVisibility);
    MenuVisibility = !MenuVisibility;
}

void City::_on_Exit_cancelled()
{
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);


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


}

void City::_on_Reset_cancelled()
{
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);

    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);
    this->_on_Game_Speed_changed();
}

void City::_on_Reset_confirmed()
{
    // RESET GAME SETTINGS
    this->time_speed = 1.0;
    this->day_tick = 0;
    this->set_budget(10000);

    // RESET TRANSPORT POLICIES
    this->fuelTax = 0;
    this->weightTax = 0;
    this->bikeSubsidy = 0;
    this->electricCarSubsidy = 0;
    this->busSubsidy = 0;
    this->carProhibition = 0;

    // RESET OTHER POLICIES
    this->set("workingPower", 0);

    // RESET STATS
    HousingCO2 = 0;
    ShopsCO2 = 0;
    ProductionCO2 = 0;
    EnergyCO2 = 0;
    TransportCO2 = 0;
    income = 0;
    population = 0;
    numberOfEmployees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
    environmentalCost = 0;
    totalSatisfactioWeight = 0;
    totalSatisfaction = 0;

    // RESET ARRAYS
    statsCarbonEmissionHousing = { };
    statsCarbonEmissionProduction = { };
    statsCarbonEmissionEnergy = { };
    statsCarbonEmissionShops = { };
    statsEnvironmentalCost = { };
    statsCarbonEmission = {};
    statsCarbonEmissionSplit = {};
    statsEnvironmentalCost = {};
    statsIncome = { };
    statsEnergy = { };
    statsUnemployment = { };
    statsTotalSatisfaction = { };
    statsPopulation = { };

    // RELOAD SCENE
    this->get_tree()->reload_current_scene();
}



void City::_on_TransportMenuButton_pressed()
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);

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
    String transportInfo = String("TRANSPORT INFORMATION") + String("\n");
    transportInfo += String("\n");
    transportInfo += String("Your city contains:") + String("\n");
    transportInfo += String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[0])) + String(" Electric Cars") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[1])) + String(" American Cars") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[2])) + String(" Normal Cars") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[3])) + String(" Collection Cars") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[4])) + String(" Bicycles") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[5])) + String(" Motorcycles") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[6])) + String(" Bus") + String("\n");
    transportInfo += to_godot_string((int)(current_car_quantities[7])) + String(" Sports Cars") + String("\n");

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("text", transportInfo);

    this->time_speed = 0;
    this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", true);
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);
    
    this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuTransport")->set("visible", true);
}


void City::_on_Menu_pressed(String name)
{

    active_button = name;

    hide_menus();

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);


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
    else if (this->active_button == String("WindTurbinesShop"))
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
        return String("This is a subsidy for green factories, promoting the reduction of harmful chemicals and heavy metals emissions. Please input a value between 1000 and 100 000 euros per factory per year. You will not be able to change this value until next year!");
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

}

void City::_on_Validate_pressed()
{

    this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
    String mytext = this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->get("text");
    this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
    this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);


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
            this->trigger_notification(String("This solar panel subsidy has been implemented. This will push businesses to buy solar panels."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("WindTurbinesShop")) {    ///I can Changed the name here
        if (value >= 0 && value <= 800) {
            Godot::print("ROOFTOP WINDTURBINES SUBSIDIES ON SHOPS IMPLEMENTED");
            for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
            {
                //if ((String)(*it)->get("object_type") == (String)("Restaurant")) {}
                (*it)->set("wind_turbine_subsidies", value);

            }
            this->trigger_notification(String("This wind turbine subsidy has been implemented. This will push businesses to utilize wind energy."));
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
            this->trigger_notification(String("This wind turbine subsidy has been implemented. This will push citizens to harness wind energy."));
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
                if (((*it)->get("object_type")).operator String() == (String)("Coal Power Plant")) {
                    (*it)->set("efficiency_supercritical", value);
                }
            }
            this->trigger_notification(String("Thanks to your city's expertise, the coal plant has now reached super critical efficiency levels."));
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
                if (((*it)->get("object_type")).operator String() == (String)("Coal Power Plant")) {
                    (*it)->set("efficiency_cogeneration", value);
                }
            }
            this->trigger_notification(String("Policy implemented !"));
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
            this->trigger_notification(String("The sun has set on the city's nuclear power plant. Will citizens appreciate this move?"));
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
            this->trigger_notification(String("The sun has set on the city's coal power plant. Another blow for the fossil fuel industry."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("MaximumCarbonFactories")) {
        if (42 >= value && value > 1) {
            Godot::print("MAXIMUM EMISSIONS ON FACTORIES IMPLEMENTED");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if (((*it)->get("object_type")).operator String() == (String)("Goods Factory")) {
                    (*it)->set("maximum_CO2", value);
                }
            }
            this->trigger_notification(String("Carbon maximum policy implemented !"));
        }
        else if (value==0){ this->trigger_notification(String("Sadly, such a harsh measure was rejected by the population.")); }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("SubsidyFactories")) {
        if (((100000 >= value && value >= 1000) || value == 0) && !(factoryyearsubsidy)) {
            if ((100000 >= value && value >= 1000) || value == 0) {
                Godot::print("GREEN SUBSIDY FOR FACTORIES IMPLEMENTED");
                for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
                {

                    if (((*it)->get("object_type")).operator String() == (String)("Goods Factory")) {
                        (*it)->set("subsidy_green", value);
                        budget -= value;
                    }
                }
                factoryyearsubsidy = true;
                yearlyfactorysubsidy = (double)value;
                this->trigger_notification(String("Much to every capitalist's delight, factories are now being subsidized."));
            }
            else if (factoryyearsubsidy) {
                this->trigger_notification(String("You can only change this once a year ! You wouldn't want to confuse the markets, would you ? "));
            }
            else {
                this->trigger_notification(String("The value you provided was not in the specified range."));
            }
        }
    }
    else if (this->active_button == String("Pesticides")) {
        if (value == 1 || value == 0) {
            Godot::print("PROHIBITION OF PESTICIDES");
            for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
            {
                if (((*it)->get("object_type")).operator String() == (String)("Agricultural Production")) {
                    (*it)->set("pesticideProhibited", value);
                }
            }
            this->trigger_notification(String("Pesticides have now been prohibited ! Crickets, weevils and other pests celebrate."));
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
                if (((*it)->get("object_type")).operator String() == (String)("Agricultural Production")) {
                    (*it)->set("GMOProhibited", value);
                }
            }
            this->trigger_notification(String("GMOs have now been prohibited ! Whether this decision is popular remains to be seen."));
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
                if (((*it)->get("object_type")).operator String() == (String)("Agricultural Production")) {
                    (*it)->set("fertilizerProhibited", value);
                }
            }
            this->trigger_notification(String("Synthetic fertilizers have now been prohibited ! Farmers across the city's outskirts scratch their heads."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("SolarHousing")) {
        if (value >= 0 && value <= 450) {
            Godot::print("SOLAR PANELS SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                (*it)->set("solar_panel_subsidies_housing", value);
            }
            this->trigger_notification(String("You have changed the incentive for citizens to buy solar panels for their homes."));
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
            this->trigger_notification(String("You have changed the incentive for citizens to buy wind turbines for their homes."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("DoubleGlazingHousing")) {
        if (value >= 0 && value <= 1000) {
            Godot::print("DOUBLE GLAZING SUBSIDIES ON HOUSING IMPLEMENTED");
            for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
            {
                (*it)->set("double_glazing_subsidies", value);
            }
            this->trigger_notification(String("Double glazing policy implemented. This is quite costly, but only well off, modern houses will install them."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("ConsumptionTransport")) {
        if (value >= 0 && value <= 2) {
            Godot::print("TAX ON CAR CONSUMPTION IMPLEMENTED");
            fuelTax = value;
            if (value > 0) { this->trigger_notification(String("Fuel tax implemented ! Yellow jackets are being bought up in shops around the city.")); }
            else { this->trigger_notification(String("You have completely eliminated the fuel tax. Automobilists and Formula 1 enthusiasts rejoice.")); }
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("WeightTransport")) {
        if (value >= 0 && value <= 5000) {
            Godot::print("TAX ON CAR WEIGHT IMPLEMENTED");
            weightTax = value;
            this->trigger_notification(String("Heavy cars will now be taxed higher. Pickup truck drivers are up in arms."));
        }

        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("BikeSubsidy")) {
        if (value >= 0 && value <= 250) {
            Godot::print("BIKE SUBSIDY IMPLEMENTED");
            bikeSubsidy = value;
            this->trigger_notification(String("Bike subsidy implemented. Two wheelers are now increasingly common on the city sidewalks and roads."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("ElectricSubsidy")) {
        if (value >= 0 && value <= 30000) {
            Godot::print("ELECTRIC CAR SUBSIDY IMPLEMENTED");
            electricCarSubsidy = value;
            this->trigger_notification(String("Policy implemented. Some citizens complained it still wasn't enough to buy the new Tesla."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("BusSubsidy")) {
        if (value >= 0 && value <= 60000) {
            Godot::print("BUS SUBSIDY IMPLEMENTED");
            busSubsidy = value;
            this->trigger_notification(String("Bus incentives implemented."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("CarsProhibition")) {
        if (value >= 0 && value <= 7) {
            Godot::print("CARS PROHIBITION ON CERTAIN DAYS IMPLEMENTED");
            carProhibition = value;
            this->trigger_notification(String("The rules about car circulation have been implemented! Commuters are angered."));
        }
        else {
            this->trigger_notification(String("The value you provided was not in the specified range."));
        }
    }
    else if (this->active_button == String("TreesBudget")) {
        if (trees_iterator == trees_vector.end()) { this->trigger_notification(String("All possible buildings now have trees ! This is a good thing. However, you cannot add any more.")); }
        else if (value >= 0) {
            Godot::print("TREES ARE BEING ADDED");
            this->houses_with_trees += int(value);

            for (int i = 0; i < int(value); ++i) {
                (*trees_iterator)->get_node("MeshComponents/Trees")->set("visible", true);
                (*trees_iterator)->set("satisfaction", (double)(*trees_iterator)->get("satisfaction") + 4);
                this->totalSatisfaction += 20/((double)(this->all_houses.size() + this->all_shops.size()));
                trees_iterator++;
                budget -= 5000;
                houses_with_trees++;
                if (trees_iterator == trees_vector.end()) { this->trigger_notification(String("All possible buildings now have trees ! This is a good thing. However, you cannot add any more.")); break; }
            }
            this->trigger_notification(String("Trees have been added to that many buildings. What a happy sight."));
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

    if (pos == Vector3(-1, -1, -1)) {
        int chosenhouse = rand() % all_houses.size();
        std::vector<Housing*>::iterator addcariterator;
        addcariterator = all_houses.begin();
        addcariterator += chosenhouse;
        pos = (Vector3)((Node*)(*addcariterator))->get("translation");

        
    }

    if (OldCarScene.is_valid() && SportCarScene.is_valid() && MotoScene.is_valid() && BusScene.is_valid() && AmericanCarScene.is_valid() && BikeScene.is_valid() && ElectricCarScene.is_valid() && NormalCarScene.is_valid())
    {
        int type = most_missing_type();
        std::cout << "The most missing type is currently : " << type << std::endl;
        if (type != -1) {

            //int type = rand() % 3;
            Node* node;
            std::cout << "INSTANCE A CAR OF TYPE: " << type << std::endl;

            switch (type) {
            case 0: node = ElectricCarScene->instance(); current_car_quantities[0] += 1; budget -= electricCarSubsidy; break;
            case 1: node = AmericanCarScene->instance(); current_car_quantities[1] += 1; break;
            case 2: node = NormalCarScene->instance(); current_car_quantities[2] += 1; break;
            case 3: node = OldCarScene->instance(); current_car_quantities[3] += 1; break;
            case 4: node = BikeScene->instance(); current_car_quantities[4] += 1; budget -= bikeSubsidy; break;
            case 5: node = MotoScene->instance(); current_car_quantities[5] += 1; break;
            case 6: node = BusScene->instance(); current_car_quantities[6] += 1; budget -= busSubsidy; break;
            case 7: node = SportCarScene->instance(); current_car_quantities[7] += 1;  break;
            default: node = SportCarScene->instance(); current_car_quantities[7] += 1;  break;
            }

            std::cout << "INSTANCE DONE A CAR OF TYPE: " << type << std::endl;
            node->set("scale", Vector3(15, 15, 15));
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
    totalSatisfactioWeight += 3;
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
        if (scene != MallScene) { trees_vector.push_back((Structure*)node); }

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

    totalSatisfactioWeight += 1;
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
    totalSatisfactioWeight += 10;
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
    totalSatisfactioWeight += 5;
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
           // std::cout << "DEBUG: updating traffic for BIG building coordinates : " << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
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
            }
            else {
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

    Array newCarbonEmissionHousing{};
    newCarbonEmissionHousing.push_back(return_word_date_godot());
    newCarbonEmissionHousing.push_back((int)(HousingCO2 / pow(10, 6) + 0.5));

    if (statsCarbonEmissionHousing.size() > 100) {
        statsCarbonEmissionHousing.remove(1);
    }
    statsCarbonEmissionHousing.push_back(newCarbonEmissionHousing);

    Array newCarbonEmissionShops{};
    newCarbonEmissionShops.push_back(return_word_date_godot());
    newCarbonEmissionShops.push_back((int)((ShopsCO2 / pow(10, 6)) + 0.5));

    if (statsCarbonEmissionShops.size() > 100) {
        statsCarbonEmissionShops.remove(1);
    }
    statsCarbonEmissionShops.push_back(newCarbonEmissionShops);

    Array newCarbonEmissionEnergy{};
    newCarbonEmissionEnergy.push_back(return_word_date_godot());
    newCarbonEmissionEnergy.push_back((int)((EnergyCO2 / pow(10, 6)) + 0.5));

    if (statsCarbonEmissionEnergy.size() > 100) {
        statsCarbonEmissionEnergy.remove(1);
    }
    statsCarbonEmissionEnergy.push_back(newCarbonEmissionEnergy);

    Array newCarbonEmissionProduction{};
    newCarbonEmissionProduction.push_back(return_word_date_godot());
    newCarbonEmissionProduction.push_back((int)((ProductionCO2 / pow(10, 6)) + 0.5));

    if (statsCarbonEmissionProduction.size() > 100) {
        statsCarbonEmissionProduction.remove(1);
    }
    statsCarbonEmissionProduction.push_back(newCarbonEmissionProduction);

    this->update_transport_emissions();

    Array newCarbonEmissionTransport{};
    newCarbonEmissionTransport.push_back(return_word_date_godot());
    newCarbonEmissionTransport.push_back((int)(((this->TransportCO2) / pow(10, 6)) + 0.5));

    if (statsCarbonEmissionTransport.size() > 100) {
        statsCarbonEmissionTransport.remove(1);
    }
    statsCarbonEmissionTransport.push_back(newCarbonEmissionTransport);



    Array newCarbonEmission{};
    newCarbonEmission.push_back(return_word_date_godot());
    newCarbonEmission.push_back((int)(((carbonEmission + (this->TransportCO2)) / pow(10, 3)) + 0.5));

    if (statsCarbonEmission.size() > 100) {
        statsCarbonEmission.remove(1);
    }
    statsCarbonEmission.push_back(newCarbonEmission);


    Array newCarbonEmissionSplit{};
    newCarbonEmissionSplit.push_back(return_word_date_godot());
    newCarbonEmissionSplit.push_back((int)(HousingCO2 / pow(10, 3) + 0.5));
    newCarbonEmissionSplit.push_back((int)(ShopsCO2 / pow(10, 3) + 0.5));
    newCarbonEmissionSplit.push_back((int)(EnergyCO2 / pow(10, 3) + 0.5));
    newCarbonEmissionSplit.push_back((int)(ProductionCO2 / pow(10, 3) + 0.5));
    newCarbonEmissionSplit.push_back((int)((this->TransportCO2) / pow(10, 3) + 0.5));

    if (statsCarbonEmissionSplit.size() > 100) {
        statsCarbonEmissionSplit.remove(1);
    }
    statsCarbonEmissionSplit.push_back(newCarbonEmissionSplit);


    Array newEnvironmentalCost{};
    newEnvironmentalCost.push_back(return_word_date_godot());
    newEnvironmentalCost.push_back((int)((environmentalCost / pow(10, 6)) + 0.5));

    if (statsEnvironmentalCost.size() > 100) {
        statsEnvironmentalCost.remove(1);
    }
    statsEnvironmentalCost.push_back(newEnvironmentalCost);


    Array newIncome{};  //GDP
    newIncome.push_back(return_word_date_godot());
    newIncome.push_back((int)((max(budget, 0.0) / pow(10, 3)) + 0.5));

    if (statsIncome.size() > 100) {
        statsIncome.remove(1);
    }
    statsIncome.push_back(newIncome);


    Array newEnergy{};
    newEnergy.push_back(return_word_date_godot());
    newEnergy.push_back((int)((energyDemand / pow(10, 6)) + 0.5));

    if (statsEnergy.size() > 100) {
        statsEnergy.remove(1);
    }
    statsEnergy.push_back(newEnergy);


    Array newUnemployment{};
    newUnemployment.push_back(return_word_date_godot());
    newUnemployment.push_back((int)(100 - 100 * fmin((double)1, (double)(numberOfEmployees / population)) + 0.5));

    if (statsUnemployment.size() > 100) {
        statsUnemployment.remove(1);
    }
    statsUnemployment.push_back(newUnemployment);


    Array newTotalSatisfaction{}; //to be finished
    newTotalSatisfaction.push_back(return_word_date_godot());
    newTotalSatisfaction.push_back( (int)(10 * return_totalSatisfaction()));

    if (statsTotalSatisfaction.size() > 100) {
        statsTotalSatisfaction.remove(1);
    }
    statsTotalSatisfaction.push_back(newTotalSatisfaction);

    Array newPopulation{};
    newPopulation.push_back(return_word_date_godot());
    newPopulation.push_back((int)(population + 0.5));

    if (statsPopulation.size() > 100) {
        statsPopulation.remove(1);
    }
    statsPopulation.push_back(newPopulation);

}



double City::return_income() {
    return income;
}

double City::return_totalSatisfaction() {
    return (double) totalSatisfaction/totalSatisfactioWeight;
}

double City::return_numberOfEmployees() {
    return numberOfEmployees;
}

double City::return_carbonEmission() {
    return carbonEmission;
}

double City::return_environmentalCost() {
    return environmentalCost;
}

double City::return_energyDemand() {
    return energyDemand;
}

double City::return_energySupply() {
    return energySupply;
}

void City::remove_type_car(int type) {
    if (current_car_quantities[type] > 0) {
        current_car_quantities[type] -= 1;
    }
}

int City::most_missing_type() {
    int min = 0;
    std::cout << "missing_car_quantities   ";
    for (int i = 0; i < 8; i++) {
        if (missing_car_quantities[min] > missing_car_quantities[i]) {
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

void City::update_transport_emissions() 
{
    double emission = 0;
    double emissions[8] = {0, 0.328 * 365, 0.115 * 365, 0.4 * 365, 0, 0.11 * 365, 1.25 * 365, 0.5 * 365 };
    for (int i = 0; i < 8; i++) {
        emission += current_car_quantities[i] * emissions[i];
    }
    TransportCO2 = emission;
}

void City::transport_to_add() { //now the old finction transport_probabilities updates the missing_car_quantities with the relavent numbers
                               // the finction goes through all the buidlings  to get the wage as income
    Transport electicCar = Transport(0);
    Transport bigCar = Transport(1);
    Transport car = Transport(2);
    Transport collectionCar = Transport(3);
    Transport bike = Transport(4);
    Transport motorcycle = Transport(5);
    Transport bus = Transport(6);
    Transport sportsCar = Transport(7);


    airQuality = pow(1.4, -100 * carbonEmission / (all_structures.size() * 30 * 30 * 10)) + 0.2 * (trees_iterator - trees_vector.begin()) / (trees_vector.size());

    std::cout << "Current air quality is: " << airQuality << std::endl;

    double satisfaction[8] = { electicCar.satisfaction, bigCar.satisfaction * ((double)(7 - carProhibition) / 7.0), car.satisfaction * ((double)(7 - carProhibition) / 7.0), collectionCar.satisfaction * ((double)(7 - carProhibition) / 7.0), bike.satisfaction * (1.0 + pow(((double)(carProhibition) / 7.0), 0.2)) * sqrt(airQuality), motorcycle.satisfaction , bus.get_satisfaction(), sportsCar.satisfaction * ((double)(7 - carProhibition) / 7.0) };
    double costs[8] = { electicCar.cost - electricCarSubsidy , bigCar.cost + weightTax * bigCar.weight, car.cost,collectionCar.cost + weightTax * collectionCar.weight , bike.cost - bikeSubsidy , motorcycle.cost, bus.cost - busSubsidy, sportsCar.cost };
    double pricesPerMonth[8] = { electicCar.pricePerMonth + costs[0] / 15, bigCar.pricePerMonth + fuelTax * bigCar.fuelInput / 12 + costs[1] / 15, car.pricePerMonth + fuelTax * car.fuelInput / 12 +costs[2] / 15,collectionCar.pricePerMonth + fuelTax * collectionCar.fuelInput / 12 +costs[3] / 15, bike.pricePerMonth + costs[4] / 5,
    motorcycle.pricePerMonth + fuelTax * motorcycle.fuelInput / 12 + costs[5] / 10, bus.pricePerMonth / bus.capacity, sportsCar.pricePerMonth + fuelTax * sportsCar.fuelInput / 12 + costs[7] / 15 };
    double probabilities[8] = { 0 };
    double quantities[8] = { 0 };
    double alphaSum = 0;
    double alpha[8] = { 0 };
    int capacity[8] = { 3, 3, 3, 2, 1 ,1, 8, 2 };

    for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it) {
        
        for (int i = 0; i < 8; i++) {
            alpha[i] = fmax(normalGenerator(satisfaction[i], satisfaction[i] / 2), 0.01);
            alphaSum += alpha[i];
        }
        for (int i = 0; i < 8; i++) {
            alpha[i] = alpha[i] / alphaSum;
        }

        double choice[8] = { 0 };

        for (int i = 0; i < 8; i++) {
            probabilities[i] = 1000 * alpha[i] * (30 * ((double)((*it)->get("housingIncome")) / pricesPerMonth[i]));
            // 1000 is one adjustement constant, how much people are willing to put on a car,
            //higher, everybody can afford (prob > 1)any type and will choose the one he prefer (higher alpha)
            // lower, price will restrict people more and more people won't spend money on transport (so sport or bike)
            if (probabilities[i] > 1) {
                choice[i] = alpha[i];
            }
            else {
                choice[i] = 0;
            }
        }

        int maxIndex = -1; 
        double maxChoice = choice[0];
        for (int i = 1; i < 8; i++) {
            if (choice[i] > maxChoice) {
                maxChoice = choice[i];
                maxIndex = i;
            }
        }
        if (maxIndex != -1) { quantities[maxIndex] += 1; }
    }

    for (int i = 0; i < 8; i++) {
        missing_car_quantities[i] = current_car_quantities[i] - quantities[i] / capacity[i];

        if (missing_car_quantities[i] > 0) {
            missing_car_quantities[i] = 0;
        }
    }
}

//generate random alphas for previous function
double City::normalGenerator(double mean, double stdDev)
{
    static default_random_engine generator(time(0));
    normal_distribution<double> distribution(mean, fmax(stdDev, 0.001));
    return distribution(generator);
}
// auxiliary function to be able to have values between 1 and 100 in the pie charts
// 100000 pour power demand et 1 million pour C02
int City::value_pie_chart_C02(int value, int growth) {
    if (value > 0) {return  (int)(100 * (1 - pow(value / growth, -1))); }
    return (value);
 
}

void City::change_pie_chart(int value, NodePath name, bool isPositive)
{
    value = fmax(value, 0);
    TextureProgress* node = ((TextureProgress*)this->get_parent()->get_child(1)->get_node("Infographics")->get_node(name));
    if (value == 0) {
        if (isPositive) { node->set_tint_progress(Color(1, 0, 0, 1.0)); }
        else { node->set_tint_progress(Color(0, 1, 0, 1.0)); }
        node->set("value", 100);
    }
    else {
        if (isPositive) {
            node->set_tint_progress(Color(min(2 - (double)value / 50, 0.99), min((double)value / 50, 0.99), 0, 1.0));
        }
        else {
            node->set_tint_progress(Color(min((double)value / 50, 0.99), min(2 - (double)value / 50, 0.99), 0, 1.0));
        }
        node->set("value", value);
    }
}

void City::change_pie_label(int value, NodePath name) {
    Label* label = ((Label*)this->get_parent()->get_child(1)->get_node("Infographics")->get_node(name)->get_child(1));
    std::string standardString = std::to_string((int)value);
    godot::String godotString = godot::String(standardString.c_str());
    label->set("text", godotString);
}




// auxiliary function to simplify ring city generation

float City::calculate_building_prob(float roota, float rootb, float proportion, double dist) {
    if (dist < (citysize * roota / 20) || dist >(citysize * rootb / 20)) { return 0; }
    else {

        // function which calculates coefficients of quadratic equation for a building distribution
        double a = double(proportion / (((rootb - roota) / 6) * (-pow(rootb, 2) - pow(roota, 2) + 2 * roota * rootb)));

        double b = double(-(roota + rootb) * a);

        double c = double(a * roota * rootb);

        // returns specific probability 
        return float(a * pow(((20 * dist / (citysize))), 2) + b * (20 * dist / (citysize)) + c);


        // test line to verify that the integral is the proportion

        // return float((a / 3) * (pow(rootb, 3) - pow(roota, 3)) + (b / 2) * (pow(rootb, 2) - pow(roota, 2)) + (rootb - roota) * c);
    }


}
// function that returns unemployment rate

double City::return_unemployment_rate() {
    if (population != 0) {
        return min(0.0, (1 - this->numberOfEmployees / this->population));
    }
    else { return 0; }
}

// functions that get and set the value that coordinates the power plants
int City::get_workingPower() {
    return this->workingPower;
}

void City::set_workingPower(int s) {
    (this->workingPower) = s;
}

double City::get_budget() {
    return this->budget;
}
void City::set_budget(double v) {
    this->budget = v;
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

