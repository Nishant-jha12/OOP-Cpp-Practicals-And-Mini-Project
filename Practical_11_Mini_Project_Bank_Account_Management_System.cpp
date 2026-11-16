#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BankAccount {
public:
    int accountNumber;
    string holderName;
    double balance;

    BankAccount(int accNo, string name, double initialBalance) {
        accountNumber = accNo;
        holderName = name;
        balance = initialBalance;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited Rs. " << amount << " successfully.\n";
            cout << "New Balance: Rs. " << balance << "\n";
        } else {
            cout << "Invalid deposit amount!\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawn Rs. " << amount << " successfully.\n";
            cout << "New Balance: Rs. " << balance << "\n";
        } else if (amount > balance) {
            cout << "Insufficient balance!\n";
        } else {
            cout << "Invalid withdrawal amount!\n";
        }
    }

    void display() {
        cout << "Acc No: " << accountNumber 
             << " | Name: " << holderName 
             << " | Balance: Rs. " << balance << "\n";
    }
};

int main() {
    vector<BankAccount> accounts;
    int nextAccountNumber = 1001;
    int choice;

    cout << "=================================\n";
    cout << "      ADYPU BANKING SYSTEM       \n";
    cout << "=================================\n";

    while (true) {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Open New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Display All Accounts\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            double initialDeposit;
            
            cout << "Enter Account Holder Name (single word): ";
            cin >> name;
            cout << "Enter Initial Deposit Amount: ";
            cin >> initialDeposit;

            if (initialDeposit >= 0) {
                BankAccount newAccount(nextAccountNumber, name, initialDeposit);
                accounts.push_back(newAccount);
                
                cout << "Account created successfully! Your Account Number is " << nextAccountNumber << "\n";
                nextAccountNumber++;
            } else {
                cout << "Initial deposit cannot be negative!\n";
            }
        } 
        else if (choice == 2) {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            
            bool found = false;
            for (int i = 0; i < accounts.size(); i++) {
                if (accounts[i].accountNumber == accNo) {
                    cout << "Enter Deposit Amount: ";
                    cin >> amount;
                    accounts[i].deposit(amount);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found!\n";
        }
        else if (choice == 3) {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            
            bool found = false;
            for (int i = 0; i < accounts.size(); i++) {
                if (accounts[i].accountNumber == accNo) {
                    cout << "Enter Withdrawal Amount: ";
                    cin >> amount;
                    accounts[i].withdraw(amount);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found!\n";
        }
        else if (choice == 4) {
            if (accounts.size() == 0) {
                cout << "No accounts to display.\n";
            } else {
                cout << "\n--- ALL BANK ACCOUNTS ---\n";
                for (int i = 0; i < accounts.size(); i++) {
                    accounts[i].display();
                }
            }
        }
        else if (choice == 5) {
            cout << "Thank you for using ADYPU Bank. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
        }
    }

    return 0;
}
