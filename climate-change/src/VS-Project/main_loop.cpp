#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "Restaurant.h"
#include "obj.h"
#include "City.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main()
{
	City c = City();

	while (true) {

		// run simulation (one day tick) , and then print out date and income
		c.simulation();
		std::cout << c.return_game_date()<< std::endl;
		std::cout << "city income is " << c.return_income() << std::endl;
		std::cout << "city number of employees is " << c.return_numberOfEmployees() << std::endl;
		std::cout << "city carbon emissions are " << c.return_carbonEmission() << std::endl;
		std::cout << "city energy demand is " << c.return_energyDemand() << std::endl;
		std::cout << "city energy supply is " << c.return_energySupply() << std::endl;
		std::cout << "city healthcare is " << c.return_healthcare() << std::endl;
		std::cout << "city needs are " << c.return_needs() << std::endl;

		// add a restaurant pointer to the buildings set
		Restaurant rest = Restaurant(100,5,15,2,0,2.5,1.67);
		Struc* struc_pointer = &rest;
		sleep(2);
		c.add_building(struc_pointer);

		// advance by 3 days before rerunning through the loop 
		for (int i=0; i<3; i++) {
			c.simulation();
			std::cout << c.return_game_date() << std::endl;
			std::cout << "city income is " << c.return_income() << std::endl;
			std::cout << "city number of employees is " << c.return_numberOfEmployees() << std::endl;
			std::cout << "city carbon emissions are " << c.return_carbonEmission() << std::endl;
			std::cout << "city energy demand is " << c.return_energyDemand() << std::endl;
			std::cout << "city energy supply is " << c.return_energySupply() << std::endl;
			std::cout << "city healthcare is " << c.return_healthcare() << std::endl;
			std::cout << "city needs are " << c.return_needs() << std::endl;
			sleep(2);
		}
	}
}

