#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Student {
public:
    int rollNo;
    string name;
    string branch;
    double cgpa;

    Student(int r = 0, string n = "", string b = "", double c = 0.0) {
        rollNo = r;
        name = n;
        branch = b;
        cgpa = c;
    }

    void display() {
        cout << rollNo << " | " << name << " | " << branch << " | " << cgpa << "\n";
    }
};

void writeData(string filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << 101 << " " << "Nishant" << " " << "CSE" << " " << 9.35 << "\n";
        file << 102 << " " << "Aarav" << " " << "ENTC" << " " << 8.60 << "\n";
        file << 103 << " " << "Priya" << " " << "MECH" << " " << 8.95
