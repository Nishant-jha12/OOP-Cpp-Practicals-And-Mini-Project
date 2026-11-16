#include <iostream>
#include <cmath>

using namespace std;

class Shape {
public:
    virtual double area() = 0;
    virtual double perimeter() = 0;
    
    virtual void display() {
        cout << "Area: " << area() << " | Perimeter: " << perimeter() << "\n";
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) {
        radius = r;
    }

    double area() {
        return 3.14159 * radius * radius;
    }

    double perimeter() {
        return 2.0 * 3.14159 * radius;
    }

    void display() {
        cout << "Circle (r=" << radius << ") -> ";
        Shape::display();
    }
};

class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) {
        length = l;
        width = w;
    }

    double area() {
        return length * width;
    }

    double perimeter() {
        return 2.0 * (length + width);
    }

    void display() {
        cout << "Rectangle (l=" << length << ", w=" << width << ") -> ";
        Shape::display();
    }
};

class Triangle : public Shape {
private:
    double a, b, c;

public:
    Triangle(double sideA, double sideB, double sideC) {
        a = sideA;
        b = sideB;
        c = sideC;
    }

    double area() {
        double s = (a + b + c) / 2.0;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }

    double perimeter() {
        return a + b + c;
    }
