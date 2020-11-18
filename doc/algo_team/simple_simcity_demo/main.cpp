#include <iostream>
#include <vector>
#include "Player.hpp"
#include "Building.hpp"

int energy_policies();
int economic_policies();
int housing_policies();

void summary(std::vector<Building*> buildings);

int main(int argc, char const *argv[]) {
    Player player = Player();
    while (true) {
        // show current status of the city
        player.get_city_status();

        // choose some policies
        std::cout << "The policies are divided into three category: energy, economic and housing.\n" 
                  << "Please choose one policy from each category.\n";
        std::vector<Building*> buildings_to_build;

        // Energy
        Building* energy_building;
        int energy_policy = energy_policies();
        switch (energy_policy) {
            case 1: // Nuclear
                energy_building = new EnergyBuilding(player.get_city_time(), 3000, 300, 30, 5, 1000); 
                break;
            case 2: // Coal
                energy_building = new EnergyBuilding(player.get_city_time(), 500, 100, 30, 50, 600); 
                break;
            case 3: // Wind
                energy_building = new EnergyBuilding(player.get_city_time(), 2000, 300, 30, 1, 100); 
                break;
        };
        buildings_to_build.push_back(energy_building);

        // Economic
        Building* economic_building;
        int economic_policy = economic_policies();
        switch (economic_policy) {
            case 1: // Factory
                economic_building = new EconomicBuilding(player.get_city_time(), 100, 50, 150, 100, 5, 1000);
                break;
            case 2: // High Tech
                economic_building = new EconomicBuilding(player.get_city_time(), 1000, 500, 50, 10, 50, 150);
                break;
        };
        buildings_to_build.push_back(economic_building);

        // Housing
        Building* housing_building;
        int housing_policy = housing_policies();
        switch (housing_policy) {
            case 1: // Luxury
                housing_building = new HousingBuilding(player.get_city_time(), 200, 20, 20, 10, 100);
                break;
            case 2: // Normal
                housing_building = new HousingBuilding(player.get_city_time(), 100, 10, 10, 20, 200);
                break;
            case 3: // Crowded
                housing_building = new HousingBuilding(player.get_city_time(), 50, 5, 5, 30, 500);
        };
        buildings_to_build.push_back(housing_building);

        // do some summary
        summary(buildings_to_build);

        // build the buildings
        player.build(buildings_to_build);

        // go to next round
        player.next_round();
    }
    return 0;
}

int energy_policies() {
    std::cout << "===== Energey Policies =====" << std::endl;
    std::cout << "Type\t\tPollution\tEnergy Supply\tCost" << std::endl;
    std::cout << "1. Nuclear\tmedium\t\thigh\t\thigh" << std::endl;
    std::cout << "2. Coal\t\thigh\t\tmedium\t\tlow" << std::endl;
    std::cout << "3. Wind\t\tlow\t\tlow\t\tmedium" << std::endl;
    std::cout << "Please choose one (1~3): ";

    int choice;
    std::cin >> choice;
    return choice;
}

int economic_policies() {
    std::cout << "===== Economic Policies =====" << std::endl;
    std::cout << "Type\t\tPollution\tEmployment\tIncome\tCost" << std::endl;
    std::cout << "1. Factory\thigh\t\thigh\t\tlow\tlow" << std::endl;
    std::cout << "2. High-Tech\tlow\t\tlow\t\thigh\thigh" << std::endl;
    std::cout << "Please choose one (1~2): ";

    int choice;
    std::cin >> choice;
    return choice;
}

int housing_policies() {
    std::cout << "===== Housing Policies =====" << std::endl;
    std::cout << "Type\t\tPollution\tPolpulation\tCost" << std::endl;
    std::cout << "1. Luxury\tlow\t\tlow\t\thigh" << std::endl;
    std::cout << "2. Normal\tmedium\t\tmedium\t\tmedium" << std::endl;
    std::cout << "3. Crowded\thigh\t\thigh\t\tlow" << std::endl;
    std::cout << "Please choose one (1~3): ";

    int choice;
    std::cin >> choice;
    return choice;
}

void summary(std::vector<Building*> buildings) {
    int total_cost = 0;
    for (auto i = buildings.begin(); i != buildings.end(); i++) {
        total_cost += (*i)->get_build_cost();
    }
    std::cout << "\nSpending " << total_cost << " on building these buildings!\n" << std::endl;
}