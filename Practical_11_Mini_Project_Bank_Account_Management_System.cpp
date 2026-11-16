// ============================================================================
// Practical 11 (MINI PROJECT): Bank Account Management System
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Institution: Ajeenkya DY Patil University (ADYPU)
// Syllabus   : Strictly built from Units 1 to 5 of the ADYPU C++ Syllabus:
//   - Unit 1 : OOP Pillars, const/constexpr, auto, range-based for, nullptr, ::, I/O
//   - Unit 2 : Classes, Access Specifiers, Constructors & Rule of 5, Static
//              Members, Friend Functions, RAII & Smart Pointers (shared_ptr, unique_ptr)
//   - Unit 3 : Single, Hierarchical & Multilevel Inheritance, Abstract Base Class,
//              Virtual Functions, Function & Operator Overloading,
//              Design Patterns (Singleton, Factory Method, Observer)
//   - Unit 4 : Function & Class Templates, STL Containers (vector, map,
//              unordered_map, queue), STL Algorithms (sort, find_if, transform,
//              accumulate, for_each) & Lambda Expressions
//   - Unit 5 : Custom Exception Hierarchy (try-catch-throw, noexcept) &
//              CSV File I/O (fstream, seekg, seekp, tellg, tellp)
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <memory>
#include <algorithm>
#include <numeric>
#include <exception>
#include <fstream>
#include <sstream>
#include <utility>

// Global Bank Name Constant (Demonstrating Scope Resolution :: in Unit 1)
const std::string BANK_TITLE = "ADYPU NATIONAL BANKING SYSTEM";

// Compile-time constant using constexpr (Unit 1)
constexpr double MINIMUM_SAVINGS_BALANCE = 1000.0;
constexpr double DEFAULT_SAVINGS_INTEREST_RATE = 6.0;
constexpr double DEFAULT_FD_BONUS_INTEREST_RATE = 2.5;

