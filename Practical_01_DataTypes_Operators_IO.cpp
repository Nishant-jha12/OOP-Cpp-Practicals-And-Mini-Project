#include <iostream>
#include <string>

using namespace std;

int taxRate = 18;

int main() {
    int studentId = 101;
    float attendance = 92.5;
    double tuitionFee = 85000.75;
    char grade = 'A';
    bool isEligible = true;
    string name = "Aarav Sharma";

    cout << "ID: " << studentId << "\n";
    cout << "Name: " << name << "\n";
    cout << "Attendance: " << attendance << "%\n";
    cout << "Fee: " << tuitionFee << "\n";
    cout << "Grade: " << grade << "\n";
    cout << "Eligible: " << isEligible << "\n\n";

    const int MAX_CREDITS = 24;
    cout << "Max Credits: " << MAX_CREDITS << "\n\n";

    int a, b;
    cout << "Enter two numbers: ";
    cin >> a >> b;

    cout << "Addition: " << a + b << "\n";
    cout << "Subtraction: " << a - b << "\n";
    cout << "Multiplication: " << a * b << "\n";
    
    if (b != 0) {
        cout << "Division: " << a / b << "\n";
        cout << "Modulus: " << a % b << "\n";
    }

    if (a > b && a > 0) {
        cout << "The first number is greater and positive.\n";
    } else {
        cout << "Condition not met.\n";
    }

    cout << "\nLocal Tax Rate: " << taxRate << "\n";
    cout << "Global Tax Rate: " << ::taxRate << "\n\n";

    int marks[] = {88, 91, 79, 95, 84};
    int total = 0;
    
    cout << "Marks: ";
    for (int mark : marks) {
        cout << mark << " ";
        total += mark;
    }
    
    cout << "\nTotal Marks: " << total << "\n";
    cout << "Average: " << total / 5.0 << "\n";

    return 0;
}
