#include <vector>
#include "City.hpp"
#include "Building.hpp"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player() : city(City()) {}

    void get_city_status();
    int get_city_time();
    void build(std::vector<Building*> buildings);
    void next_round();
private:
    City city;
};

#endif