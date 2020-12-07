#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

<<<<<<< HEAD
=======
// All inputs are strings for the functions below.
// The documentName for the file pollution.csv that is located in the "data" folder is "pollution".
// Ideally, all csv files should be stored in the "data" folder.

>>>>>>> master
string get_path(string documentName) {
    return "data/" + documentName + ".csv";
}

<<<<<<< HEAD
=======
// Function to add a line of the form "2015;76" to the csv file named documentName.
// To do so, call add_data("pollution", "2015", "76");
>>>>>>> master
void add_data(string documentName, string year, string value) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
<<<<<<< HEAD
    file << year << ";" << value << "\n";
    file.close();
}

void change_data(string documentName, string dataChanged, string newValue) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::in | ios::out);
    int lineNumber = 0;
    file.close();
    // Function is not finished
}

void clear(string documentName) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

int main() {
    //clear("pollution");
    //add_data("pollution", "2012", "44");
    //change_data("pollution", "2013", "44");
=======
    file << year << ";" << value << '\n';
    file.close();
}

// Suppresses all data stored in the file documentName.
void clear(string documentName) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
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
void change_data(string documentName, string dataToChange, string newValue) {
    fstream file;
    fstream temp;
    string path = get_path(documentName);
    file.open(path);
    temp.open("data/tmp.csv");

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

    clear(documentName);

    copy("tmp", documentName);
    clear("tmp");
}

int main() {
    // More examples on how to use the functions: 
    // clear("pollution");
    // change_data("pollution", "2013", "01");
    // copy("pollution", "tmp");
    // add_data("pollution", "2015", "76");
>>>>>>> master
    return 0;
}
