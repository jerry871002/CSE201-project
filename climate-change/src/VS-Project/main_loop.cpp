#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "Restaurant.h"
#include "Player.h"
#include "Transport.h"
#include "obj.h"
#include "City.h"
#include "edit_text_files.cpp"
#include "Shop.h"

// Sleep(miliseconds) on Windows and sleep(seconds) on linux/mac
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace godot;

int main() {
    City c = City();

    while (true) {
 	//check if writing to files works

 	/* current test fct on mac:

 	g++ -std=c++17 main_loop.cpp obj.cpp edit_text_files.cpp City.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64

 	then run:
 	./a.out

 	*/

 	c.simulation();

        /*
        // run simulation (one day tick) , and then print out date and income
        c.simulation();
        std::cout << c.return_game_date()<< std::endl;
        std::cout << "city income is " << c.return_income() << std::endl;
        std::cout << "city number of employees is " << c.return_numberOfEmployees() << std::endl;
        std::cout << "city carbon emissions are " << c.return_carbonEmission() << std::endl;
        std::cout << "city energy demand is " << c.return_energyDemand() << std::endl;
        std::cout << "city energy supply is " << c.return_energySupply() << std::endl;
        std::cout << "city healthcare is " << c.return_healthcare() << std::endl;
        std::cout << "city needs are " << c.return_needs() << std::endl << std::endl;

        // add a restaurant pointer to the buildings set
        Restaurant rest = Restaurant(100,5,15,2,0,2.5,1.67);
        Structure* struc_pointer = &rest;

#ifdef _WIN32
        Sleep(100);
#else
        sleep(1);
#endif
        
        c.add_building(struc_pointer);
        std::cout << "New restaurant!" << std::endl << std::endl;

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
            std::cout << "city needs are " << c.return_needs()<< std::endl << std::endl;

#ifdef _WIN32
            Sleep(100);
#else
            sleep(1);
#endif
        }
	*/
    }
	
}
