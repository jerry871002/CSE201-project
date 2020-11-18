#include <iostream>
#include "City.hpp"

city_status City::get_city_status() {
    return {
        money,
        population,
        electric_supply,
        electric_demand,
        employment,
        gdp,
        pollution,
        happiness,
        current_time
    };
}

void City::update_happiness() {
    // pollution per citizen, income per citizen, employment rate, energy supply
    double pol = (double)pollution / population;
    double gdp = (double)gdp / population;
    double emp = (double)employment / population;
    double elec = (double)electric_supply / electric_demand;

    // just not to make happiness changing too much at a time    
    double pol_nor = (1 - pol);
    double gdp_nor = (gdp - 100) / 100;
    double emp_nor = (emp - 1) / 10;
    double elec_nor = (elec - 1.25) / 5;
    double satisfy = pol_nor + gdp_nor + emp_nor + elec_nor;

    happiness *= (1 + satisfy / 100);

    std::cout << "The happiness changes by " << satisfy << "% this time." << std::endl;
}