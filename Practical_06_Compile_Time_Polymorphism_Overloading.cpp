#include <iostream>

using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0) {
        real = r;
        imag = i;
    }

    Complex add(double realPart) {
        return Complex(real + realPart, imag);
    }

    Complex add(double realPart, double imagPart) {
        return Complex(real + realPart, imag + imagPart);
    }

    Complex add(Complex other) {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator+(Complex rhs) {
        return Complex(real + rhs.real, imag + rhs.imag);
    }

    Complex operator