// ============================================================================
// UNIT 5: CUSTOM EXCEPTION HIERARCHY (Inheriting from std::exception)
// ============================================================================
class BankException : public std::exception {
protected:
    std::string message;

public:
    explicit BankException(const std::string& msg) : message(msg) {}
    virtual ~BankException() noexcept {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidTransactionAmountException : public BankException {
public:
    explicit InvalidTransactionAmountException(double amount)
        : BankException("[InvalidTransactionAmountException] Amount Rs. " +
                        std::to_string(amount) + " must be greater than zero!") {}
};

class InsufficientBalanceException : public BankException {
public:
    InsufficientBalanceException(int accNo, double balance, double requested)
        : BankException("[InsufficientBalanceException] Acc #" + std::to_string(accNo) +
                        ": Requested Rs. " + std::to_string(requested) +
                        " exceeds available funds (Rs. " + std::to_string(balance) + ")!") {}
};

class MinimumBalanceViolationException : public BankException {
public:
    MinimumBalanceViolationException(int accNo, double minRequired)
        : BankException("[MinimumBalanceViolationException] Acc #" + std::to_string(accNo) +
                        ": Withdrawal would breach minimum required balance of Rs. " +
                        std::to_string(minRequired) + "!") {}
};

class AccountNotFoundException : public BankException {
public:
    explicit AccountNotFoundException(int accNo)
        : BankException("[AccountNotFoundException] Account #" + std::to_string(accNo) +
                        " does not exist in the bank registry!") {}
};

// ============================================================================
// UNIT 2: RULE OF 5 DEMONSTRATION IN TransactionRecord CLASS
// ============================================================================
class TransactionRecord {
private:
    int transactionId;
    int accountNo;
    std::string type;
    double amount;
    double balanceAfter;
    char* remarks; // Dynamically allocated C-string to demonstrate Rule of 5

    void copyRemarksFrom(const char* src) {
        if (src != nullptr) {
            size_t len = 0;
            while (src[len] != '\0') ++len;
            remarks = new char[len + 1];
            for (size_t i = 0; i <= len; ++i) remarks[i] = src[i];
        } else {
            remarks = new char[1];
            remarks[0] = '\0';
        }
    }

public:
    // 1. Default Constructor (delegates to Parameterized Constructor)
    TransactionRecord() : TransactionRecord(0, 0, "INIT", 0.0, 0.0, "Default") {}

    // 2. Parameterized Constructor
    TransactionRecord(int txId, int acc, const std::string& txType,
                      double amt, double balAfter, const std::string& note)
        : transactionId(txId), accountNo(acc), type(txType),
          amount(amt), balanceAfter(balAfter), remarks(nullptr) {
        copyRemarksFrom(note.c_str());
    }

    // 3. Copy Constructor (Rule of 5 #1)
    TransactionRecord(const TransactionRecord& other)
        : transactionId(other.transactionId), accountNo(other.accountNo),
          type(other.type), amount(other.amount),
          balanceAfter(other.balanceAfter), remarks(nullptr) {
        copyRemarksFrom(other.remarks);
    }

    // 4. Copy Assignment Operator (Rule of 5 #2)
    TransactionRecord& operator=(const TransactionRecord& other) {
        if (this != &other) {
            delete[] remarks;
            transactionId = other.transactionId;
            accountNo = other.accountNo;
            type = other.type;
            amount = other.amount;
            balanceAfter = other.balanceAfter;
            copyRemarksFrom(other.remarks);
        }
        return *this;
    }

    // 5. Move Constructor (Rule of 5 #3)
    TransactionRecord(TransactionRecord&& other) noexcept
        : transactionId(other.transactionId), accountNo(other.accountNo),
          type(std::move(other.type)), amount(other.amount),
          balanceAfter(other.balanceAfter), remarks(other.remarks) {
        other.remarks = nullptr;
    }

    // 6. Move Assignment Operator (Rule of 5 #4)
    TransactionRecord& operator=(TransactionRecord&& other) noexcept {
        if (this != &other) {
            delete[] remarks;
            transactionId = other.transactionId;
            accountNo = other.accountNo;
            type = std::move(other.type);
            amount = other.amount;
            balanceAfter = other.balanceAfter;
            remarks = other.remarks;
            other.remarks = nullptr;
        }
        return *this;
    }

    // 7. Destructor (Rule of 5 #5)
    ~TransactionRecord() {
        delete[] remarks;
        remarks = nullptr;
    }

    int getTransactionId() const noexcept { return transactionId; }
    int getAccountNo() const noexcept { return accountNo; }
    const std::string& getType() const noexcept { return type; }
    double getAmount() const noexcept { return amount; }
    double getBalanceAfter() const noexcept { return balanceAfter; }
    std::string getRemarks() const { return remarks ? std::string(remarks) : ""; }
};

// ============================================================================
// UNIT 4: GENERIC CLASS TEMPLATE & FUNCTION TEMPLATE
// ============================================================================
template <typename T, size_t Capacity = 25>
class GenericAuditStack {
private:
    T items[Capacity];
    int topIdx;

public:
    GenericAuditStack() : topIdx(-1) {}

    bool isEmpty() const { return topIdx == -1; }
    size_t size() const { return static_cast<size_t>(topIdx + 1); }

    void push(const T& item) {
        if (topIdx + 1 < static_cast<int>(Capacity)) {
            items[++topIdx] = item;
        }
    }

    const T& peek() const {
        return items[topIdx];
    }
};

template <typename T>
void printMetricRow(const std::string& metricLabel, const T& metricValue) {
    std::cout << "  " << std::setw(36) << std::left << metricLabel
              << " : " << metricValue << "\n";
}

// ============================================================================
// UNIT 3: OBSERVER DESIGN PATTERN FOR TRANSACTION ALERTS
// ============================================================================
class ITransactionObserver {
public:
    virtual ~ITransactionObserver() {}
    virtual void onTransactionCompleted(const TransactionRecord& tx) = 0;
};

class SMSNotificationObserver : public ITransactionObserver {
private:
    std::queue<std::string> notificationQueue; // STL std::queue

public:
    void onTransactionCompleted(const TransactionRecord& tx) override {
        std::ostringstream oss;
        oss << "[SMS Alert] Acc #" << tx.getAccountNo() << " | " << tx.getType()
            << " of Rs. " << std::fixed << std::setprecision(2) << tx.getAmount()
            << " | Avl Bal: Rs. " << tx.getBalanceAfter()
            << " (" << tx.getRemarks() << ")";
        notificationQueue.push(oss.str());
    }

    void flushNotifications() {
        while (!notificationQueue.empty()) {
            std::cout << "  " << notificationQueue.front() << "\n";
            notificationQueue.pop();
        }
    }
};

// ============================================================================
// UNIT 2 & UNIT 3: ABSTRACT BASE CLASS BankAccount & INHERITANCE HIERARCHY
// Hierarchy:
//   BankAccount (Abstract Base Class)
//     ├── SavingsAccount (Single/Hierarchical Inheritance)
//     │     └── FixedDepositAccount (Multilevel Inheritance)
//     └── CurrentAccount (Single/Hierarchical Inheritance)
// ============================================================================
class BankAccount {
private:
    int accountNumber;
    static int totalActiveAccounts;

protected:
    std::string holderName;
    double balance;

public:
    BankAccount(int accNo, const std::string& name, double initBal)
        : accountNumber(accNo), holderName(name), balance(initBal) {
        totalActiveAccounts++;
    }

    virtual ~BankAccount() {
        totalActiveAccounts--;
    }

    int getAccountNumber() const noexcept { return accountNumber; }
    const std::string& getHolderName() const noexcept { return holderName; }
    double getBalance() const noexcept { return balance; }

    static int getTotalActiveAccounts() noexcept {
        return totalActiveAccounts;
    }

    // --- Compile-Time Polymorphism: Function Overloading for deposit() ---
    virtual void deposit(double amount) {
        deposit(amount, "Cash Deposit");
    }

    virtual void deposit(double amount, const std::string& note) {
        if (amount <= 0.0) {
            throw InvalidTransactionAmountException(amount);
        }
        balance += amount;
        (void)note;
    }

    // --- Pure Virtual Functions (Runtime Polymorphism) ---
    virtual void withdraw(double amount) = 0;
    virtual double calculateInterest() const = 0;
    virtual std::string getAccountType() const = 0;

    virtual void applyAnnualInterest() {
        double interest = calculateInterest();
        if (interest > 0.0) {
            balance += interest;
        }
    }

    // --- Compile-Time Polymorphism: Operator Overloading ---
    // Overloaded += operator for quick deposit
    BankAccount& operator+=(double amount) {
        this->deposit(amount, "Operator+= Deposit");
        return *this;
    }

    // Overloaded == operator
    bool operator==(const BankAccount& other) const noexcept {
        return this->accountNumber == other.accountNumber;
    }

    // Friend function for stream insertion operator<<
    friend std::ostream& operator<<(std::ostream& os, const BankAccount& acc) {
        os << "Acc #" << std::setw(5) << std::left << acc.accountNumber
           << " | Type: " << std::setw(14) << std::left << acc.getAccountType()
           << " | Holder: " << std::setw(16) << std::left << acc.holderName
           << " | Balance: Rs. " << std::setw(10) << std::right
           << std::fixed << std::setprecision(2) << acc.balance
           << " | Est. Interest: Rs. " << std::setw(8) << acc.calculateInterest();
        return os;
    }

    std::string toCSVRow() const {
        std::ostringstream oss;
        oss << accountNumber << ","
            << std::setw(14) << std::left << getAccountType() << ","
            << std::setw(16) << std::left << holderName << ","
            << std::fixed << std::setprecision(2) << std::setw(10) << balance;
        return oss.str();
    }
};

int BankAccount::totalActiveAccounts = 0;

// Derived Class 1: SavingsAccount (Single Inheritance from BankAccount)
class SavingsAccount : public BankAccount {
protected:
    double interestRatePercent;

public:
    SavingsAccount(int accNo, const std::string& name, double initBal,
                   double rate = DEFAULT_SAVINGS_INTEREST_RATE)
        : BankAccount(accNo, name, initBal), interestRatePercent(rate) {}

    void withdraw(double amount) override {
        if (amount <= 0.0) {
            throw InvalidTransactionAmountException(amount);
        }
        if (amount > balance) {
            throw InsufficientBalanceException(getAccountNumber(), balance, amount);
        }
        if ((balance - amount) < MINIMUM_SAVINGS_BALANCE) {
            throw MinimumBalanceViolationException(getAccountNumber(), MINIMUM_SAVINGS_BALANCE);
        }
        balance -= amount;
    }

    double calculateInterest() const override {
        return balance * (interestRatePercent / 100.0);
    }

    std::string getAccountType() const override {
        return "SAVINGS";
    }
};

// Derived Class 2: CurrentAccount (Single Inheritance from BankAccount)
class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNo, const std::string& name, double initBal,
                   double overdraft = 20000.0)
        : BankAccount(accNo, name, initBal), overdraftLimit(overdraft) {}

