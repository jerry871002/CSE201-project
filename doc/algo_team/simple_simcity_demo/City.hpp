#include <vector>
#include "Building.hpp"

#ifndef CITY_H
#define CITY_H

typedef struct city_status {
    int money;
    int population;
    int electric_supply;
    int electric_demand;
    int employment;
    int gdp;
    int pollution;
    int happiness;
    int current_time;
} city_status;

class City {
public:
    City(): money(100000),
            population(0), 
            electric_supply(0),
            electric_demand(0),
            employment(0),
            gdp(0),
            pollution(0),
            happiness(50),
            current_time(0) {}
    city_status get_city_status();
    int get_current_time() { return current_time; }
    void add_building(Building* building) { buildings.push_back(building); }
    void spend(int amount) { money -= amount; }
    void set_population(int p) { population = p; }
    void set_electric_supply(int e_s) { electric_supply = e_s; }
    void set_electric_demand(int e_d) { electric_demand = e_d; }
    void set_employment(int e) { employment = e; }
    void set_gdp(int g) { gdp = g; }
    void set_pollution(int p) { pollution = p; }
    void update_happiness();
    void update_time() { current_time += 1; }

    std::vector<Building*> buildings;
private:
    int money;
    int population;
    int electric_supply;
    int electric_demand;
    int employment;
    int gdp;
    int pollution;
    int happiness;
    int current_time; 
};

#endif