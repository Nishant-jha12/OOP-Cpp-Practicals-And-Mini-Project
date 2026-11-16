// ============================================================================
// Practical 3: All Constructor Types, Rule of 5, RAII & Memory Leak Verification
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 2 - Constructors (Default, Parameterized, Copy, Move),
//              Rule of 3/5/0, Constructor Delegation, Initializer Lists,
//              RAII & Smart Pointers (unique_ptr, shared_ptr, weak_ptr)
// ============================================================================

#include <iostream>
#include <cstring>
#include <utility>
#include <memory>
#include <string>

// ============================================================================
// Memory Tracker to Verify Zero Memory Leaks (Alongside Valgrind)
// ============================================================================
struct MemoryTracker {
    static int activeBlocks;
    static size_t totalBytesAllocated;
    static size_t totalBytesFreed;

    static void recordAlloc(size_t bytes) {
        activeBlocks++;
        totalBytesAllocated += bytes;
    }

    static void recordFree(size_t bytes) {
        activeBlocks--;
        totalBytesFreed += bytes;
    }

    static void printReport() {
        std::cout << "========================================================================\n";
        std::cout << " MEMORY LEAK VERIFICATION SUMMARY (HEAP AUDIT)\n";
        std::cout << "========================================================================\n";
        std::cout << "  Total Heap Bytes Allocated : " << totalBytesAllocated << " bytes\n";
        std::cout << "  Total Heap Bytes Freed     : " << totalBytesFreed << " bytes\n";
        std::cout << "  Active Unfreed Blocks      : " << activeBlocks << "\n";
        std::cout << "  Status                     : "
                  << ((activeBlocks == 0 && totalBytesAllocated == totalBytesFreed)
                      ? "ALL HEAP BLOCKS FREED - NO MEMORY LEAKS!"
                      : "WARNING: MEMORY LEAK DETECTED!")
                  << "\n";
        std::cout << "========================================================================\n";
    }
};

int MemoryTracker::activeBlocks = 0;
size_t MemoryTracker::totalBytesAllocated = 0;
size_t MemoryTracker::totalBytesFreed = 0;

// ============================================================================
// Class Demonstrating All Constructor Types & The Rule of 5
// ============================================================================
class DynamicIntBuffer {
private:
    std::string label;
    size_t size;
    int* data;

public:
    // 1. Default Constructor (Uses Constructor Delegation to Parameterized Constructor)
    DynamicIntBuffer() : DynamicIntBuffer("DefaultBuffer", 4, 0) {
        std::cout << "  [Default Ctor] Delegated creation for '" << label << "'\n";
    }

    // 2. Parameterized Constructor (Uses Member Initializer List)
    DynamicIntBuffer(const std::string& lbl, size_t sz, int initialValue)
        : label(lbl), size(sz), data(nullptr) {
        if (size > 0) {
            data = new int[size];
            MemoryTracker::recordAlloc(size * sizeof(int));
            for (size_t i = 0; i < size; ++i) {
                data[i] = initialValue + static_cast<int>(i) * 10;
            }
        }
        std::cout << "  [Parameterized Ctor] Allocated '" << label
                  << "' (" << size << " ints at " << data << ")\n";
    }

