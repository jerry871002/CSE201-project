
//#include "edit_text_files.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

// g++ -std=c++17 edit_text_files.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64

// All inputs are strings for the functions below.
// The documentName for the file pollution.csv that is located in the "data" folder is "pollution".
// Ideally, all csv files should be stored in the "data" folder.

string get_path(string documentName) {
    return "../../addons/easy_charts/file.samples/" + documentName + ".csv";
}

// Function to add a line of the form "2015;76" to the csv file named documentName.
// To do so, call add_data("pollution", "2015", "76");
void add_data(string documentName, string year, string value) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
    file << year << ";" << value << '\n';
    file.close();
}

void add_data3(string documentName, string year, string value1, string value2, string value3)
{
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
    file << year << ";" << value1 << ";" << value2 << ";" << value3 << '\n';
    file.close();
}

// Suppresses all data stored in the file documentName
void clear_completely(string documentName) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

// Suppresses all data stored in the file documentName except the first line.
void clear(string documentName) {
    fstream file;
    string path = get_path(documentName);

    file.open(path);
    string line;
    getline(file, line);
    file.close();

    file.open(path, ios::out | ios::trunc);
    file.close();

    file.open(path);
    file << line << '\n';
    file.close();
}

// Copies the data stored in documentNameFrom to an empty file documentNameTo.
void copy(string documentNameFrom, string documentNameTo) {
    fstream fileFrom;
    fstream fileTo;
    string path1 = get_path(documentNameFrom);
    string path2 = get_path(documentNameTo);
    fileFrom.open(path1);
    fileTo.open(path2);
    while (fileFrom.good()) {
        string line;
        getline(fileFrom, line, '\n');
        if (line.length() > 0) {
            fileTo << line << '\n';
        }
    }
    fileFrom.close();
    fileTo.close();
}

// Function to modify a line of the csv file named documentName.
// For example, if you want to change the line "2015;76" into "2015;01" of the pollution.csv file, call change_data("pollution", "2015", "01");
// By calling change_data("pollution", "2015", "01");, you will change all lines of the form "2015;**" into "2015;01".
// If no line in the file is of the form "2015;**", calling change_data("pollution", "2015", "01") won't do anything.
void change_data(string documentName, string dataToChange, string newValue) {
    fstream file;
    fstream temp;
    string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            string sub = line.substr(0, pos);
            if (sub == dataToChange) {
                temp << sub << ";" << newValue << '\n';
            } else {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}

// Function to delete a line of the csv file named documentName.
// By calling delete_line("pollution", "2015");, you will delete all lines of the form "2015;**".
// If no line in the file is of the form "2015;**", calling delete_line("pollution", "2015") won't do anything.
void delete_line(string documentName, string dataToDelete) {
    fstream file;
    fstream temp;
    string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            string sub = line.substr(0, pos);
            if (sub != dataToDelete) {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}

/*
// defined in City.cpp
int* return_date(int day_tick) {
    static int date[3];

    int Y=1,M=1,D=1;
    int julian = (1461 * (Y + 4800 + (M - 14)/12))/4 +(367 * (M - 2 - 12 / ((M - 14)/12)))/12 - (3 * ((Y + 4900 + (M - 14)/12)/100))/4 + D - 32075+day_tick+35;

    int gregorian = julian + 1401 + (int)((((int)(4*day_tick+274277) / 146097)*3) / 4) -38;
    int e = 4*gregorian+3;
    int h = 5*((int)(e%1461)/4)+2;
    int day = ((int)(h%153)/5)+1;
    int month = (((int)h/153)+2)%12+1;
    int year = (int)(e/1461) - 4716 + (int)((14-month)/12);

    date[0]= day;
    date[1] = month;
    date[2] = year;

    return date;
}


string return_string_date(int day, int month, int year) {
    return to_string(day) + ", " + to_string(month) + ", " + to_string(year);
}

double find_avg(double array[],int leap) {
    int size;
    double sum=0;
    if (leap==0) {
        size=366;
    }
    else {
        size=365;
    }
    for (int i=0; i<size; i++) {
        sum+=array[i];
    }
    return sum/size;
}

string return_game_date(int day_tick) {

	int* datenumber = return_date(day_tick);
	int year=datenumber[2];
	string date = "Year ";
	date += to_string(datenumber[2]);
	date += ", ";

	if (datenumber[1] == 1) {
		date += string("January ");
	}
	if (datenumber[1] == 2) {
		date += string("February ");
	}
	if (datenumber[1] == 3) {
		date += string("March ");
	}
	if (datenumber[1] == 4) {
		date += string("April ");
	}
	if (datenumber[1] == 5) {
		date += string("May ");
	}
	if (datenumber[1] == 6) {
		date += string("June ");
	}
	if (datenumber[1] == 7) {
		date += string("July ");
	}
	if (datenumber[1] == 8) {
		date += string("August ");
	}
	if (datenumber[1] == 9) {
		date += string("September ");
	}
	if (datenumber[1] == 10) {
		date += string("October ");
	}
	if (datenumber[1] == 11) {
		date += string("November ");
	}
	if (datenumber[1] == 12) {
		date += string("December ");
	}

	date += to_string(datenumber[0]);
    
	return date;
}
*/




//uncomment to run
int main() {
    // More examples on how to use the above functions:
    
    //clear("datas");
    //change_data("datas", "2010", "37001");
    //copy("pollution", "tmp");
    //add_data3("datas", "2015", "76", "54", "47");
    //delete_line("datas", "2010");
    //clear_completely("datas");


    /*
    
    double stat = 0;
    int day_tick = 0;
    double stats[366];
    int daycount=0;

    while (true) {

        #ifdef _WIN32
            Sleep(100);
        #else
            sleep(1);
        #endif

        day_tick += 1;
        stat+=25;
        
        string date2 = return_game_date(day_tick);
        std::cout<<date2<<std::endl;

        int *date; 
        date = return_date(day_tick);
        int day = *date;
        int month = *(date+1);
        int year = *(date+2);

        
        if (day==1 && month==1 && year!=1) {
            daycount=0;
            int leap = (year-1)%4;
            add_data("alltimestats", to_string(year-1), to_string(find_avg(stats,leap)));
            double stats[366];
            remove(get_path("statsyear" + to_string(year-1)).c_str());
        }
        

        stats[daycount]=stat;
        daycount+=1;
        add_data("statsyear" + to_string(year), return_string_date(day,month,year), to_string(stat));

        
        int *daysbef;
        daysbef = return_date(day_tick-30);
        int daydaysbef=*daysbef;
        int monthdaysbef=*(daysbef+1);
        int yeardaysbef=*(daysbef+2);
        delete_line("statsyear" + to_string(year), return_string_date(daydaysbef,monthdaysbef,yeardaysbef));
        
    }

    */
    
}

