// ============================================================================
// Practical 2: Class Hierarchy, Access Specifiers & Member Functions
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 1 & Unit 2 - Classes, Objects, Access Specifiers,
//              Static Members, Friend Functions, Array of Objects
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// PART A: BankAccount Class Hierarchy
// Demonstrates private, protected, public access specifiers, member functions
// defined inside and outside class (::), static members, and friend function.
// ============================================================================
class BankAccount {
private:
    // Private: Accessible ONLY within BankAccount class (Encapsulation of PIN/ID)
    int accountNumber;
    int securityPin;
    static int totalAccountsCreated; // Static data member

protected:
    // Protected: Accessible within BankAccount AND derived classes
    std::string holderName;
    double balance;

public:
    // Public: Accessible from anywhere
    BankAccount(int accNo, const std::string& name, double initialBalance, int pin)
        : accountNumber(accNo), securityPin(pin), holderName(name), balance(initialBalance) {
        totalAccountsCreated++;
    }

    virtual ~BankAccount() {}

    // Inline public getter for private member
    int getAccountNumber() const { return accountNumber; }

    // Member function declared inside class and defined outside using ::
    void deposit(double amount);
    bool withdraw(double amount, int enteredPin);
    virtual void displayAccountDetails() const;

    // Static member function
    static int getTotalAccounts() {
        return totalAccountsCreated;
    }

    // Object as function argument (Fund Transfer)
    bool transferTo(BankAccount& receiver, double amount, int enteredPin);

    // Friend function declaration (can access private & protected members)
    friend void auditAccount(const BankAccount& acc);
};

// Initialize static data member outside the class
int BankAccount::totalAccountsCreated = 0;

// Member functions defined outside the class using Scope Resolution Operator (::)
void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "  [Deposit] Rs. " << std::fixed << std::setprecision(2) << amount
                  << " credited to Acc #" << accountNumber
                  << " (" << holderName << "). New Balance: Rs. " << balance << "\n";
    }
}

bool BankAccount::withdraw(double amount, int enteredPin) {
    if (enteredPin != securityPin) {
        std::cout << "  [Error] Invalid PIN for Acc #" << accountNumber << "!\n";
        return false;
    }
    if (amount <= 0 || amount > balance) {
        std::cout << "  [Error] Insufficient balance in Acc #" << accountNumber << "!\n";
        return false;
    }
    balance -= amount;
    std::cout << "  [Withdraw] Rs. " << amount << " debited from Acc #" << accountNumber
              << ". Remaining Balance: Rs. " << balance << "\n";
    return true;
}

void BankAccount::displayAccountDetails() const {
    std::cout << "  Acc #" << accountNumber << " | Holder: " << std::setw(15) << std::left << holderName
              << " | Balance: Rs. " << std::fixed << std::setprecision(2) << balance << "\n";
}

bool BankAccount::transferTo(BankAccount& receiver, double amount, int enteredPin) {
    std::cout << "  [Transfer] Initiating Rs. " << amount << " from " << holderName
              << " to " << receiver.holderName << "...\n";
    if (this->withdraw(amount, enteredPin)) {
        receiver.deposit(amount);
        return true;
    }
    return false;
}

// Friend function accessing private (accountNumber, securityPin) & protected (holderName, balance)
void auditAccount(const BankAccount& acc) {
    std::cout << "  [Auditor - Friend Function] Acc #" << acc.accountNumber
              << " | Holder: " << acc.holderName
              << " | Balance: Rs. " << acc.balance
              << " | Masked PIN: **" << (acc.securityPin % 100) << "\n";
}

// Derived Class 1: SavingsAccount (Public Inheritance)
class SavingsAccount : public BankAccount {
private:
    double interestRate; // in %

public:
    SavingsAccount(int accNo, const std::string& name, double initialBalance, int pin, double rate)
        : BankAccount(accNo, name, initialBalance, pin), interestRate(rate) {}

    // Accesses 'protected' members (balance, holderName) directly
    void addAnnualInterest() {
        double interest = balance * (interestRate / 100.0);
        balance += interest; // Allowed because 'balance' is protected in BankAccount
        std::cout << "  [Interest] Added " << interestRate << "% (Rs. " << interest
                  << ") to " << holderName << "'s Savings Account. New Balance: Rs. " << balance << "\n";
    }

