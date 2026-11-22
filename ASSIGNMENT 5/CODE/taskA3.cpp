
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
using namespace std;


class Bill {
    string t; double amount;
public:
    Bill(const string& type, double a) : t(type), amount(a) {}
    virtual ~Bill() = default;
    virtual double total() const { return amount; }
    virtual string type() const { return t; }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
    virtual string name() const = 0;
    double patientPays(const Bill& b) const { return b.total() - amountCovered(b); }
};
class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0; }
    string name() const override { return "SelfPay"; }
};
class Insurance : public Payer {
    double limit, copay;                    
public:
    Insurance(double l, double c) : limit(l), copay(c) {}
    double amountCovered(const Bill& b) const override { return min(limit, b.total() * (100 - copay) / 100); }
    string name() const override { return "Insurance"; }
};
class GovernmentScheme : public Payer {
    map<string, double> rates;              
public:
    GovernmentScheme() : rates{{"OP", 1000}, {"IP", 45000}, {"DC", 30000}, {"ER", 3500}} {}
    double amountCovered(const Bill& b) const override {
        auto it = rates.find(b.type());
        return it == rates.end() ? 0 : min(it->second, b.total());
    }
    string name() const override { return "Government"; }
};


class CorporateTieUp : public Payer {
    double deductible;
public:
    explicit CorporateTieUp(double d) : deductible(d) {}
    double amountCovered(const Bill& b) const override { return max(0.0, b.total() - deductible); }
    string name() const override { return "Corporate"; }
};


using Policy = function<double(const Bill&)>;

int main() {
    Bill bills[] = { Bill("OP", 1700), Bill("IP", 50500), Bill("DC", 30000), Bill("ER", 4000) };
    SelfPay sp; Insurance ins(50000, 20); GovernmentScheme gov; CorporateTieUp corp(100);
    const Payer* payers[] = { &sp, &ins, &gov, &corp };

    for (const Payer* p : payers) {
        cout << p->name() << ":\n";
        for (const Bill& b : bills)
            cout << "   " << b.type() << " total " << b.total() << "  covered " << p->amountCovered(b)
                 << "  patient pays " << p->patientPays(b) << "\n";
    }

    double limit = 50000, copay = 20;                      
    Policy insuranceFn = [limit, copay](const Bill& b) { return min(limit, b.total() * (100 - copay) / 100); };
    cout << "std::function insurance on IP bill: " << insuranceFn(bills[1]) << "\n";
    cout << "sizeof(Insurance) = " << sizeof(Insurance) << "   sizeof(Policy) = " << sizeof(Policy) << "\n";
    return 0;
}