    void withdraw(double amount) override {
        if (amount <= 0.0) {
            throw InvalidTransactionAmountException(amount);
        }
        if (amount > (balance + overdraftLimit)) {
            throw InsufficientBalanceException(getAccountNumber(), balance + overdraftLimit, amount);
        }
        balance -= amount;
    }

    double calculateInterest() const override {
        return 0.0; // Current accounts do not earn interest
    }

    std::string getAccountType() const override {
        return "CURRENT";
    }
};

// Derived Class 3: FixedDepositAccount (Multilevel Inheritance:
// BankAccount -> SavingsAccount -> FixedDepositAccount)
class FixedDepositAccount : public SavingsAccount {
private:
    int tenureYears;

public:
    FixedDepositAccount(int accNo, const std::string& name, double initBal, int years = 3)
        : SavingsAccount(accNo, name, initBal,
                         DEFAULT_SAVINGS_INTEREST_RATE + DEFAULT_FD_BONUS_INTEREST_RATE),
          tenureYears(years) {}

    double calculateInterest() const override {
        // Compound-boosted annual return for Fixed Deposit
        return balance * (interestRatePercent / 100.0) * (1.0 + 0.05 * tenureYears);
    }

    std::string getAccountType() const override {
        return "FIXED-DEPOSIT";
    }
};

