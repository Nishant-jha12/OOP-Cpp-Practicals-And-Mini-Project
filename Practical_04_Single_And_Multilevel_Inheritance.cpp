// ============================================================================
// Practical 4: Single and Multilevel Inheritance (University Management)
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 3 - Inheritance (Single & Multilevel:
//              Person -> Student -> GraduateStudent), Visibility Modes,
//              Constructor/Destructor Chaining
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// Base Class: Person
// ============================================================================
class Person {
protected:
    std::string fullName;
    int age;
    std::string email;

public:
    Person(const std::string& name, int personAge, const std::string& mail)
        : fullName(name), age(personAge), email(mail) {
        std::cout << "  [Constructor] Person created          : " << fullName << "\n";
    }

    virtual ~Person() {
        std::cout << "  [Destructor]  Person destroyed        : " << fullName << "\n";
    }

    virtual void displayProfile() const {
        std::cout << "  Name               : " << fullName << "\n"
                  << "  Age                : " << age << " years\n"
                  << "  Email              : " << email << "\n";
    }
};

// ============================================================================
// Derived Class (Level 1 - Single Inheritance): Student inherits from Person
// ============================================================================
class Student : public Person {
protected:
    std::string enrollmentNo;
    std::string department;
    double cgpa;

public:
    Student(const std::string& name, int personAge, const std::string& mail,
            const std::string& enrollNo, const std::string& dept, double studentCgpa)
        : Person(name, personAge, mail),
          enrollmentNo(enrollNo),
          department(dept),
          cgpa(studentCgpa) {
        std::cout << "  [Constructor] Student initialized     : " << enrollmentNo
                  << " (" << department << ")\n";
    }

    ~Student() override {
        std::cout << "  [Destructor]  Student destroyed       : " << enrollmentNo << "\n";
    }

    void displayProfile() const override {
        Person::displayProfile();
        std::cout << "  Enrollment No.     : " << enrollmentNo << "\n"
                  << "  Department         : " << department << "\n"
                  << "  CGPA               : " << std::fixed << std::setprecision(2) << cgpa << "\n";
    }
};

// ============================================================================
// Derived Class (Level 2 - Multilevel Inheritance):
// GraduateStudent inherits from Student (Person -> Student -> GraduateStudent)
// ============================================================================
class GraduateStudent : public Student {
private:
    std::string researchTopic;
    std::string thesisAdvisor;
    double monthlyStipend;
    int publicationsCount;

public:
    GraduateStudent(const std::string& name, int personAge, const std::string& mail,
                    const std::string& enrollNo, const std::string& dept, double studentCgpa,
                    const std::string& topic, const std::string& advisor,
                    double stipend, int publications)
        : Student(name, personAge, mail, enrollNo, dept, studentCgpa),
          researchTopic(topic),
          thesisAdvisor(advisor),
          monthlyStipend(stipend),
          publicationsCount(publications) {
        std::cout << "  [Constructor] GraduateStudent created : Thesis on '" << researchTopic << "'\n";
    }

    ~GraduateStudent() override {
        std::cout << "  [Destructor]  GraduateStudent cleaned : " << fullName << "\n";
    }

    bool isEligibleForDoctoralAward() const {
        return (cgpa >= 8.5 && publicationsCount >= 2);
    }

    void displayProfile() const override {
        Student::displayProfile();
        std::cout << "  Research Topic     : " << researchTopic << "\n"
                  << "  Thesis Advisor     : " << thesisAdvisor << "\n"
                  << "  Monthly Stipend    : Rs. " << std::fixed << std::setprecision(2) << monthlyStipend << "\n"
                  << "  Publications       : " << publicationsCount << "\n"
                  << "  Research Award     : "
                  << (isEligibleForDoctoralAward() ? "Eligible (Merit Scholar)" : "Standard Fellowship") << "\n";
    }
};

int main() {
    std::cout << "========================================================================\n";
    std::cout << " PRACTICAL 4: SINGLE & MULTILEVEL INHERITANCE (UNIVERSITY MANAGEMENT)\n";
    std::cout << " Hierarchy: Person -> Student -> GraduateStudent\n";
    std::cout << "========================================================================\n\n";

    {
        // 1. Single Inheritance Demonstration (Person -> Student)
        std::cout << "[1] Single Inheritance (Person -> Student):\n";
        std::cout << "------------------------------------------------------------------------\n";
        Student undergrad(
            "Ananya Kulkarni", 20, "ananya.k@adypu.edu.in",
            "ADYPU-UG-2025-042", "Computer Engineering", 8.85
        );
        std::cout << "\n--- Undergraduate Student Profile ---\n";
        undergrad.displayProfile();

        // 2. Multilevel Inheritance Demonstration (Person -> Student -> GraduateStudent)
        std::cout << "\n[2] Multilevel Inheritance (Person -> Student -> GraduateStudent):\n";
        std::cout << "------------------------------------------------------------------------\n";
        GraduateStudent gradScholar(
            "Nishant Jha", 23, "nishant.jha@adypu.edu.in",
            "ADYPU-PG-2026-007", "Artificial Intelligence & Robotics", 9.35,
            "High-Frequency Low-Latency C++ Systems", "Dr. Ayushi Godiya",
            35000.00, 3
        );
        std::cout << "\n--- Graduate Research Scholar Profile ---\n";
        gradScholar.displayProfile();

        std::cout << "\n[3] Destructor Execution Order (Reverse of Construction):\n";
        std::cout << "------------------------------------------------------------------------\n";
    }

    std::cout << "========================================================================\n";
    return 0;
}
