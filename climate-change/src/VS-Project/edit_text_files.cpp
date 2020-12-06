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
    file << year << ";" << value << '\n';
    file.close();
}

void clear(string documentName) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

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
    //clear("pollution");
    //change_data("pollution", "2013", "01");
    //copy("pollution", "tmp");
    return 0;
}
