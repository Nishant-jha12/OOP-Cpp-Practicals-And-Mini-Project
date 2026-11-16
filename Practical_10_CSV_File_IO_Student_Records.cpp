// ============================================================================
// Practical 10: CSV File Reader/Writer & File Pointers (seekg, seekp, tellg, tellp)
// Course      : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus    : Unit 5 - File Handling (fstream, ios::in, ios::out, ios::app,
//               ios::binary, seekg(), seekp(), tellg(), tellp())
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

struct StudentCSVRecord {
    int rollNo;
    std::string name;
    std::string branch;
    double cgpa;

    std::string toCSVRow() const {
        std::ostringstream oss;
        oss << rollNo << ","
            << std::setw(16) << std::left << name << ","
            << std::setw(8) << std::left << branch << ","
            << std::fixed << std::setprecision(2) << cgpa;
        return oss.str();
    }

    static StudentCSVRecord fromCSVRow(const std::string& line) {
        StudentCSVRecord rec;
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        rec.rollNo = std::stoi(token);

        std::getline(ss, rec.name, ',');
        // Trim trailing spaces used for fixed-width alignment
        while (!rec.name.empty() && rec.name.back() == ' ') rec.name.pop_back();

        std::getline(ss, rec.branch, ',');
        while (!rec.branch.empty() && rec.branch.back() == ' ') rec.branch.pop_back();

        std::getline(ss, token, ',');
        rec.cgpa = std::stod(token);

        return rec;
    }
};

class StudentCSVManager {
private:
    std::string filename;

public:
    explicit StudentCSVManager(const std::string& file) : filename(file) {}

    // 1. Write initial CSV file using ios::out | ios::trunc | ios::binary and track tellp()
    void writeInitialRecords(const std::vector<StudentCSVRecord>& records,
                             std::vector<std::streampos>& recordOffsets) {
        std::fstream outFile(filename, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "  [Error] Unable to open " << filename << " for writing!\n";
            return;
        }

        // Write CSV Header
        outFile << "RollNo,Name            ,Branch  ,CGPA\n";
        std::cout << "  [tellp()] Header written. Current put pointer offset: "
                  << outFile.tellp() << " bytes\n";

        for (const auto& rec : records) {
            std::streampos posBeforeWrite = outFile.tellp();
            recordOffsets.push_back(posBeforeWrite);
            outFile << rec.toCSVRow() << "\n";
            std::cout << "  [Write ] Roll #" << rec.rollNo << " written at byte offset "
                      << std::setw(3) << posBeforeWrite
                      << " -> tellp() after write = " << outFile.tellp() << " bytes\n";
        }
        outFile.close();
    }

    // 2. Append a new record using ios::out | ios::app | ios::binary
    void appendRecord(const StudentCSVRecord& rec, std::vector<std::streampos>& recordOffsets) {
        std::fstream appFile(filename, std::ios::out | std::ios::app | std::ios::binary);
        if (!appFile.is_open()) return;

        appFile.seekp(0, std::ios::end);
        std::streampos appendOffset = appFile.tellp();
        recordOffsets.push_back(appendOffset);
        appFile << rec.toCSVRow() << "\n";
        std::cout << "  [Append] Roll #" << rec.rollNo << " appended at byte offset "
                  << appendOffset << " -> tellp() = " << appFile.tellp() << " bytes\n";
        appFile.close();
    }

