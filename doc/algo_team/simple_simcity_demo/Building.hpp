#ifndef BUILDING_H
#define BUILDING_H

class Building {
public:
    Building(int build_time, int build_cost, int cost_per_month,
             int energy_demand, int polution) :
             build_time(build_time), build_cost(build_cost),
             cost_per_month(cost_per_month), energy_demand(energy_demand),
             polution(polution) {}
    virtual void update_stat(int current_time);
    int get_build_cost() { return build_cost; }
    int get_cost_per_month() { return cost_per_month; }
    int get_energy_demand() { return energy_demand; }
    int get_polution() { return polution; }

protected:
    int build_time;
    int build_cost;
    int cost_per_month;
    int energy_demand;
    int polution;
};

class EnergyBuilding: public Building {
public:
    EnergyBuilding(int build_time, int build_cost, int cost_per_month,
           int energy_demand, int polution, int energy_supply) :
           Building(build_time, build_cost, cost_per_month, energy_demand, polution),
           energy_supply(energy_supply) {}
    void update_stat(int current_time) override;
    int get_energy_supply() { return energy_supply; }

protected:
    int energy_supply;
};

class EconomicBuilding: public Building {
public:
    EconomicBuilding(int build_time, int build_cost, int cost_per_month, int energy_demand, 
             int polution, int income_per_person, int employment) :
             Building(build_time, build_cost, cost_per_month, energy_demand, polution),
             income_per_person(income_per_person), employment(employment) {}
    void update_stat(int current_time) override;
    int get_income_per_person() { return income_per_person; }
    int get_employment() { return employment; }

protected:
    int income_per_person;
    int employment;
};

class HousingBuilding: public Building {
public:
    HousingBuilding(int build_time, int build_cost, int cost_per_month, 
            int energy_demand, int polution, int population) :
            Building(build_time, build_cost, cost_per_month, energy_demand, polution),
            population(population) {}
    void update_stat(int current_time) override;
    int get_population() { return population; }

protected:
    int population;
};

#endif