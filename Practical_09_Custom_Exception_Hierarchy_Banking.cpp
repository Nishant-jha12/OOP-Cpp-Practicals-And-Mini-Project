// ============================================================================
// Practical 9: Custom Exception Hierarchy for Banking Transactions
// Course     : Object Oriented Programming using C++ Laboratory (E0054B)
// Syllabus   : Unit 5 - Exception Handling (try-catch-throw, Custom Exception
//              Hierarchy, noexcept specifier, Exception Safety Guarantees)
// ============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <sstream>

// ============================================================================
// Custom Exception Hierarchy Inheriting from std::exception
// ============================================================================
class BankingException : public std::exception {
protected:
    std::string errorMessage;
    int accountNo;

public:
    BankingException(int accNo, const std::string& msg)
        : errorMessage(msg), accountNo(accNo) {}

    virtual ~BankingException() noexcept {}

    // Override std::exception::what() with noexcept guarantee (no-throw)
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

    int getAccountNo() const noexcept {
        return accountNo;
    }
};

// Derived Exception 1: Invalid Transaction Amount (<= 0)
class InvalidAmountException : public BankingException {
public:
    InvalidAmountException(int accNo, double attemptedAmount)
        : BankingException(accNo, "") {
        std::ostringstream oss;
        oss << "[InvalidAmountException] Acc #" << accNo
            << ": Transaction amount (Rs. " << std::fixed << std::setprecision(2)
            << attemptedAmount << ") must be strictly positive!";
        errorMessage = oss.str();
    }
};

// Derived Exception 2: Insufficient Funds
class InsufficientFundsException : public BankingException {
private:
    double availableBalance;
    double requestedAmount;

public:
    InsufficientFundsException(int accNo, double balance, double requested)
        : BankingException(accNo, ""),
          availableBalance(balance),
          requestedAmount(requested) {
        std::ostringstream oss;
        oss << "[InsufficientFundsException] Acc #" << accNo
            << ": Requested Rs. " << std::fixed << std::setprecision(2) << requested
            << ", but available balance is only Rs. " << balance
            << " (Shortfall: Rs. " << (requested - balance) << ")";
        errorMessage = oss.str();
    }

    double getShortfall() const noexcept {
        return requestedAmount - availableBalance;
    }
};

// Derived Exception 3: Daily Withdrawal Limit Exceeded
class DailyLimitExceededException : public BankingException {
public:
    DailyLimitExceededException(int accNo, double requested, double dailyLimit)
        : BankingException(accNo, "") {
        std::ostringstream oss;
        oss << "[DailyLimitExceededException] Acc #" << accNo
            << ": Withdrawal of Rs. " << std::fixed << std::setprecision(2) << requested
            << " exceeds daily ATM limit of Rs. " << dailyLimit << "!";
        errorMessage = oss.str();
    }
};

// Derived Exception 4: Frozen / Locked Account
class AccountFrozenException : public BankingException {
public:
    explicit AccountFrozenException(int accNo)
        : BankingException(accNo,
            "[AccountFrozenException] Acc #" + std::to_string(accNo) +
            ": Account is currently frozen for security verification!") {}
};

// ============================================================================
// BankAccount Class Demonstrating Strong & No-Throw Exception Safety Guarantees
// ============================================================================
class SecureBankAccount {
private:
    int accountNo;
    std::string holderName;
    double balance;
    double dailyLimit;
    bool isFrozen;

public:
    SecureBankAccount(int accNo, const std::string& name, double initBal, double limit)
        : accountNo(accNo), holderName(name), balance(initBal),
          dailyLimit(limit), isFrozen(false) {}

    // No-throw guarantee (noexcept)
    double getBalance() const noexcept { return balance; }
    void setFrozen(bool frozenState) noexcept { isFrozen = frozenState; }

    // Strong exception safety guarantee: if an exception is thrown, account state is unchanged
    void deposit(double amount) {
        if (isFrozen) {
            throw AccountFrozenException(accountNo);
        }
        if (amount <= 0.0) {
            throw InvalidAmountException(accountNo, amount);
        }
        balance += amount;
        std::cout << "  [SUCCESS] Deposited Rs. " << std::fixed << std::setprecision(2)
                  << amount << " to Acc #" << accountNo
                  << ". New Balance: Rs. " << balance << "\n";
    }

