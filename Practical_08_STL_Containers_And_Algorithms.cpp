// ============================================================================
// Practical 8: STL Containers and Algorithms for Student Marks Dataset
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 4 - STL Containers (vector, map, unordered_map), Iterators,
//              STL Algorithms (sort, find_if, transform, accumulate, for_each),
//              and Lambda Expressions as Predicates
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>

struct StudentRecord {
    int rollNo;
    std::string name;
    double rawMarks;
    double gracedMarks;
    char grade;
};

char computeGrade(double marks) {
    if (marks >= 90.0) return 'O';
    if (marks >= 80.0) return 'A';
    if (marks >= 70.0) return 'B';
    if (marks >= 60.0) return 'C';
    return 'P';
}

int main() {
    std::cout << "================================================================================\n";
    std::cout << " PRACTICAL 8: STL CONTAINERS & ALGORITHMS (STUDENT MARKS DATASET)\n";
    std::cout << "================================================================================\n\n";

    // ------------------------------------------------------------------------
    // 1. std::vector<StudentRecord> Dataset Initialization
    // ------------------------------------------------------------------------
    std::vector<StudentRecord> students = {
        {104, "Riya Sharma",     82.5, 82.5, 'A'},
        {101, "Nishant Jha",     91.0, 91.0, 'O'},
        {105, "Kabir Deshmukh",  67.0, 67.0, 'C'},
        {102, "Aarav Kulkarni",  76.5, 76.5, 'B'},
        {106, "Sneha Patil",     88.0, 88.0, 'A'},
        {103, "Vikram Joshi",    58.5, 58.5, 'P'}
    };

    // ------------------------------------------------------------------------
    // 2. std::transform with Lambda: Add +3.0 Grace Marks (capped at 100.0)
    // ------------------------------------------------------------------------
    std::cout << "[1] Applying +3.0 Grace Marks using std::transform & Lambda:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::transform(students.begin(), students.end(), students.begin(),
        [](StudentRecord s) {
            s.gracedMarks = std::min(100.0, s.rawMarks + 3.0);
            s.grade = computeGrade(s.gracedMarks);
            return s;
        }
    );

    // ------------------------------------------------------------------------
    // 3. std::sort with Lambda Comparator: Sort by Graced Marks (Descending)
    // ------------------------------------------------------------------------
    std::sort(students.begin(), students.end(),
        [](const StudentRecord& a, const StudentRecord& b) {
            return a.gracedMarks > b.gracedMarks;
        }
    );

    std::cout << "  Rank | Roll No | Name               | Raw Marks | Graced Marks | Grade\n";
    std::cout << "  -----+---------+--------------------+-----------+--------------+------\n";
    int rank = 1;
    std::for_each(students.begin(), students.end(),
        [&rank](const StudentRecord& s) {
            std::cout << "   " << std::setw(2) << rank++ << "  |   "
                      << std::setw(4) << s.rollNo << "  | "
                      << std::setw(18) << std::left << s.name << " |   "
                      << std::setw(6) << std::fixed << std::setprecision(2) << s.rawMarks << "  |    "
                      << std::setw(6) << s.gracedMarks << "    |   " << s.grade << "\n";
        }
    );
    std::cout << "\n";

    // ------------------------------------------------------------------------
    // 4. std::find_if with Lambda Predicate: Find First Student with Marks >= 90
    //    and First Student Requiring Remedial Support (Graced Marks < 65)
    // ------------------------------------------------------------------------
    std::cout << "[2] Searching Dataset using std::find_if & Lambda Predicates:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    auto topperIt = std::find_if(students.begin(), students.end(),
        [](const StudentRecord& s) { return s.gracedMarks >= 90.0; }
    );
    if (topperIt != students.end()) {
        std::cout << "  Topper (>= 90 Marks)      : " << topperIt->name
                  << " (Roll #" << topperIt->rollNo << ", Marks: " << topperIt->gracedMarks << ")\n";
    }

    auto remedialIt = std::find_if(students.begin(), students.end(),
        [](const StudentRecord& s) { return s.gracedMarks < 65.0; }
    );
    if (remedialIt != students.end()) {
        std::cout << "  Remedial Support (< 65)   : " << remedialIt->name
                  << " (Roll #" << remedialIt->rollNo << ", Marks: " << remedialIt->gracedMarks << ")\n";
    }

    // Calculate Class Average using std::accumulate
    double totalMarks = std::accumulate(students.begin(), students.end(), 0.0,
        [](double sum, const StudentRecord& s) { return sum + s.gracedMarks; }
    );
    std::cout << "  Class Average (accumulate): " << (totalMarks / students.size()) << "\n\n";

    // ------------------------------------------------------------------------
    // 5. std::map (Ordered by Roll Number - Red-Black Tree, O(log N))
    // ------------------------------------------------------------------------
    std::cout << "[3] std::map<int, StudentRecord> (Ordered by Roll Number):\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::map<int, StudentRecord> rollNoDirectory;
    for (const auto& s : students) {
        rollNoDirectory[s.rollNo] = s;
    }

    for (auto it = rollNoDirectory.cbegin(); it != rollNoDirectory.cend(); ++it) {
        std::cout << "  Roll #" << it->first << " -> "
                  << std::setw(18) << std::left << it->second.name
                  << " | Marks: " << it->second.gracedMarks
                  << " | Grade: " << it->second.grade << "\n";
    }
    std::cout << "\n";

    // ------------------------------------------------------------------------
    // 6. std::unordered_map (Hash Table O(1) Lookup by Name & Grade Count)
    // ------------------------------------------------------------------------
    std::cout << "[4] std::unordered_map<std::string, double> (O(1) Hash Lookup by Name):\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::unordered_map<std::string, double> nameToMarksMap;
    for (const auto& s : students) {
        nameToMarksMap[s.name] = s.gracedMarks;
    }

    std::string searchName = "Nishant Jha";
    auto foundIt = nameToMarksMap.find(searchName);
    if (foundIt != nameToMarksMap.end()) {
        std::cout << "  Hash Lookup ['" << searchName << "'] -> Marks = "
                  << foundIt->second << "\n";
    }
    std::cout << "================================================================================\n";

    return 0;
}
