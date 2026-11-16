#include <iostream>
#include <string>

using namespace std;

template <typename T>
class Stack {
private:
    T arr[10];
    int top;

public:
    Stack() {
        top = -1;
    }

    void push(T val) {
        if (top < 9) {
            top++;
            arr[top] = val;
            cout << "Pushed: " << val << "\n";
        } else {
            cout << "Stack Full!\n";
        }
    }

    void pop() {
        if (top >= 0) {
            cout << "Popped: " << arr[top] << "\n";
            top--;
        } else {
            cout << "Stack Empty!\n";
        }
    }

    void display() {
        cout << "Stack (bottom to top): ";
        for (int i = 0; i <= top; i++) {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }
};

template <typename T>
class Queue {
private:
    T arr[10];
    int front;
    int rear;
    int count;

public:
    Queue() {
        front = 0;
        rear = 0;
        count = 0;
    }

    void enqueue(T val) {
        if (count < 10) {
            arr[rear] = val;
            rear = (rear + 1) % 10;
            count++;
            cout << "Enqueued: " << val << "\n";
        } else {
            cout << "Queue Full!\n";
        }
    }

    void dequeue() {
        if (count > 0) {
            cout << "Dequeued: " << arr[front] << "\n";
            front = (front + 1) % 10;
            count--;
        } else {
            cout << "Queue Empty!\n";
        }
    }

    void display() {
        cout << "Queue (front to rear): ";
        for (int i = 0; i < count; i++) {
            int index = (front + i) % 10;
            cout << arr[index] << " ";
        }
        cout << "\n";
    }
};

int main() {
    cout << "--- 1. Testing Stack<int> ---\n";
    Stack<int> s1;
    s1.push(10);
    s1.push(20);
    s1.push(30);
    s1.display();
    s1.pop();
    s1.display();

    cout << "\n--- 2. Testing Stack<float> ---\n";
    Stack<float> s2;
    s2.push(3.14);
    s2.push(6.28);
    s2.display();
    s2.pop();
    
    cout << "\n--- 3. Testing Stack<string> ---\n";
    Stack<string> s3;
    s3.push("Apple");
    s3.push("Banana");
    s3.display();

    cout << "\n--- 4. Testing Queue<int> ---\n";
    Queue<int> q1;
    q1.enqueue(101);
    q1.enqueue(102);
    q1.enqueue(103);
    q1.display();
    q1.dequeue();
    q1.display();
    
    cout << "\n--- 5. Testing Queue<string> ---\n";
    Queue<string> q2;
    q2.enqueue("Customer_A");
    q2.enqueue("Customer_B");
    q2.display();
    q2.dequeue();
    q2.display();

    return 0;
}
