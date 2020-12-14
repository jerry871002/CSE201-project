#include <core/Godot.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "obj.h"
#include "edit_text_files.cpp"
#include <ctime>

using namespace godot;


void write_stat_history_to_file() {
    std::ofstream incomefile;
    incomefile.open("incomefile.txt", std::ofstream::out | std::ofstream::app);
    if(!incomefile) { 
        std::cout<<"Error in creating file!!!"<< std::endl;
        }
    std::cout<<"File created successfully."<<std::endl; 
    incomefile.close();
    //incomefile << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
    //incomefile << carbonEmission << " " << energyDemand << " " << energySupply << std::endl;
    //add_data("incomefile",std::to_string((day_tick / 365) + 1),std::to_string(income));



    /* old version to be deleted?
    std::ofstream out_file;
    out_file.open("stat_history.txt", std::ofstream::out | std::ofstream::app);

    //out_file << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
    out_file << carbonEmission << " " << energyDemand << " " << energySupply << std::endl;
    out_file.close();
    */
}