    void withdraw(double amount) {
        if (isFrozen) {
            throw AccountFrozenException(accountNo);
        }
        if (amount <= 0.0) {
            throw InvalidAmountException(accountNo, amount);
        }
        if (amount > dailyLimit) {
            throw DailyLimitExceededException(accountNo, amount, dailyLimit);
        }
        if (amount > balance) {
            throw InsufficientFundsException(accountNo, balance, amount);
        }
        balance -= amount;
        std::cout << "  [SUCCESS] Withdrew Rs. " << std::fixed << std::setprecision(2)
                  << amount << " from Acc #" << accountNo
                  << ". Remaining Balance: Rs. " << balance << "\n";
    }
};

void executeTransactionScenario(int scenarioId, const std::string& description,
                                void (*transactionFunc)(SecureBankAccount&),
                                SecureBankAccount& acc) {
    std::cout << "Scenario " << scenarioId << ": " << description << "\n";
    try {
        transactionFunc(acc);
    } catch (const InvalidAmountException& e) {
        std::cout << "  -> Caught Specific Derived: " << e.what() << "\n";
    } catch (const DailyLimitExceededException& e) {
        std::cout << "  -> Caught Specific Derived: " << e.what() << "\n";
    } catch (const InsufficientFundsException& e) {
        std::cout << "  -> Caught Specific Derived: " << e.what() << "\n";
    } catch (const AccountFrozenException& e) {
        std::cout << "  -> Caught Specific Derived: " << e.what() << "\n";
    } catch (const BankingException& e) {
        std::cout << "  -> Caught Base BankingException: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "  -> Caught Standard Exception: " << e.what() << "\n";
    } catch (...) {
        std::cout << "  -> Caught Unknown Exception via catch(...)!\n";
    }
    std::cout << "  -> Account Balance after Scenario " << scenarioId
              << " (Strong Guarantee Verified): Rs. "
              << std::fixed << std::setprecision(2) << acc.getBalance() << "\n\n";
}

int main() {
    std::cout << "================================================================================\n";
    std::cout << " PRACTICAL 9: CUSTOM EXCEPTION HIERARCHY (BANKING TRANSACTION SCENARIOS)\n";
    std::cout << "================================================================================\n\n";

    SecureBankAccount account(5001, "Nishant Jha", 30000.00, 25000.00);
    std::cout << "Initial Account State: Acc #5001 (Nishant Jha) | Balance: Rs. 30000.00 | Daily Limit: Rs. 25000.00\n";
    std::cout << "--------------------------------------------------------------------------------\n\n";

    // Scenario 1: Valid Deposit & Withdrawal
    executeTransactionScenario(1, "Valid Deposit (Rs. 5000) & Valid Withdrawal (Rs. 4000)",
        [](SecureBankAccount& a) {
            a.deposit(5000.00);
            a.withdraw(4000.00);
        }, account);

    // Scenario 2: Negative Deposit Amount (Throws InvalidAmountException)
    executeTransactionScenario(2, "Attempt Negative Deposit (Rs. -1500)",
        [](SecureBankAccount& a) {
            a.deposit(-1500.00);
        }, account);

    // Scenario 3: Exceeding Daily ATM Limit (Throws DailyLimitExceededException)
    executeTransactionScenario(3, "Attempt Withdrawal Exceeding Daily Limit (Rs. 28000 > Rs. 25000)",
        [](SecureBankAccount& a) {
            a.withdraw(28000.00);
        }, account);

    // Scenario 4: Insufficient Balance (First withdraw Rs. 20000, then attempt Rs. 15000 when only Rs. 11000 left)
    executeTransactionScenario(4, "Withdraw Rs. 20000, then Attempt Rs. 15000 (Insufficient Funds)",
        [](SecureBankAccount& a) {
            a.withdraw(20000.00);
            a.withdraw(15000.00);
        }, account);

    // Scenario 5: Frozen Account Transaction (Throws AccountFrozenException)
    executeTransactionScenario(5, "Freeze Account and Attempt Withdrawal of Rs. 1000",
        [](SecureBankAccount& a) {
            a.setFrozen(true);
            a.withdraw(1000.00);
        }, account);

    std::cout << "================================================================================\n";
    return 0;
}
