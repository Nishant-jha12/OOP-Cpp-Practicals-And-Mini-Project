#include <iostream>
#include <string>

using namespace std;

class BankAccount {
public:
    int accNo;
    string name;
    double balance;

    BankAccount() {
        accNo = 0;
        name = "";
        balance = 0.0;
    }

    BankAccount(int a, string n, double b) {
        accNo = a;
        name = n;
        balance = b;
    }

    void deposit(double amount) {
        balance = balance + amount;
        cout << "New Balance: " << balance << "\n";
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance = balance - amount;
            cout << "New Balance: " << balance << "\n";
        } else {
            cout << "Insufficient balance!\n";
        }
    }

    void display() {
        cout << "Acc: " << accNo << " | Name: " << name << " | Bal: " << balance << "\n";
    }
};

int main() {
    BankAccount accounts[100];
    int count = 0;
    int nextAccNo = 1001;
    int choice;

    while (true) {
        cout << "\n1. Open Account  2. Deposit  3. Withdraw  4. Display  5. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            double amount;
            cout << "Name: ";
            cin >> name;
            cout << "Initial Deposit: ";
            cin >> amount;
            
            accounts[count] = BankAccount(nextAccNo, name, amount);
            count++;
            cout << "Account created! Acc No: " << nextAccNo << "\n";
            nextAccNo++;
        } 
        else if (choice == 2) {
            int acc;
            double amt;
            cout << "Account No: ";
            cin >> acc;
            cout << "Amount to Deposit: ";
            cin >> amt;

            for (int i = 0; i < count; i++) {
                if (accounts[i].accNo == acc) {
                    accounts[i].deposit(amt);
                }
            }
        }
        else if (choice == 3) {
            int acc;
            double amt;
            cout << "Account No: ";
            cin >> acc;
            cout << "Amount to Withdraw: ";
            cin >> amt;

            for (int i = 0; i < count; i++) {
                if (accounts[i].accNo == acc) {
                    accounts[i].withdraw(amt);
                }
            }
        }
        else if (choice == 4) {
            for (int i = 0; i < count; i++) {
                accounts[i].display();
            }
        }
        else if (choice == 5) {
            break;
        }
    }
    return 0;
}
