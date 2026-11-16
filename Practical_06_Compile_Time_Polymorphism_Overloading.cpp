// ============================================================================
// Practical 6: Compile-Time Polymorphism (Function & Operator Overloading)
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 3 - Compile-Time Polymorphism (Function Overloading &
//              Operator Overloading for Complex Number and Matrix Classes)
// ============================================================================

#include <iostream>
#include <iomanip>
#include <cmath>

// ============================================================================
// PART A: Complex Number Class
// Demonstrates Function Overloading & Operator Overloading (+, -, *, ==, <<)
// ============================================================================
class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // --- Function Overloading (Compile-Time Polymorphism) ---
    // 1. Add a real scalar to Complex number
    Complex add(double realPart) const {
        return Complex(real + realPart, imag);
    }

    // 2. Add separate real and imaginary values
    Complex add(double realPart, double imagPart) const {
        return Complex(real + realPart, imag + imagPart);
    }

    // 3. Add another Complex object
    Complex add(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // --- Operator Overloading (Compile-Time Polymorphism) ---
    // Binary + operator
    Complex operator+(const Complex& rhs) const {
        return Complex(real + rhs.real, imag + rhs.imag);
    }

    // Binary - operator
    Complex operator-(const Complex& rhs) const {
        return Complex(real - rhs.real, imag - rhs.imag);
    }

    // Binary * operator: (a+bi)(c+di) = (ac - bd) + (ad + bc)i
    Complex operator*(const Complex& rhs) const {
        return Complex(
            (real * rhs.real) - (imag * rhs.imag),
            (real * rhs.imag) + (imag * rhs.real)
        );
    }

    // Unary ~ operator (Complex Conjugate)
    Complex operator~() const {
        return Complex(real, -imag);
    }

    // Equality == operator
    bool operator==(const Complex& rhs) const {
        return (std::fabs(real - rhs.real) < 1e-9) &&
               (std::fabs(imag - rhs.imag) < 1e-9);
    }

    // Friend stream insertion operator <<
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(2) << c.real
           << (c.imag >= 0 ? " + " : " - ") << std::fabs(c.imag) << "i";
        return os;
    }
};

// ============================================================================
// PART B: 2x2 Matrix Class
// Demonstrates Function Overloading & Operator Overloading (+, -, *, <<)
// ============================================================================
class Matrix2x2 {
private:
    int mat[2][2];

public:
    Matrix2x2() {
        mat[0][0] = mat[0][1] = mat[1][0] = mat[1][1] = 0;
    }

    Matrix2x2(int a00, int a01, int a10, int a11) {
        mat[0][0] = a00; mat[0][1] = a01;
        mat[1][0] = a10; mat[1][1] = a11;
    }

    // --- Function Overloading ---
    // 1. Fill entire matrix with a single value
    void setValues(int uniformValue) {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                mat[i][j] = uniformValue;
    }

    // 2. Set diagonal elements only
    void setValues(int d1, int d2) {
        mat[0][0] = d1; mat[0][1] = 0;
        mat[1][0] = 0;  mat[1][1] = d2;
    }

    // 3. Set all 4 elements of 2x2 matrix
    void setValues(int a00, int a01, int a10, int a11) {
        mat[0][0] = a00; mat[0][1] = a01;
        mat[1][0] = a10; mat[1][1] = a11;
    }

    // --- Operator Overloading ---
    Matrix2x2 operator+(const Matrix2x2& rhs) const {
        return Matrix2x2(
            mat[0][0] + rhs.mat[0][0], mat[0][1] + rhs.mat[0][1],
            mat[1][0] + rhs.mat[1][0], mat[1][1] + rhs.mat[1][1]
        );
    }

    Matrix2x2 operator-(const Matrix2x2& rhs) const {
        return Matrix2x2(
            mat[0][0] - rhs.mat[0][0], mat[0][1] - rhs.mat[0][1],
            mat[1][0] - rhs.mat[1][0], mat[1][1] - rhs.mat[1][1]
        );
    }

    Matrix2x2 operator*(const Matrix2x2& rhs) const {
        Matrix2x2 result;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                result.mat[i][j] = 0;
                for (int k = 0; k < 2; ++k) {
                    result.mat[i][j] += mat[i][k] * rhs.mat[k][j];
                }
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        os << "    [ " << std::setw(4) << m.mat[0][0] << "  " << std::setw(4) << m.mat[0][1] << " ]\n"
           << "    [ " << std::setw(4) << m.mat[1][0] << "  " << std::setw(4) << m.mat[1][1] << " ]";
        return os;
    }
};

int main() {
    std::cout << "========================================================================\n";
    std::cout << " PRACTICAL 6: COMPILE-TIME POLYMORPHISM (FUNCTION & OPERATOR OVERLOADING)\n";
    std::cout << "========================================================================\n\n";

    // ------------------------------------------------------------------------
    // PART A: Complex Number Class
    // ------------------------------------------------------------------------
    Complex c1(4.0, 3.0);
    Complex c2(2.0, -5.0);

    std::cout << "[1] Complex Number - Function Overloading (add):\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << "  c1                         = " << c1 << "\n";
    std::cout << "  c1.add(10.0) [scalar]      = " << c1.add(10.0) << "\n";
    std::cout << "  c1.add(1.5, 2.5) [2 args]  = " << c1.add(1.5, 2.5) << "\n";
    std::cout << "  c1.add(c2) [Complex obj]   = " << c1.add(c2) << "\n\n";

    std::cout << "[2] Complex Number - Operator Overloading (+, -, *, ~, ==, <<):\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << "  c1                         = " << c1 << "\n";
    std::cout << "  c2                         = " << c2 << "\n";
    std::cout << "  c1 + c2                    = " << (c1 + c2) << "\n";
    std::cout << "  c1 - c2                    = " << (c1 - c2) << "\n";
    std::cout << "  c1 * c2                    = " << (c1 * c2) << "\n";
    std::cout << "  ~c1 (Conjugate)            = " << (~c1) << "\n";
    std::cout << "  c1 == c2                   = " << (c1 == c2 ? "true" : "false") << "\n\n";

    // ------------------------------------------------------------------------
    // PART B: Matrix Class
    // ------------------------------------------------------------------------
    Matrix2x2 A, B;
    std::cout << "[3] Matrix2x2 - Function Overloading (setValues) & Operator Overloading:\n";
    std::cout << "------------------------------------------------------------------------\n";
    A.setValues(5, 10);         // Overloaded 2-argument diagonal initializer
    B.setValues(1, 2, 3, 4);    // Overloaded 4-argument full matrix initializer

    std::cout << "  Matrix A (via setValues(5, 10) diagonal):\n" << A << "\n\n";
    std::cout << "  Matrix B (via setValues(1, 2, 3, 4)):\n" << B << "\n\n";
    std::cout << "  Matrix A + B:\n" << (A + B) << "\n\n";
    std::cout << "  Matrix A - B:\n" << (A - B) << "\n\n";
    std::cout << "  Matrix A * B:\n" << (A * B) << "\n";
    std::cout << "========================================================================\n";

    return 0;
}
