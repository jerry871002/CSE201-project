#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "obj.h"
#include "obj.cpp"
#include "City.h"
#include "City.cpp"


int main()
{
	City c = City();


	std::cout << "income of the city is" << c.return_income();
	

	return 0;
}