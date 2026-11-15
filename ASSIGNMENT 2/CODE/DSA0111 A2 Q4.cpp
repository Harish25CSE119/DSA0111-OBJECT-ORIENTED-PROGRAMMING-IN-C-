
#include <iostream>
using namespace std;

const int MAX = 10;     

struct Transaction {
    char type;          
    double amount;
};

class SavingsAccount {
private:
    int accountNumber;
    double balance;
    Transaction history[MAX];   
    int count;                  

    void addTransaction(char type, double amount);

public:
    SavingsAccount(int no, double opening)
    {
        accountNumber = no;
        balance = opening;
        count = 0;
    }

    
    void deposit(double amount)
    {
        balance = balance + amount;
        addTransaction('D', amount);
    }

   
    bool withdraw(double amount);
    void miniStatement();
};


void SavingsAccount::addTransaction(char type, double amount)
{
    if (count == MAX) {
        for (int i = 1; i < MAX; i++)
            history[i - 1] = history[i];    
        count--;
    }
    history[count].type = type;
    history[count].amount = amount;
    count++;
}

bool SavingsAccount::withdraw(double amount)
{
    if (amount > balance) return false;
    balance = balance - amount;
    addTransaction('W', amount);
    return true;
}

void SavingsAccount::miniStatement()
{
    cout << "Mini statement for account " << accountNumber << endl;
    for (int i = 0; i < count; i++) {
        if (history[i].type == 'D') cout << "  Deposit    ";
        else                        cout << "  Withdrawal ";
        cout << history[i].amount << endl;
    }
    cout << "  Balance: " << balance << endl;
}

int main()
{
    SavingsAccount acc(1001, 1000);

   
    for (int i = 1; i <= 7; i++) acc.deposit(100 * i);
    for (int i = 1; i <= 6; i++) acc.withdraw(50 * i);

    acc.miniStatement();
    return 0;
}


