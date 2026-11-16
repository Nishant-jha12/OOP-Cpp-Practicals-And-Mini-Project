#include <iostream>
#include <string>

using namespace std;

class Person {
protected:
    string name;
    int age;

public:
    Person(string n, int a) {
        name = n;
        age = a;
    }

    void displayPerson() {
        cout << "Name: " << name << "\n";
        cout << "Age: " << age << "\n";
    }
};

class Student : public Person {
protected:
    string rollNo;
    string course;

public:
    Student(string n, int a, string r, string c) : Person(n, a) {
        rollNo = r;
        course = c;
    }

    void displayStudent() {
        displayPerson();
        cout << "Roll No: " << rollNo << "\n";
        cout << "Course: " << course << "\n";
    }
};

class GraduateStudent : public Student {
private:
    string researchTopic;

public:
    GraduateStudent(string n, int a, string r, string c, string topic) 
        : Student(n, a, r, c) {
        researchTopic = topic;
    }

    void displayGraduate() {
        displayStudent();
        cout << "Research Topic: " << researchTopic << "\n";
    }
};

int main() {
    cout << "--- Student (Single Inheritance) ---\n";
    Student s1("Ananya", 20, "UG101", "Computer Engineering");
    s1.displayStudent();

    cout << "\n--- Graduate Student (Multilevel Inheritance) ---\n";
    GraduateStudent g1("Nishant", 23, "PG201", "Artificial Intelligence", "Robotics");
    g1.displayGraduate();

    return 0;
}
