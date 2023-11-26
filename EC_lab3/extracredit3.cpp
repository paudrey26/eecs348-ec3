#include <iostream>
#include <iomanip>

class Account {
public:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):\n"
                  << "   Holder: " << accountHolder << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << getBalance() << "\n";
    }

    void deposit(double amount) {
        balance += amount;
        std::cout << "Account (" << accountNumber << ") deposited $" << amount << ". New balance: $" << balance << "\n";
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Account (" << accountNumber << ") withdrawn $" << amount << ". New balance: $" << balance << "\n";
        } else {
            std::cout << "Account (" << accountNumber << ") withdrawal failed. Insufficient funds.\n";
        }
    }

    double getBalance() const {
        return balance;
    }

    std::string getHolderName() const {
        return accountHolder;
    }

    // Operator overloading for transferring money between accounts
    friend void operator+(Account& from, Account& to) {
        double transferAmount = 300;

        if (from.getBalance() >= transferAmount) {
            from.withdraw(transferAmount);
            to.deposit(transferAmount);
            std::cout << "Transferred $" << transferAmount << " from Account (" << from.accountNumber << ") to Account (" << to.accountNumber << ").\n";
        } else {
            std::cout << "Transfer failed. Insufficient funds in Account (" << from.accountNumber << ").\n";
        }
    }

    // Operator overloading for displaying account details
    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Account Details:\n"
           << "   Holder: " << account.getHolderName() << "\n"
           << "   Balance: $" << std::fixed << std::setprecision(2) << account.getBalance() << "\n";
        return os;
    }
};

class SavingsAccount : public Account {
public:
    double interestRate;

    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%\n";
    }

    void withdraw(double amount) override {
        const double minimumBalance = 100; // Minimum balance requirement for savings account
        if (balance - amount >= minimumBalance) {
            balance -= amount;
            std::cout << "Account (" << accountNumber << ") withdrawn $" << amount << ". New balance: $" << balance << "\n";
        } else {
            std::cout << "Account (" << accountNumber << ") withdrawal failed. Minimum balance requirement not met.\n";
        }
    }
};

class CurrentAccount : public Account {
public:
    double overdraftLimit;

    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << "\n";
    }

    void withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            std::cout << "Account (" << accountNumber << ") withdrawn $" << amount << ". New balance: $" << balance << "\n";
        } else {
            std::cout << "Account (" << accountNumber << ") withdrawal failed. Overdraft limit exceeded.\n";
        }
    }
};

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    savings + current;

    savings.displayDetails();
    current.displayDetails();

    return 0;
}
