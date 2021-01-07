/*
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

string get_path(string documentName);

void add_data(string documentName, string year, string value);

void clear_completely(string documentName);

void clear(string documentName);

void copy(string documentNameFrom, string documentNameTo);

void change_data(string documentName, string dataToChange, string newValue);

void delete_line(string documentName, string dataToDelete);

*/