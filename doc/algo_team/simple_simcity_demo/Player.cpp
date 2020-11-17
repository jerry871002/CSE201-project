#include <iostream>
#include "Player.hpp"
#include "City.hpp"
#include "Building.hpp"

void Player::get_city_status() {
    city_status status = city.get_city_status();
    std::cout << "===== Current Time: " << status.current_time << " =====" << std::endl;
    std::cout << "Money: " << status.money << std::endl;
    std::cout << "Population: " << status.population << std::endl;
    std::cout << "Electric Supply: " << status.electric_supply << std::endl;
    std::cout << "Electric Demand: " << status.electric_demand << std::endl;
    std::cout << "Employment: " << status.employment << std::endl;
    std::cout << "GDP: " << status.gdp << std::endl;
    std::cout << "Pollution: " << status.pollution<< std::endl;
    std::cout << "Happiness: " << status.happiness << std::endl << std::endl;  
}

int Player::get_city_time() {
    return city.get_current_time();
}

void Player::build(std::vector<Building*> buildings) {
    int total_cost = 0;
    for (auto i = buildings.begin(); i != buildings.end(); i++) {
        total_cost += (*i)->get_build_cost();
        city.add_building(*i);
    }
    city.spend(total_cost);
}

void Player::next_round() {
    // for all Building
    int monthly_cost = 0;
    int energy_demand = 0;
    int pollution = 0;

    // only for EnergyBuilding
    int energy_supply = 0;

    // only for EconomicBuilding
    int employment = 0;
    int total_income = 0;

    // only for HousingBuilding
    int population = 0;

    for (auto i = city.buildings.begin(); i != city.buildings.end(); i++) {
        (*i)->update_stat(get_city_time());

        monthly_cost += (*i)->get_cost_per_month();
        energy_demand += (*i)->get_energy_demand();
        pollution += (*i)->get_polution();

        if (EnergyBuilding* e = dynamic_cast<EnergyBuilding*>(*i)) {
            energy_supply += e->get_energy_supply();
        }

        if (EconomicBuilding* e = dynamic_cast<EconomicBuilding*>(*i)) {
            employment += e->get_employment();
            total_income += (e->get_income_per_person() * employment);
        }

        if (HousingBuilding* h = dynamic_cast<HousingBuilding*>(*i)) {
            population += h->get_population();
        }
    }

    city.spend(monthly_cost);
    city.set_population(population);
    city.set_electric_demand(energy_demand);
    city.set_electric_supply(energy_supply);
    city.set_pollution(pollution);
    city.set_gdp(total_income);
    city.set_employment(employment);

    city.update_happiness();

    city.update_time();
}