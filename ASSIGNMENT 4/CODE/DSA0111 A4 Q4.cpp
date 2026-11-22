
#include <iostream>
#include <memory>
#include <vector>
using namespace std;


namespace buggy {
class Account {
public:
    double rate() const { return 3.5; }
    ~Account() { cout << "~Account\n"; }
};
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    double rate() const { return 4.0; }
    ~SeniorAccount() { delete[] history; cout << "~SeniorAccount\n"; }
};
void run() {
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) cout << a->rate() << "\n";   
    for (Account* a : accounts) delete a;                    
    Account copy = SeniorAccount();                          
    cout << "copy.rate() = " << copy.rate() << "\n";
}
}


namespace fixed {
class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() { cout << "~Account\n"; }
};
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    SeniorAccount() = default;
    SeniorAccount(const SeniorAccount&) = delete;            
    SeniorAccount& operator=(const SeniorAccount&) = delete;
    double rate() const override { return 4.0; }
    ~SeniorAccount() override { delete[] history; cout << "~SeniorAccount\n"; }
};
void run() {
    cout << "--- raw pointers ---\n";
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) cout << a->rate() << "\n";  
    for (Account* a : accounts) delete a;                     

    cout << "--- unique_ptr in a vector ---\n";
    vector<unique_ptr<Account>> v;
    v.push_back(make_unique<Account>());
    v.push_back(make_unique<SeniorAccount>());
    for (const auto& a : v) cout << a->rate() << "\n";
    
}
}


namespace predict {
struct Account {
    double rate() const { return 3.5; }
    virtual double vrate() const { return 3.5; }
    virtual ~Account() = default;
};
struct SeniorAccount : Account {
    double rate() const { return 4.0; }
    double vrate() const override { return 4.0; }
};
void run() {
    SeniorAccount s;
    Account* p = &s;
    Account copy = s;
    cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';
}
}

int main(int argc, char** argv) {
    if (argc > 1 && string(argv[1]) == "buggy") {
        cout << "--- Part A: buggy ---\n";
        buggy::run();
    }
    cout << "--- Part B: repaired ---\n";
    fixed::run();
    cout << "--- Part C: predict ---\n";
    predict::run();
    return 0;
}