    // 3. Read all CSV records sequentially using ios::in | ios::binary
    void readAllRecords() const {
        std::fstream inFile(filename, std::ios::in | std::ios::binary);
        if (!inFile.is_open()) return;

        std::string line;
        std::getline(inFile, line); // Skip CSV header
        std::cout << "  Roll No | Name               | Branch   | CGPA\n";
        std::cout << "  --------+--------------------+----------+------\n";

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            StudentCSVRecord rec = StudentCSVRecord::fromCSVRow(line);
            std::cout << "    " << std::setw(4) << rec.rollNo << "  | "
                      << std::setw(18) << std::left << rec.name << " | "
                      << std::setw(8) << std::left << rec.branch << " | "
                      << std::fixed << std::setprecision(2) << rec.cgpa << "\n";
        }
        inFile.close();
    }

    // 4. Random Access Read using seekg() and tellg()
    void readRecordAtOffset(std::streampos offset) const {
        std::fstream file(filename, std::ios::in | std::ios::binary);
        if (!file.is_open()) return;

        // Move get pointer directly to target record offset
        file.seekg(offset, std::ios::beg);
        std::cout << "  [seekg()] Moved get pointer to offset: " << file.tellg() << " bytes\n";

        std::string line;
        if (std::getline(file, line)) {
            StudentCSVRecord rec = StudentCSVRecord::fromCSVRow(line);
            std::cout << "  [Direct Read] Roll #" << rec.rollNo << " | Name: " << rec.name
                      << " | Branch: " << rec.branch << " | CGPA: " << rec.cgpa << "\n";
            std::cout << "  [tellg()] Get pointer after reading line: " << file.tellg() << " bytes\n";
        }
        file.close();
    }

    // 5. In-Place Update of a Record using seekp() and tellp()
    void updateRecordAtOffset(std::streampos offset, const StudentCSVRecord& updatedRec) {
        std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;

        // Jump put pointer directly to the target record's byte offset
        file.seekp(offset, std::ios::beg);
        std::cout << "  [seekp()] Positioned put pointer at offset: " << file.tellp() << " bytes\n";
        file << updatedRec.toCSVRow() << "\n";
        std::cout << "  [Update ] Overwrote Roll #" << updatedRec.rollNo
                  << " with updated CGPA = " << updatedRec.cgpa
                  << " (tellp() now at " << file.tellp() << " bytes)\n";
        file.close();
    }
};

int main() {
    std::cout << "================================================================================\n";
    std::cout << " PRACTICAL 10: CSV FILE READER/WRITER & FILE POINTERS (seekg, seekp, tellg, tellp)\n";
    std::cout << "================================================================================\n\n";

    const std::string csvFile = "students_records.csv";
    StudentCSVManager manager(csvFile);
    std::vector<std::streampos> offsets;

    std::vector<StudentCSVRecord> initialBatch = {
        {101, "Nishant Jha",    "CSE-AI", 9.35},
        {102, "Aarav Sharma",   "CSE",    8.60},
        {103, "Priya Nair",     "ENTC",   8.95}
    };

    // 1. Write CSV Records & Track tellp()
    std::cout << "[1] Writing Student Records to '" << csvFile << "' (Tracking tellp()):\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.writeInitialRecords(initialBatch, offsets);
    std::cout << "\n";

    // 2. Append a Record using ios::app
    std::cout << "[2] Appending New Student Record using std::ios::app:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.appendRecord({104, "Rohan Kulkarni", "MECH", 8.20}, offsets);
    std::cout << "\n";

    // 3. Read All Records from CSV
    std::cout << "[3] Reading All Records from CSV File (std::ios::in):\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.readAllRecords();
    std::cout << "\n";

    // 4. Random Access Read using seekg() and tellg() (Read 2nd student directly)
    std::cout << "[4] Random Access Retrieval of Record #2 using seekg() & tellg():\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.readRecordAtOffset(offsets[1]);
    std::cout << "\n";

    // 5. In-Place Record Update using seekp() and tellp() (Update Roll #102 CGPA from 8.60 -> 9.10)
    std::cout << "[5] In-Place Update of Record #2 using seekp() & tellp():\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.updateRecordAtOffset(offsets[1], {102, "Aarav Sharma", "CSE", 9.10});
    std::cout << "\n";

    // 6. Verify Updated CSV File Contents
    std::cout << "[6] Verifying CSV Records After In-Place seekp() Update:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    manager.readAllRecords();
    std::cout << "================================================================================\n";

    return 0;
}
