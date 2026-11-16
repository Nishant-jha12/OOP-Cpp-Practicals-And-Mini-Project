#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    int securityPin;
    static int totalAccountsCreated;

protected:
    string holderName;
    double balance;

public:
    BankAccount(int accNo, string name, double initialBalance, int pin) {
        accountNumber = accNo;
        holderName = name;
        balance = initialBalance;
        securityPin = pin;
        totalAccountsCreated++;
    }

    int getAccountNumber() {
        return accountNumber;
    }

    void deposit(double amount);
    bool withdraw(double amount, int enteredPin);
    void displayAccountDetails();

    static int getTotalAccounts() {
        return totalAccountsCreated;
    }

    bool transferTo(BankAccount& receiver, double amount, int enteredPin);

    friend void auditAccount(BankAccount acc);
};

int BankAccount::totalAccountsCreated = 0;

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited Rs. " << amount << ". New Balance: Rs. " << balance << "\n";
    }
}

bool BankAccount::withdraw(double amount, int enteredPin) {
    if (enteredPin != securityPin) {
        cout << "Invalid PIN!\n";