    void displayAccountDetails() const override {
        std::cout << "  [Savings] Acc #" << getAccountNumber()
                  << " | Holder: " << std::setw(15) << std::left << holderName
                  << " | Balance: Rs. " << std::setw(10) << balance
                  << " | Interest Rate: " << interestRate << "%\n";
    }
};

// Derived Class 2: CurrentAccount (Public Inheritance)
class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNo, const std::string& name, double initialBalance, int pin, double overdraft)
        : BankAccount(accNo, name, initialBalance, pin), overdraftLimit(overdraft) {}

    void displayAccountDetails() const override {
        std::cout << "  [Current] Acc #" << getAccountNumber()
                  << " | Holder: " << std::setw(15) << std::left << holderName
                  << " | Balance: Rs. " << std::setw(10) << balance
                  << " | Overdraft Limit: Rs. " << overdraftLimit << "\n";
    }
};

// ============================================================================
// PART B: Vehicle Class Hierarchy (Demonstrating Array of Objects)
// ============================================================================
class Vehicle {
private:
    std::string chassisNumber;

protected:
    std::string brand;
    int maxSpeedKmh;

public:
    Vehicle(const std::string& chassis, const std::string& b, int speed)
        : chassisNumber(chassis), brand(b), maxSpeedKmh(speed) {}

    void showVehicleSpec() const {
        std::cout << "  Brand: " << std::setw(12) << std::left << brand
                  << " | Max Speed: " << std::setw(4) << maxSpeedKmh << " km/h"
                  << " | Chassis (Private): " << chassisNumber << "\n";
    }
};

class ElectricVehicle : public Vehicle {
private:
    int batteryCapacityKWh;

public:
    ElectricVehicle(const std::string& chassis, const std::string& b, int speed, int battery)
        : Vehicle(chassis, b, speed), batteryCapacityKWh(battery) {}

    void showEVDetails() const {
        std::cout << "  [EV] Brand: " << std::setw(10) << std::left << brand
                  << " | Speed: " << maxSpeedKmh << " km/h"
                  << " | Battery: " << batteryCapacityKWh << " kWh\n";
    }
};

int main() {
    std::cout << "========================================================================\n";
    std::cout << " PRACTICAL 2: CLASS HIERARCHY, ACCESS SPECIFIERS & MEMBER FUNCTIONS\n";
    std::cout << "========================================================================\n\n";

    // 1. Creating Derived Objects in BankAccount Hierarchy
    std::cout << "[1] Creating BankAccount Hierarchy Objects (Savings & Current):\n";
    std::cout << "------------------------------------------------------------------------\n";
    SavingsAccount savAcc(1001, "Nishant Jha", 25000.00, 4321, 6.5);
    CurrentAccount curAcc(1002, "Riya Verma", 60000.00, 9876, 15000.00);

    savAcc.displayAccountDetails();
    curAcc.displayAccountDetails();
    std::cout << "  Total Accounts Created (Static Function): " << BankAccount::getTotalAccounts() << "\n\n";

    // 2. Demonstrating Member Functions & Protected Member Access
    std::cout << "[2] Member Functions & Protected Member Access in Derived Class:\n";
    std::cout << "------------------------------------------------------------------------\n";
    savAcc.deposit(5000.00);
    savAcc.addAnnualInterest();
    savAcc.withdraw(2000.00, 4321);
    savAcc.withdraw(1000.00, 1111); // Wrong PIN test
    std::cout << "\n";

    // 3. Demonstrating Object as Function Argument & Friend Function
    std::cout << "[3] Object as Function Argument (Transfer) & Friend Function:\n";
    std::cout << "------------------------------------------------------------------------\n";
    curAcc.transferTo(savAcc, 8000.00, 9876);
    auditAccount(savAcc);
    auditAccount(curAcc);
    std::cout << "\n";

    // 4. Demonstrating Vehicle Hierarchy & Array of Objects
    std::cout << "[4] Vehicle Hierarchy & Array of Objects:\n";
    std::cout << "------------------------------------------------------------------------\n";
    ElectricVehicle fleet[3] = {
        ElectricVehicle("CH-EV-901", "Tata Nexon", 150, 40),
        ElectricVehicle("CH-EV-902", "MG ZS EV",   175, 50),
        ElectricVehicle("CH-EV-903", "Hyundai I5", 185, 72)
    };

    for (const auto& ev : fleet) {
        ev.showEVDetails();
    }
    std::cout << "========================================================================\n";

    return 0;
}