// ============================================================================
// UNIT 3: FACTORY METHOD DESIGN PATTERN FOR ACCOUNT CREATION
// ============================================================================
enum class AccountCategory {
    SAVINGS = 1,
    CURRENT = 2,
    FIXED_DEPOSIT = 3
};

class AccountFactory {
public:
    static std::shared_ptr<BankAccount> createAccount(
        AccountCategory category, int accNo, const std::string& holder, double initialDeposit) {
        if (initialDeposit <= 0.0) {
            throw InvalidTransactionAmountException(initialDeposit);
        }
        switch (category) {
            case AccountCategory::SAVINGS:
                return std::make_shared<SavingsAccount>(accNo, holder, initialDeposit);
            case AccountCategory::CURRENT:
                return std::make_shared<CurrentAccount>(accNo, holder, initialDeposit);
            case AccountCategory::FIXED_DEPOSIT:
                return std::make_shared<FixedDepositAccount>(accNo, holder, initialDeposit, 3);
            default:
                return std::make_shared<SavingsAccount>(accNo, holder, initialDeposit);
        }
    }
};

// ============================================================================
// UNIT 3, 4 & 5: SINGLETON DESIGN PATTERN - BankSystem
// Manages STL Containers, Observer Notifications, Transactions & CSV File I/O
// ============================================================================
class BankSystem {
private:
    std::map<int, std::shared_ptr<BankAccount>> accountDirectory; // Ordered O(log N)
    std::unordered_map<std::string, int> holderNameIndex;         // Hash Table O(1)
    std::vector<TransactionRecord> transactionLedger;             // Dynamic Array
    GenericAuditStack<TransactionRecord, 50> auditStack;          // Template Stack
    std::unique_ptr<SMSNotificationObserver> smsObserver;         // RAII Unique Ptr
    int nextAccountNo;
    int nextTransactionId;

