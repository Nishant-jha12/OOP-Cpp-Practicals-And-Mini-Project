// ============================================================================
// Practical 7: Generic Stack and Queue Classes using C++ Templates
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 4 - Generic Programming (Class Templates, Function
//              Templates, Template Specialization) tested with int, float,
//              and std::string
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

// ============================================================================
// Function Template + Explicit Template Specialization for std::string
// ============================================================================
template <typename T>
void printFormattedItem(const T& item) {
    std::cout << item;
}

// Template Specialization for std::string (adds quotes around string items)
template <>
void printFormattedItem<std::string>(const std::string& item) {
    std::cout << "\"" << item << "\"";
}

// ============================================================================
// Class Template 1: Generic Stack<T, Capacity> (LIFO - Last In First Out)
// ============================================================================
template <typename T, size_t Capacity = 10>
class GenericStack {
private:
    T elements[Capacity];
    int topIndex;

public:
    GenericStack() : topIndex(-1) {}

    bool isEmpty() const { return topIndex == -1; }
    bool isFull() const { return topIndex == static_cast<int>(Capacity) - 1; }
    size_t size() const { return static_cast<size_t>(topIndex + 1); }

    void push(const T& value) {
        if (isFull()) {
            throw std::overflow_error("Stack Overflow: Maximum capacity reached!");
        }
        elements[++topIndex] = value;
        std::cout << "  [Stack Push] ";
        printFormattedItem(value);
        std::cout << " (size = " << size() << ")\n";
    }

    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Cannot pop from an empty stack!");
        }
        T removed = elements[topIndex--];
        std::cout << "  [Stack Pop ] ";
        printFormattedItem(removed);
        std::cout << " (remaining = " << size() << ")\n";
        return removed;
    }

    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty!");
        }
        return elements[topIndex];
    }

    void display() const {
        std::cout << "  [Stack State (Bottom -> Top)] : [ ";
        for (int i = 0; i <= topIndex; ++i) {
            printFormattedItem(elements[i]);
            if (i < topIndex) std::cout << ", ";
        }
        std::cout << " ]\n";
    }
};

// ============================================================================
// Class Template 2: Generic Circular Queue<T, Capacity> (FIFO - First In First Out)
// ============================================================================
template <typename T, size_t Capacity = 10>
class GenericQueue {
private:
    T elements[Capacity];
    size_t frontIdx;
    size_t rearIdx;
    size_t count;

public:
    GenericQueue() : frontIdx(0), rearIdx(0), count(0) {}

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == Capacity; }
    size_t size() const { return count; }

    void enqueue(const T& value) {
        if (isFull()) {
            throw std::overflow_error("Queue Overflow: Maximum capacity reached!");
        }
        elements[rearIdx] = value;
        rearIdx = (rearIdx + 1) % Capacity;
        count++;
        std::cout << "  [Enqueue] ";
        printFormattedItem(value);
        std::cout << " (size = " << count << ")\n";
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue Underflow: Cannot dequeue from empty queue!");
        }
        T removed = elements[frontIdx];
        frontIdx = (frontIdx + 1) % Capacity;
        count--;
        std::cout << "  [Dequeue] ";
        printFormattedItem(removed);
        std::cout << " (remaining = " << count << ")\n";
        return removed;
    }

    const T& front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty!");
        }
        return elements[frontIdx];
    }

    void display() const {
        std::cout << "  [Queue State (Front -> Rear)] : [ ";
        for (size_t i = 0; i < count; ++i) {
            size_t idx = (frontIdx + i) % Capacity;
            printFormattedItem(elements[idx]);
            if (i + 1 < count) std::cout << " <- ";
        }
        std::cout << " ]\n";
    }
};

int main() {
    std::cout << "========================================================================\n";
    std::cout << " PRACTICAL 7: GENERIC STACK & QUEUE USING C++ TEMPLATES\n";
    std::cout << " Tested with: int, float, and std::string\n";
    std::cout << "========================================================================\n\n";

    std::cout << std::fixed << std::setprecision(2);

    // 1. Testing GenericStack<int>
    std::cout << "[1] Testing GenericStack<int>:\n";
    std::cout << "------------------------------------------------------------------------\n";
    GenericStack<int, 5> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    intStack.display();
    std::cout << "  Top element (peek): " << intStack.peek() << "\n";
    intStack.pop();
    intStack.display();
    std::cout << "\n";

    // 2. Testing GenericStack<float>
    std::cout << "[2] Testing GenericStack<float>:\n";
    std::cout << "------------------------------------------------------------------------\n";
    GenericStack<float, 5> floatStack;
    floatStack.push(3.14f);
    floatStack.push(6.28f);
    floatStack.push(9.81f);
    floatStack.display();
    floatStack.pop();
    floatStack.display();
    std::cout << "\n";

    // 3. Testing GenericStack<std::string> (Also triggers Template Specialization)
    std::cout << "[3] Testing GenericStack<std::string>:\n";
    std::cout << "------------------------------------------------------------------------\n";
    GenericStack<std::string, 5> stringStack;
    stringStack.push("Encapsulation");
    stringStack.push("Inheritance");
    stringStack.push("Polymorphism");
    stringStack.display();
    stringStack.pop();
    stringStack.display();
    std::cout << "\n";

    // 4. Testing GenericQueue<int>, GenericQueue<float>, and GenericQueue<std::string>
    std::cout << "[4] Testing GenericQueue<int>, <float>, and <std::string>:\n";
    std::cout << "------------------------------------------------------------------------\n";
    GenericQueue<int, 5> intQueue;
    intQueue.enqueue(101);
    intQueue.enqueue(102);
    intQueue.enqueue(103);
    intQueue.display();
    intQueue.dequeue();
    intQueue.display();

    std::cout << "\n";
    GenericQueue<float, 5> floatQueue;
    floatQueue.enqueue(98.6f);
    floatQueue.enqueue(100.4f);
    floatQueue.display();

    std::cout << "\n";
    GenericQueue<std::string, 5> stringQueue;
    stringQueue.enqueue("Customer_A");
    stringQueue.enqueue("Customer_B");
    stringQueue.enqueue("Customer_C");
    stringQueue.display();
    stringQueue.dequeue();
    stringQueue.display();
    std::cout << "========================================================================\n";

    return 0;
}
