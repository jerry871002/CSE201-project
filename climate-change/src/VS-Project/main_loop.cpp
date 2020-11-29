#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "Restaurant.h"
#include "obj.h"
#include "obj.cpp"
#include "City.h"
#include<windows.h>



int main()
{
	City c = City();

	while (true) {
		c.simulation();
		std::cout << c.return_game_date()<< std::endl;
		std::cout << "city income is " << c.return_income() << std::endl;
		Restaurant resto = Restaurant();
		Restaurant *restpoint = &resto;
		// c.add_building(restpoint);
		Sleep(200);
	}



}