    // Private Constructor for Singleton Pattern
    BankSystem()
        : smsObserver(new SMSNotificationObserver()),
          nextAccountNo(1001),
          nextTransactionId(5001) {}

    void recordTransaction(int accNo, const std::string& type, double amount,
                           double balanceAfter, const std::string& note) {
        TransactionRecord tx(nextTransactionId++, accNo, type, amount, balanceAfter, note);
        transactionLedger.push_back(tx);
        auditStack.push(tx);
        if (smsObserver != nullptr) {
            smsObserver->onTransactionCompleted(tx);
        }
    }

public:
    // Delete Copy Constructor & Assignment Operator (Singleton Guarantee)
    BankSystem(const BankSystem&) = delete;
    BankSystem& operator=(const BankSystem&) = delete;

    static BankSystem& getInstance() {
        static BankSystem instance;
        return instance;
    }

    int openAccount(AccountCategory category, const std::string& holderName, double initialDeposit) {
        int accNo = nextAccountNo++;
        auto account = AccountFactory::createAccount(category, accNo, holderName, initialDeposit);
        accountDirectory[accNo] = account;
        holderNameIndex[holderName] = accNo;
        recordTransaction(accNo, "OPEN", initialDeposit, account->getBalance(),
                          "Opened " + account->getAccountType());
        return accNo;
    }

    std::shared_ptr<BankAccount> findAccount(int accNo) const {
        auto it = accountDirectory.find(accNo);
        if (it == accountDirectory.end()) {
            throw AccountNotFoundException(accNo);
        }
        return it->second;
    }

    void depositToAccount(int accNo, double amount, const std::string& note = "Branch Deposit") {
        auto acc = findAccount(accNo);
        acc->deposit(amount, note); // Function overloading
        recordTransaction(accNo, "DEPOSIT", amount, acc->getBalance(), note);
    }

    void withdrawFromAccount(int accNo, double amount, const std::string& note = "ATM Withdrawal") {
        auto acc = findAccount(accNo);
        acc->withdraw(amount); // Virtual dispatch
        recordTransaction(accNo, "WITHDRAW", amount, acc->getBalance(), note);
    }

    void transferFunds(int fromAccNo, int toAccNo, double amount) {
        auto sender = findAccount(fromAccNo);
        auto receiver = findAccount(toAccNo);
        // Strong exception safety: if sender->withdraw throws, receiver is untouched
        sender->withdraw(amount);
        receiver->deposit(amount, "Transfer from Acc #" + std::to_string(fromAccNo));
        recordTransaction(fromAccNo, "TRANSFER-OUT", amount, sender->getBalance(),
                          "To Acc #" + std::to_string(toAccNo));
        recordTransaction(toAccNo, "TRANSFER-IN", amount, receiver->getBalance(),
                          "From Acc #" + std::to_string(fromAccNo));
    }

    void creditAnnualInterestToAll() {
        std::for_each(accountDirectory.begin(), accountDirectory.end(),
            [this](std::pair<const int, std::shared_ptr<BankAccount>>& entry) {
                double interest = entry.second->calculateInterest();
                if (interest > 0.0) {
                    entry.second->applyAnnualInterest();
                    recordTransaction(entry.first, "INTEREST", interest,
                                      entry.second->getBalance(), "Annual Interest Credit");
                }
            }
        );
    }

