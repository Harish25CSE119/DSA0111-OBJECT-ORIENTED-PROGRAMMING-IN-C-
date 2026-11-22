
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#ifdef NOVIRTUAL
#define VT
#else
#define VT virtual
#endif
#ifdef NOVDTOR
#define VD
#define OVR
#else
#define VD virtual
#define OVR override
#endif


namespace complete {
class Bill {
protected:
    string patient;
public:
    explicit Bill(const string& p) : patient(p) {}
    virtual ~Bill() = default;
    // TODO 1
    virtual double total() const = 0;
    virtual string type() const { return "Bill"; }

    void print() const
    {
        cout << type() << ' ' << patient << ": " << total() << '\n';
    }
};
class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(const string& p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}
    // TODO 2
    double total() const override { return consultation + tests; }
    string type() const override { return "OP"; }
};
class InPatient : public Bill {
    int days;
    double perDay, surgery;
public:
    InPatient(const string& p, int d, double rate, double s)
        : Bill(p), days(d), perDay(rate), surgery(s) {}
    // TODO 3
    double total() const override { return days * perDay + surgery; }
    string type() const override { return "IP"; }
};
void run() {
    Bill* bills[] = { new OutPatient("Ravi", 500, 1200), new InPatient("Meena", 3, 2500, 40000) };
    double sum = 0;
    for (Bill* b : bills) {
        b->print();                 
        sum += b->total();
    }
    cout << "Total " << sum << '\n';
    for (Bill* b : bills) delete b; 
}
}


namespace full {
class Bill {
protected:
    string patient;
public:
    explicit Bill(const string& p) : patient(p) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual string type() const = 0;
};
class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(const string& p, double c, double t) : Bill(p), consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
    string type() const override { return "OP"; }
};
class InPatientBill : public Bill {
    int days; double perDay, procedures, medicines;
public:
    InPatientBill(const string& p, int d, double r, double pr, double m)
        : Bill(p), days(d), perDay(r), procedures(pr), medicines(m) {}
    double total() const override { return days * perDay + procedures + medicines; }
    string type() const override { return "IP"; }
};
class DayCareBill : public Bill {
    double package;
public:
    DayCareBill(const string& p, double pk) : Bill(p), package(pk) {}
    double total() const override { return package; }
    string type() const override { return "DC"; }
};
class EmergencyBill : public Bill {
    double base, hours, perHour;
public:
    EmergencyBill(const string& p, double b, double h, double r) : Bill(p), base(b), hours(h), perHour(r) {}
    double total() const override { return base + hours * perHour; }
    string type() const override { return "ER"; }
};
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
    virtual string name() const = 0;
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
void run() {
    vector<Bill*> bills = { new OutPatientBill("Ravi", 500, 1200),        
                            new InPatientBill("Meena", 3, 2500, 40000, 3000), 
                            new DayCareBill("Anil", 30000),                  
                            new EmergencyBill("Sara", 2000, 4, 500) };       
    SelfPay sp; Insurance ins(50000, 20); GovernmentScheme gov;
    const Payer* payers[] = { &sp, &ins, &gov };
    for (const Bill* b : bills) {
        cout << b->type() << " total " << b->total() << "\n";
        for (const Payer* p : payers)
            cout << "    " << p->name() << ": covered " << p->amountCovered(*b)
                 << ", patient pays " << b->total() - p->amountCovered(*b) << "\n";
    }
    for (Bill* b : bills) delete b;
}

}


namespace binding {
struct Base { VT double total() const { return 0; } virtual ~Base() = default; };
struct Derived : Base { double total() const { return 500; } };
void run() {
    Derived d; Base* p = &d;
    cout << "through Base*: " << p->total() << "   (500 = run-time binding, 0 = compile-time binding)\n";
}
}


namespace tmpl {
struct OP { double v; double total() const { return v; } };
struct IP { double v; double total() const { return v; } };
template <class T> double sumAll(const vector<T>& v) { double s = 0; for (const T& x : v) s += x.total(); return s; }
void run() {
    vector<OP> ops{{1700}, {1200}};
    vector<IP> ips{{50500}};
    cout << "template totals: OP " << sumAll(ops) << ", IP " << sumAll(ips)
         << "  (one container per type; cannot hold OP and IP in one list)\n";
}
}


namespace dtor {
struct Base { VD ~Base() { cout << "~Base "; } };
struct Derived : Base {
    int* data = new int[100];
    ~Derived() OVR { delete[] data; cout << "~Derived "; }
};
void run() {
    Base* b = new Derived;
    delete b;
    cout << "\n";
}
}

int main() {
    cout << "--- Part A: complete the code ---\n";   complete::run();
    cout << "--- Part B: all bills x all payers ---\n"; full::run();
    cout << "--- Part C: binding ---\n";               binding::run();
    cout << "--- Part D: templates ---\n";             tmpl::run();
    cout << "--- Part E: destructor ---\n";            dtor::run();
    return 0;
}
