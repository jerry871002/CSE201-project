#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "Restaurant.h"
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

		// add a restaurant pointer to the buildings set
		Restaurant rest = Restaurant(100);
		Structure* struc_pointer = &rest;
		c.add_building(struc_pointer);
		Sleep(200);

		// advance by 3 days before rerunning through the loop 
		c.simulation();
		std::cout << c.return_game_date() << std::endl;
		std::cout << "city income is " << c.return_income() << std::endl;
		Sleep(200);

		c.simulation();
		std::cout << c.return_game_date() << std::endl;
		std::cout << "city income is " << c.return_income() << std::endl;
		Sleep(200);

		c.simulation();
		std::cout << c.return_game_date() << std::endl;
		std::cout << "city income is " << c.return_income() << std::endl;
	}

}