    void flushSMSAlerts() {
        smsObserver->flushNotifications();
    }

    void displayAllAccounts() const {
        std::cout << "  ----------------------------------------------------------------------------------------\n";
        for (const auto& pair : accountDirectory) {
            std::cout << "  " << *(pair.second) << "\n";
        }
        std::cout << "  ----------------------------------------------------------------------------------------\n";
    }

    // --- Unit 4: STL Algorithms (sort, find_if, accumulate, transform) ---
    void runSTLAnalyticsReport() const {
        std::vector<std::shared_ptr<BankAccount>> vec;
        for (const auto& kv : accountDirectory) {
            vec.push_back(kv.second);
        }

        // 1. std::sort with Lambda (Sort accounts by balance descending)
        std::sort(vec.begin(), vec.end(),
            [](const std::shared_ptr<BankAccount>& a, const std::shared_ptr<BankAccount>& b) {
                return a->getBalance() > b->getBalance();
            }
        );

        std::cout << "  [STL std::sort] Accounts Ranked by Balance (Highest -> Lowest):\n";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << "    #" << (i + 1) << " -> " << *(vec[i]) << "\n";
        }

        // 2. std::accumulate with Lambda (Total Liquidity in Bank)
        double totalLiquidity = std::accumulate(vec.begin(), vec.end(), 0.0,
            [](double sum, const std::shared_ptr<BankAccount>& acc) {
                return sum + acc->getBalance();
            }
        );

        // 3. std::find_if with Lambda (First Premium Account with Balance >= Rs. 50,000)
        auto premiumIt = std::find_if(vec.begin(), vec.end(),
            [](const std::shared_ptr<BankAccount>& acc) {
                return acc->getBalance() >= 50000.0;
            }
        );

        std::cout << "\n";
        printMetricRow("Total Active Accounts (Static Count)", BankAccount::getTotalActiveAccounts());
        printMetricRow("Total Bank Liquidity (std::accumulate)", totalLiquidity);
        if (premiumIt != vec.end()) {
            printMetricRow("Top Premium Holder (std::find_if)", (*premiumIt)->getHolderName());
        }
        if (!auditStack.isEmpty()) {
            printMetricRow("Latest Audit Stack Tx ID (Template)", auditStack.peek().getTransactionId());
        }
    }

    void lookupByCustomerName(const std::string& name) const {
        auto it = holderNameIndex.find(name);
        if (it != holderNameIndex.end()) {
            auto acc = findAccount(it->second);
            std::cout << "  [unordered_map O(1) Lookup] Found '" << name << "' -> " << *acc << "\n";
        } else {
            std::cout << "  [unordered_map O(1) Lookup] Customer '" << name << "' not found.\n";
        }
    }

    // --- Unit 5: CSV File I/O with seekg(), seekp(), tellg(), tellp() ---
    void saveAndVerifyCSVFiles(const std::string& accountsCsv, const std::string& txCsv) const {
        std::vector<std::streampos> rowOffsets;

        // Write Accounts CSV using ios::out | ios::trunc | ios::binary
        std::fstream accFile(accountsCsv, std::ios::out | std::ios::trunc | std::ios::binary);
        if (accFile.is_open()) {
            accFile << "AccNo,Type          ,HolderName      ,Balance\n";
            for (const auto& kv : accountDirectory) {
                rowOffsets.push_back(accFile.tellp());
                accFile << kv.second->toCSVRow() << "\n";
            }
            std::cout << "  [CSV Writer] Saved " << accountDirectory.size() << " accounts to '"
                      << accountsCsv << "' (Final tellp() = " << accFile.tellp() << " bytes)\n";
            accFile.close();
        }

        // Write Transactions CSV
        std::fstream txFile(txCsv, std::ios::out | std::ios::trunc | std::ios::binary);
        if (txFile.is_open()) {
            txFile << "TxID,AccNo,Type,Amount,BalanceAfter,Remarks\n";
            for (const auto& tx : transactionLedger) {
                txFile << tx.getTransactionId() << ","
                       << tx.getAccountNo() << ","
                       << tx.getType() << ","
                       << std::fixed << std::setprecision(2) << tx.getAmount() << ","
                       << tx.getBalanceAfter() << ","
                       << tx.getRemarks() << "\n";
            }
            std::cout << "  [CSV Writer] Saved " << transactionLedger.size() << " transactions to '"
                      << txCsv << "' (Final tellp() = " << txFile.tellp() << " bytes)\n";
            txFile.close();
        }

        // Demonstrate Random Access Read using seekg() and tellg() on accountsCsv
        if (rowOffsets.size() >= 2) {
            std::fstream inAcc(accountsCsv, std::ios::in | std::ios::binary);
            if (inAcc.is_open()) {
                inAcc.seekg(rowOffsets[1], std::ios::beg);
                std::streampos posBefore = inAcc.tellg();
                std::string secondRecordLine;
                std::getline(inAcc, secondRecordLine);
                std::streampos posAfter = inAcc.tellg();
                std::cout << "  [File Pointer seekg/tellg] Jumped directly to Record #2 at byte "
                          << posBefore << " -> Read: [" << secondRecordLine
                          << "] (tellg() after = " << posAfter << " bytes)\n";
                inAcc.close();
            }
        }
    }
};

