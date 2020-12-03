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
        Restaurant rest = Restaurant();
        Structure* struc_pointer = &rest;
        c.add_building(struc_pointer);
#ifdef _WIN32
        Sleep(200);
#else 
        sleep(2);
#endif

        // advance by 3 days before rerunning through the loop
        for (int i = 0; i < 3; i++) {
            c.simulation();
            std::cout << c.return_game_date() << std::endl;
            std::cout << "city income is " << c.return_income() << std::endl;
#ifdef _WIN32
            Sleep(200);
#else 
            sleep(2);
#endif
        }
    }
}