    // 3. Copy Constructor (Deep Copy - Rule of 5 #1)
    DynamicIntBuffer(const DynamicIntBuffer& other)
        : label(other.label + "_Copy"), size(other.size), data(nullptr) {
        if (size > 0 && other.data != nullptr) {
            data = new int[size];
            MemoryTracker::recordAlloc(size * sizeof(int));
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        std::cout << "  [Copy Ctor] Deep copied '" << other.label << "' -> '"
                  << label << "' (new address " << data << ")\n";
    }

    // 4. Copy Assignment Operator (Deep Copy - Rule of 5 #2)
    DynamicIntBuffer& operator=(const DynamicIntBuffer& other) {
        std::cout << "  [Copy Assignment] Assigning '" << other.label
                  << "' into '" << label << "'\n";
        if (this == &other) {
            return *this; // Self-assignment guard
        }

        // Release existing resource
        if (data != nullptr) {
            delete[] data;
            MemoryTracker::recordFree(size * sizeof(int));
        }

        label = other.label + "_Assigned";
        size = other.size;
        data = nullptr;
        if (size > 0 && other.data != nullptr) {
            data = new int[size];
            MemoryTracker::recordAlloc(size * sizeof(int));
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // 5. Move Constructor (Steals resource in O(1) - Rule of 5 #3)
    DynamicIntBuffer(DynamicIntBuffer&& other) noexcept
        : label(std::move(other.label) + "_Moved"), size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr; // Leave source object in valid empty state
        std::cout << "  [Move Ctor] Transferred ownership to '" << label
                  << "' (address " << data << ") with zero extra allocation\n";
    }

    // 6. Move Assignment Operator (Steals resource in O(1) - Rule of 5 #4)
    DynamicIntBuffer& operator=(DynamicIntBuffer&& other) noexcept {
        std::cout << "  [Move Assignment] Moving resource into '" << label << "'\n";
        if (this == &other) {
            return *this;
        }

        if (data != nullptr) {
            delete[] data;
            MemoryTracker::recordFree(size * sizeof(int));
        }

        label = std::move(other.label) + "_MoveAssigned";
        size = other.size;
        data = other.data;

        other.size = 0;
        other.data = nullptr;
        return *this;
    }

    // 7. Destructor (RAII Cleanup - Rule of 5 #5)
    ~DynamicIntBuffer() {
        if (data != nullptr) {
            std::cout << "  [Destructor] Releasing heap memory of '" << label
                      << "' (" << size * sizeof(int) << " bytes at " << data << ")\n";
            delete[] data;
            MemoryTracker::recordFree(size * sizeof(int));
            data = nullptr;
        } else {
            std::cout << "  [Destructor] Destroyed moved-from/empty object '" << label << "'\n";
        }
    }

    void print() const {
        std::cout << "    -> Buffer '" << label << "' [size=" << size << ", addr=" << data << "]: { ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << (i + 1 < size ? ", " : " ");
        }
        std::cout << "}\n";
    }
};

// ============================================================================
// RAII with Modern C++ Smart Pointers (unique_ptr, shared_ptr, weak_ptr)
// ============================================================================
void demonstrateSmartPointers() {
    std::cout << "\n[6] RAII with Smart Pointers (unique_ptr, shared_ptr, weak_ptr):\n";
    std::cout << "------------------------------------------------------------------------\n";

    // Exclusive ownership using std::unique_ptr
    std::unique_ptr<DynamicIntBuffer> uniqueBuf(
        new DynamicIntBuffer("UniquePtrBuffer", 3, 100)
    );
    uniqueBuf->print();

    // Shared ownership using std::shared_ptr & non-owning std::weak_ptr
    std::shared_ptr<DynamicIntBuffer> sharedBuf1 =
        std::make_shared<DynamicIntBuffer>("SharedPtrBuffer", 3, 500);
    std::weak_ptr<DynamicIntBuffer> weakObserver = sharedBuf1;
    {
        std::shared_ptr<DynamicIntBuffer> sharedBuf2 = sharedBuf1;
        std::cout << "  shared_ptr reference count (inside inner scope) : "
                  << sharedBuf1.use_count() << "\n";
        sharedBuf2->print();
    }
    std::cout << "  shared_ptr reference count (after inner scope)  : "
              << sharedBuf1.use_count() << "\n";
    std::cout << "  weak_ptr expired?                               : "
              << (weakObserver.expired() ? "Yes" : "No") << "\n";
}

int main() {
    std::cout << "========================================================================\n";
    std::cout << " PRACTICAL 3: CONSTRUCTORS (DEFAULT, PARAM, COPY, MOVE) & RULE OF 5\n";
    std::cout << "========================================================================\n\n";

    {
        // 1. Default Constructor (with Constructor Delegation)
        std::cout << "[1] Default Constructor:\n";
        std::cout << "------------------------------------------------------------------------\n";
        DynamicIntBuffer buf1;
        buf1.print();

        // 2. Parameterized Constructor
        std::cout << "\n[2] Parameterized Constructor:\n";
        std::cout << "------------------------------------------------------------------------\n";
        DynamicIntBuffer buf2("SensorData", 5, 10);
        buf2.print();

        // 3. Copy Constructor (Deep Copy)
        std::cout << "\n[3] Copy Constructor (Deep Copy):\n";
        std::cout << "------------------------------------------------------------------------\n";
        DynamicIntBuffer buf3(buf2);
        buf3.print();

        // 4. Copy Assignment Operator
        std::cout << "\n[4] Copy Assignment Operator:\n";
        std::cout << "------------------------------------------------------------------------\n";
        buf1 = buf2;
        buf1.print();

        // 5. Move Constructor & Move Assignment Operator
        std::cout << "\n[5] Move Constructor & Move Assignment Operator:\n";
        std::cout << "------------------------------------------------------------------------\n";
        DynamicIntBuffer buf4(std::move(buf2)); // Move constructor
        buf4.print();
        buf2.print(); // Shows buf2 is safely in an empty state (size=0, addr=0)

        DynamicIntBuffer buf5("TempTarget", 2, 99);
        buf5 = std::move(buf3); // Move assignment
        buf5.print();

        // 6. Smart Pointers (RAII)
        demonstrateSmartPointers();

        std::cout << "\n[7] Exiting Scope - Automatic RAII Destructor Calls:\n";
        std::cout << "------------------------------------------------------------------------\n";
    }

    std::cout << "\n";
    MemoryTracker::printReport();
    return 0;
}