// ============================================================================
// Automated End-to-End Syllabus Demonstration + Interactive Console Menu
// ============================================================================
void runAutomatedDemonstration(BankSystem& bank) {
    std::cout << "\n========================================================================================\n";
    std::cout << "                   " << ::BANK_TITLE << " - MINI PROJECT\n";
    std::cout << "========================================================================================\n\n";

    // 1. Account Creation using Factory Method Pattern & Inheritance Hierarchy
    std::cout << "[PHASE 1] Creating Accounts via Factory Pattern (Savings, Current, Fixed Deposit):\n";
    int acc1 = bank.openAccount(AccountCategory::SAVINGS,       "Nishant Jha",    45000.00);
    int acc2 = bank.openAccount(AccountCategory::CURRENT,       "Aarav Kulkarni", 80000.00);
    int acc3 = bank.openAccount(AccountCategory::FIXED_DEPOSIT, "Sneha Patil",    100000.00);
    int acc4 = bank.openAccount(AccountCategory::SAVINGS,       "Riya Verma",     15000.00);
    (void)acc3;
    bank.displayAllAccounts();
    bank.flushSMSAlerts();
    std::cout << "\n";

    // 2. Deposits, Withdrawals, Operator+= Overloading & Fund Transfers
    std::cout << "[PHASE 2] Executing Deposits, Withdrawals, Operator+= & Inter-Account Transfers:\n";
    bank.depositToAccount(acc1, 10000.00, "Salary Credit");
    *(bank.findAccount(acc4)) += 5000.00; // Overloaded operator+=
    bank.withdrawFromAccount(acc2, 12000.00, "Vendor Payment");
    bank.transferFunds(acc2, acc1, 15000.00);
    bank.flushSMSAlerts();
    std::cout << "\n";

    // 3. Runtime Polymorphism: Crediting Annual Interest Across Account Hierarchy
    std::cout << "[PHASE 3] Runtime Polymorphism - Applying Annual Interest (Virtual calculateInterest()):\n";
    bank.creditAnnualInterestToAll();
    bank.flushSMSAlerts();
    bank.displayAllAccounts();
    std::cout << "\n";

    // 4. Custom Exception Hierarchy & Strong Exception Safety Verification
    std::cout << "[PHASE 4] Custom Exception Hierarchy Tests (try-catch-throw):\n";
    try {
        std::cout << "  Test A: Attempting negative deposit of Rs. -500 on Acc #" << acc1 << "...\n";
        bank.depositToAccount(acc1, -500.00);
    } catch (const BankException& ex) {
        std::cout << "    Caught -> " << ex.what() << "\n";
    }

    try {
        std::cout << "  Test B: Attempting withdrawal breaching Rs. 1000 minimum balance on Acc #" << acc4 << "...\n";
        bank.withdrawFromAccount(acc4, 20500.00);
    } catch (const BankException& ex) {
        std::cout << "    Caught -> " << ex.what() << "\n";
    }

    try {
        std::cout << "  Test C: Querying non-existent Account #9999...\n";
        bank.findAccount(9999);
    } catch (const BankException& ex) {
        std::cout << "    Caught -> " << ex.what() << "\n";
    }
    std::cout << "\n";

    // 5. STL Containers & Algorithms Analytics
    std::cout << "[PHASE 5] STL Containers & Algorithms Analytics (sort, accumulate, find_if, unordered_map):\n";
    bank.runSTLAnalyticsReport();
    bank.lookupByCustomerName("Nishant Jha");
    std::cout << "\n";

    // 6. CSV File Persistence & File Pointers (seekg, seekp, tellg, tellp)
    std::cout << "[PHASE 6] CSV File Persistence & Random Access File Pointers (seekg / tellg):\n";
    bank.saveAndVerifyCSVFiles("bank_accounts.csv", "bank_transactions.csv");
    std::cout << "========================================================================================\n";
}

