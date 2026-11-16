#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    int accountNo;
    double balance;
    double dailyLimit;
    bool isFrozen;

public:
    BankAccount(int accNo, double initBal, double limit) {
        accountNo = accNo;
        balance = initBal;
        dailyLimit = limit;
        isFrozen = false;
    }

    void setFrozen(bool frozen) {
        isFrozen = frozen;
    }

    void deposit(double amount) {
        if (isFrozen) {
            cout << "Error: Account is frozen!\n";
        } else if (amount <= 0) {
            cout << "Error: Deposit amount must be greater than zero!\n";
        } else {
            balance += amount;
            cout << "Deposited Rs. " << amount << ". New Balance: Rs. " << balance << "\n";
        }
    }

    void withdraw(double amount) {
        if (isFrozen) {
            cout << "Error: Account is frozen!\n";
        } else if (amount <= 0) {
            cout << "Error: Withdrawal amount must be greater than zero!\n";
        } else if (amount > dailyLimit) {
            cout << "Error: Amount exceeds daily limit of Rs. " << dailyLimit << "!\n";
        } else if (amount > balance) {
            cout << "Error: Insufficient funds! Available balance: Rs. " << balance << "\n";
        } else {
            balance -= amount;
            cout << "Withdrew Rs. " << amount << ". Remaining Balance: Rs. " << balance << "\n";
        }
    }

    void printBalance() {
        cout << "Current Balance: Rs. " << balance << "\n\n";
    }
};

int main() {
    cout << "--- Initial Account Setup ---\n";
    BankAccount acc(5001, 30000.0, 25000.0);
    acc.printBalance();

    cout << "--- Scenario 1: Valid Transactions ---\n";
    acc.deposit(5000.0);
    acc.withdraw(4000.0);
    acc.printBalance();

    cout << "--- Scenario 2: Negative Deposit ---\n";
    acc.deposit(-1500.0);
    acc.printBalance();

    cout << "--- Scenario 3: Exceeding Daily Limit ---\n";
    acc.
