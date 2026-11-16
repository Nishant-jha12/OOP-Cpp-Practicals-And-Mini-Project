#include <iostream>
#include <string>
#include <vector>

using namespace std;

class StudentRecord {
public:
    int rollNo;
    string name;
    double marks;
    char grade;

    StudentRecord(int r, string n, double m) {
        rollNo = r;
        name = n;
        marks = m;
        
        if (marks >= 90) grade = 'O';
        else if (marks >= 80) grade = 'A';
        else if (marks >= 70) grade = 'B';
        else if (marks >= 60) grade = 'C';
        else grade = 'P';
    }

    void display() {
        cout << "Roll: " << rollNo << " | Name: " << name 
             << " | Marks: " << marks << " | Grade: " << grade << "\n";
    }
};

int main() {
    vector<StudentRecord> students;
    
    students.push_back(StudentRecord(104, "Riya", 82.5));
    students.push_back(StudentRecord(101, "N
