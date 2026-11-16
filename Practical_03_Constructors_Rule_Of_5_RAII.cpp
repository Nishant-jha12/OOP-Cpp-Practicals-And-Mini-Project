#include <iostream>
#include <string>

using namespace std;

class IntBuffer {
public:
    string name;
    int size;
    int* data;

    IntBuffer() {
        name = "Default";
        size = 3;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
        cout << "[Default Constructor] Created " << name << "\n";
    }

    IntBuffer(string n, int s, int startValue) {
        name = n;
        size = s;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = startValue + (i * 10);
        }
        cout << "[Parameterized Constructor] Created " << name << "\n";
    }

    IntBuffer(const IntBuffer& other) {
        name = other.name + "_Copy";
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "[Copy Constructor] Copied into " << name << "\n";
    }

    IntBuffer& operator=(const IntBuffer& other) {
        if (this != &other) {
            delete[] data;
            
            name = other.name + "_Assigned";
            size = other.size;
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
            cout << "[Assignment Operator] Assigned into " << name << "\n";
        }
        return *this;
    }

    ~IntBuffer() {
        delete[] data;
        cout << "[Destructor] Deleted memory for " << name << "\n";
    }

    void print() {
        cout << name << " -> { ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << "}\n";
    }
};

int main() {
    cout << "--- 1. Default Constructor ---\n";
    IntBuffer b1;
    b1.print();

    cout << "\n--- 2. Parameterized Constructor ---\n";
    IntBuffer b2("Sensor", 5, 10);
    b2.print();

    cout << "\n--- 3. Copy Constructor ---\n";
    IntBuffer b3(b2);
    b3.print();

    cout << "\n--- 4. Assignment Operator ---\n";
    IntBuffer b4;
    b4 = b2;
    b4.print();

    cout << "\n--- 5. Destructors called automatically here ---\n";

    return 0;
}
