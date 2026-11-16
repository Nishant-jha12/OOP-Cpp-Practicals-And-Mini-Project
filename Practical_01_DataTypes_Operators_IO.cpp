// ============================================================================
// Practical 1: C++ Foundations - Data Types, Constants, Variables, Operators & I/O
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 1 - Introduction to OOP & Modern C++ Foundations
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>

// Global variable to demonstrate Scope Resolution Operator (::)
int taxRatePercent = 18;

//Compile-time constant evaluation using constexpr (Modern C++ foundation)
constexpr double calculateSimpleInterest(double principal, double rate, int time) {
    return (principal * rate * time) / 100.0;
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << " PRACTICAL 1: DATA TYPES, CONSTANTS, VARIABLES, OPERATORS & I/O\n";
    std::cout << "====================================================================\n\n";

    // ------------------------------------------------------------------------
    // 1. BASIC DATA TYPES & VARIABLES
    // ------------------------------------------------------------------------
    int studentId = 101;
    float attendancePercentage = 92.5f;
    double tuitionFee = 85000.75;
    char sectionGrade = 'A';
    bool isScholarshipEligible = true;
    std::string studentName = "Aarav Sharma";

    std::cout << "[1] Basic Data Types & Variables (with sizeof in bytes):\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "  int (studentId)            : " << std::setw(14) << std::left << studentId
              << " | sizeof(int)    = " << sizeof(studentId) << " bytes\n";
    std::cout << "  float (attendance)         : " << std::setw(14) << std::left << attendancePercentage
              << " | sizeof(float)  = " << sizeof(attendancePercentage) << " bytes\n";
    std::cout << "  double (tuitionFee)        : " << std::setw(14) << std::left << std::fixed << std::setprecision(2) << tuitionFee
              << " | sizeof(double) = " << sizeof(tuitionFee) << " bytes\n";
    std::cout << "  char (sectionGrade)        : " << std::setw(14) << std::left << sectionGrade
              << " | sizeof(char)   = " << sizeof(sectionGrade) << " byte\n";
    std::cout << "  bool (scholarshipEligible) : " << std::setw(14) << std::left << std::boolalpha << isScholarshipEligible
              << " | sizeof(bool)   = " << sizeof(isScholarshipEligible) << " byte\n\n";

    // ------------------------------------------------------------------------
    // 2. CONSTANTS (const and constexpr)
    // ------------------------------------------------------------------------
    const int MAX_CREDITS = 24;
    constexpr double PI = 3.141592653589793;
    constexpr double compileTimeInterest = calculateSimpleInterest(10000.0, 7.5, 2);

    std::cout << "[2] Constants (const & constexpr):\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "  const MAX_CREDITS          : " << MAX_CREDITS << "\n";
    std::cout << "  constexpr PI               : " << std::setprecision(6) << PI << "\n";
    std::cout << "  constexpr Simple Interest  : Rs. " << std::setprecision(2) << compileTimeInterest << "\n\n";

    // ------------------------------------------------------------------------
    // 3. CONSOLE I/O (std::cin and std::cout)
    // ------------------------------------------------------------------------
    std::cout << "[3] Console Input / Output Demonstration:\n";
    std::cout << "--------------------------------------------------------------------\n";
    int a = 25, b = 10;
    std::cout << "  Enter two integers (or press Enter/EOF to use defaults 25 and 10): ";
    if (!(std::cin >> a >> b)) {
        std::cin.clear();
        a = 25;
        b = 10;
        std::cout << "\n  [Using default values: a = " << a << ", b = " << b << "]\n";
    } else {
        std::cout << "  [Received values: a = " << a << ", b = " << b << "]\n";
    }
    std::cout << "\n";

    // ------------------------------------------------------------------------
    // 4. OPERATORS IN C++
    // ------------------------------------------------------------------------
    std::cout << "[4] C++ Operators Demonstration (for a = " << a << ", b = " << b << "):\n";
    std::cout << "--------------------------------------------------------------------\n";

    // 4a. Arithmetic Operators
    std::cout << "  (a) Arithmetic Operators:\n";
    std::cout << "      a + b = " << (a + b) << " | a - b = " << (a - b)
              << " | a * b = " << (a * b);
    if (b != 0) {
        std::cout << " | a / b = " << (a / b) << " | a % b = " << (a % b) << "\n";
    } else {
        std::cout << " | Division by zero skipped\n";
    }

    // 4b. Relational Operators
    std::cout << "  (b) Relational Operators:\n";
    std::cout << "      a == b : " << (a == b) << " | a != b : " << (a != b)
              << " | a > b : " << (a > b) << " | a < b : " << (a < b) << "\n";

    // 4c. Logical Operators
    bool cond1 = (a > 0), cond2 = (b > 0);
    std::cout << "  (c) Logical Operators (a>0 is " << cond1 << ", b>0 is " << cond2 << "):\n";
    std::cout << "      (a>0 && b>0) = " << (cond1 && cond2)
              << " | (a>0 || b>0) = " << (cond1 || cond2)
              << " | !(a>0) = " << (!cond1) << "\n";

    // 4d. Bitwise Operators
    std::cout << "  (d) Bitwise Operators:\n";
    std::cout << "      a & b = " << (a & b) << " | a | b = " << (a | b)
              << " | a ^ b = " << (a ^ b) << " | a << 1 = " << (a << 1)
              << " | a >> 1 = " << (a >> 1) << "\n";

    // 4e. Assignment, Increment/Decrement & Ternary Operator
    int temp = a;
    temp += b;
    int afterCompoundAdd = temp;
    int afterPreIncrement = ++temp;
    int maxVal = (a > b) ? a : b;
    std::cout << "  (e) Compound Assignment & Ternary Operator:\n";
    std::cout << "      temp (after temp += b) = " << afterCompoundAdd
              << " | Pre-increment (++temp) = " << afterPreIncrement
              << " | Max using (a > b ? a : b) = " << maxVal << "\n\n";

    // ------------------------------------------------------------------------
    // 5. SCOPE RESOLUTION (::) & MEMORY MANAGEMENT OPERATORS (new / delete)
    // ------------------------------------------------------------------------
    int taxRatePercent = 5; // Local variable shadowing global variable
    std::cout << "[5] Scope Resolution (::) & Dynamic Memory Operators (new / delete):\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "  Local taxRatePercent       : " << taxRatePercent << "%\n";
    std::cout << "  Global ::taxRatePercent    : " << ::taxRatePercent << "%\n";

    // Dynamic allocation using new and nullptr check
    int* dynamicScore = new int(95);
    std::cout << "  Dynamically allocated int  : *dynamicScore = " << *dynamicScore
              << " (at address " << dynamicScore << ")\n";
    delete dynamicScore;
    dynamicScore = nullptr; // Modern C++ nullptr safety
    std::cout << "  After delete & nullptr     : dynamicScore  = " << dynamicScore << "\n\n";

    // ------------------------------------------------------------------------
    // 6. MODERN C++ FEATURES (auto & Range-Based for loop)
    // ------------------------------------------------------------------------
    std::cout << "[6] Modern C++ Features (auto type deduction & range-based for loop):\n";
    std::cout << "--------------------------------------------------------------------\n";
    int subjectMarks[] = {88, 91, 79, 95, 84};
    auto totalMarks = 0; // 'auto' deduces int

    std::cout << "  Subject Marks              : ";
    for (const auto& mark : subjectMarks) {
        std::cout << mark << " ";
        totalMarks += mark;
    }
    auto averageMarks = static_cast<double>(totalMarks) / 5.0; // 'auto' deduces double
    std::cout << "\n  Total Marks (auto int)     : " << totalMarks << "\n";
    std::cout << "  Average Marks (auto double): " << std::setprecision(2) << averageMarks << "\n";
    std::cout << "====================================================================\n";

    return 0;
}
