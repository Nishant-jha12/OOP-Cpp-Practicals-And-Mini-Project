// ============================================================================
// Practical 5: Runtime Polymorphism - Abstract Base Class & Shape Hierarchy
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 3 - Runtime Polymorphism (Virtual Functions, Pure Virtual
//              Functions, Abstract Class, Vtable Mechanism, Virtual Destructor)
// ============================================================================

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <memory>
#include <string>

// ============================================================================
// Abstract Base Class: Shape
// Contains Pure Virtual Functions (= 0) -> Cannot be instantiated directly.
// Compiler generates a vtable (Virtual Method Table) for dynamic dispatch.
// ============================================================================
class Shape {
protected:
    std::string shapeName;

public:
    explicit Shape(const std::string& name) : shapeName(name) {}

    // Virtual Destructor ensures derived class destructors are called via Shape*
    virtual ~Shape() {
        std::cout << "  [~Shape] Destroyed base shape: " << shapeName << "\n";
    }

    // Pure Virtual Functions
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void displayDimensions() const = 0;

    // Virtual Function with default implementation
    virtual void printReport() const {
        std::cout << "  Shape: " << std::setw(12) << std::left << shapeName << " | ";
        displayDimensions();
        std::cout << " | Area = " << std::setw(8) << std::fixed << std::setprecision(2) << area()
                  << " | Perimeter = " << std::setw(8) << perimeter() << "\n";
    }
};

// ============================================================================
// Derived Class 1: Circle
// ============================================================================
class Circle : public Shape {
private:
    double radius;
    static constexpr double PI = 3.141592653589793;

public:
    explicit Circle(double r) : Shape("Circle"), radius(r) {}

    ~Circle() override {
        std::cout << "  [~Circle] Cleaning up Circle (r = " << radius << ")\n";
    }

    double area() const override {
        return PI * radius * radius;
    }

    double perimeter() const override {
        return 2.0 * PI * radius;
    }

    void displayDimensions() const override {
        std::cout << std::setw(24) << std::left
                  << ("r = " + std::to_string(static_cast<int>(radius)));
    }
};

// ============================================================================
// Derived Class 2: Rectangle
// ============================================================================
class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) : Shape("Rectangle"), length(l), width(w) {}

    ~Rectangle() override {
        std::cout << "  [~Rectangle] Cleaning up Rectangle (" << length << " x " << width << ")\n";
    }

    double area() const override {
        return length * width;
    }

    double perimeter() const override {
        return 2.0 * (length + width);
    }

    void displayDimensions() const override {
        std::cout << std::setw(24) << std::left
                  << ("l = " + std::to_string(static_cast<int>(length)) +
                      ", w = " + std::to_string(static_cast<int>(width)));
    }
};

// ============================================================================
// Derived Class 3: Triangle (Heron's Formula)
// ============================================================================
class Triangle : public Shape {
private:
    double sideA, sideB, sideC;

public:
    Triangle(double a, double b, double c)
        : Shape("Triangle"), sideA(a), sideB(b), sideC(c) {}

    ~Triangle() override {
        std::cout << "  [~Triangle] Cleaning up Triangle ("
                  << sideA << ", " << sideB << ", " << sideC << ")\n";
    }

    double area() const override {
        double s = (sideA + sideB + sideC) / 2.0;
        return std::sqrt(s * (s - sideA) * (s - sideB) * (s - sideC));
    }

    double perimeter() const override {
        return sideA + sideB + sideC;
    }

    void displayDimensions() const override {
        std::cout << std::setw(24) << std::left
                  << ("sides = " + std::to_string(static_cast<int>(sideA)) + "," +
                      std::to_string(static_cast<int>(sideB)) + "," +
                      std::to_string(static_cast<int>(sideC)));
    }
};

int main() {
    std::cout << "====================================================================================\n";
    std::cout << " PRACTICAL 5: RUNTIME POLYMORPHISM (ABSTRACT CLASS & SHAPE HIERARCHY)\n";
    std::cout << "====================================================================================\n\n";

    std::cout << "[1] Dynamic Dispatch via Base Class Pointers (Shape*):\n";
    std::cout << "------------------------------------------------------------------------------------\n";

    // Collection of Base Class Smart Pointers pointing to Derived Objects
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::unique_ptr<Shape>(new Circle(7.0)));
    shapes.push_back(std::unique_ptr<Shape>(new Rectangle(12.0, 5.0)));
    shapes.push_back(std::unique_ptr<Shape>(new Triangle(3.0, 4.0, 5.0)));
    shapes.push_back(std::unique_ptr<Shape>(new Circle(10.0)));
    shapes.push_back(std::unique_ptr<Shape>(new Triangle(6.0, 8.0, 10.0)));

    double totalCombinedArea = 0.0;
    for (const auto& shapePtr : shapes) {
        // Runtime polymorphism: vptr looks up the overridden method in the vtable
        shapePtr->printReport();
        totalCombinedArea += shapePtr->area();
    }

    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "  Total Combined Area of All Shapes = "
              << std::fixed << std::setprecision(2) << totalCombinedArea << " sq. units\n\n";

    std::cout << "[2] Vtable / Vptr Size Verification:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "  sizeof(Shape) [with vptr + std::string] : " << sizeof(Shape) << " bytes\n";
    std::cout << "  sizeof(Circle)                          : " << sizeof(Circle) << " bytes\n";
    std::cout << "  sizeof(Rectangle)                       : " << sizeof(Rectangle) << " bytes\n";
    std::cout << "  sizeof(Triangle)                        : " << sizeof(Triangle) << " bytes\n\n";

    std::cout << "[3] Virtual Destructor Invocation via Base Class Pointers:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    shapes.clear(); // Triggers virtual destructor chain for each Shape*
    std::cout << "====================================================================================\n";

    return 0;
}
