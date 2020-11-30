#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string get_path(string documentName) {
    return "data/" + documentName + ".csv";
}

void add_data(string documentName, string year, string value) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
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
    return 0;
}
