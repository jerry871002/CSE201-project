#include "Building.hpp"

void Building::update_stat(int current_time) {
    int delta_t = current_time - build_time;
    cost_per_month *= (1 + delta_t / 100);
    energy_demand *= (1 + delta_t / 100);
    polution *= (1 + delta_t / 100);
}

void EnergyBuilding::update_stat(int current_time) {
    Building::update_stat(current_time);
    int delta_t = current_time - build_time;
    energy_supply *= (1 - delta_t / 100);
}

void EconomicBuilding::update_stat(int current_time) {
    Building::update_stat(current_time);
    int delta_t = current_time - build_time;
    income_per_person *= (1 - delta_t / 1000);
    employment *= (1 - delta_t / 1000);
}

void HousingBuilding::update_stat(int current_time) {
    Building::update_stat(current_time);
    int delta_t = current_time - build_time;
    population *= (1 - delta_t / 1000);
}