int main() {
    BankSystem& bank = BankSystem::getInstance();

    // Run full syllabus showcase first
    runAutomatedDemonstration(bank);

    // Optional interactive menu loop if user provides interactive menu choices
    while (true) {
        std::cout << "\n--- INTERACTIVE BANK MENU (Enter 0 or EOF to Exit) ---\n";
        std::cout << "  1. Open New Account      2. Deposit Funds       3. Withdraw Funds\n";
        std::cout << "  4. Transfer Funds        5. View All Accounts   6. STL Analytics & Save CSV\n";
        std::cout << "  0. Exit\n";
        std::cout << "Choice: ";

        int choice = 0;
        if (!(std::cin >> choice) || choice == 0) {
            std::cout << "Exiting " << ::BANK_TITLE << ". Goodbye!\n";
            break;
        }

        try {
            if (choice == 1) {
                int cat = 1;
                std::string name;
                double initDep = 0.0;
                std::cout << "Enter Category (1=Savings, 2=Current, 3=FixedDeposit): ";
                std::cin >> cat;
                std::cout << "Enter Holder Name (single_word_or_underscore): ";
                std::cin >> name;
                std::cout << "Enter Initial Deposit: ";
                std::cin >> initDep;
                int newAcc = bank.openAccount(static_cast<AccountCategory>(cat), name, initDep);
                std::cout << "Opened Account #" << newAcc << "!\n";
                bank.flushSMSAlerts();
            } else if (choice == 2) {
                int accNo; double amt;
                std::cout << "Enter Account No & Deposit Amount: ";
                std::cin >> accNo >> amt;
                bank.depositToAccount(accNo, amt);
                bank.flushSMSAlerts();
            } else if (choice == 3) {
                int accNo; double amt;
                std::cout << "Enter Account No & Withdrawal Amount: ";
                std::cin >> accNo >> amt;
                bank.withdrawFromAccount(accNo, amt);
                bank.flushSMSAlerts();
            } else if (choice == 4) {
                int fromAcc, toAcc; double amt;
                std::cout << "Enter Sender Acc, Receiver Acc & Amount: ";
                std::cin >> fromAcc >> toAcc >> amt;
                bank.transferFunds(fromAcc, toAcc, amt);
                bank.flushSMSAlerts();
            } else if (choice == 5) {
                bank.displayAllAccounts();
            } else if (choice == 6) {
                bank.runSTLAnalyticsReport();
                bank.saveAndVerifyCSVFiles("bank_accounts.csv", "bank_transactions.csv");
            }
        } catch (const BankException& ex) {
            std::cout << "  [Error] " << ex.what() << "\n";
        }
    }

    return 0;
}
