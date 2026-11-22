
#include <iostream>
#include <string>
using namespace std;

class Bill {
protected:
    string patient;
public:
    explicit Bill(const string& p) : patient(p) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual string type() const = 0;
    void print() const { cout << type() << " " << patient << ": " << total() << "\n"; }
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

double dayTotal(Bill* bills[], int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i) sum += bills[i]->total();
    return sum;
}


struct BillSwitch {
    int type;                    
    double a, b, c, d;            
    double total() const {
        switch (type) {
            case 0: return a + b;
            case 1: return a * b + c + d;
            case 2: return a;
            case 3: return a + b * c;
        }
        return 0;
    }
};

int main() {
    Bill* bills[] = { new OutPatientBill("Ravi", 500, 1200),
                      new InPatientBill("Meena", 3, 2500, 40000, 3000),
                      new DayCareBill("Anil", 30000),
                      new EmergencyBill("Sara", 2000, 4, 500) };
    for (Bill* b : bills) b->print();
    cout << "Day total = " << dayTotal(bills, 4) << "\n";   
    for (Bill* b : bills) delete b;

    BillSwitch s[] = { {0, 500, 1200, 0, 0}, {1, 3, 2500, 40000, 3000}, {2, 30000, 0, 0, 0}, {3, 2000, 4, 500, 0} };
    double t = 0;
    for (const BillSwitch& x : s) t += x.total();
    cout << "Switch version total = " << t << "\n";
    cout << "sizeof(BillSwitch) = " << sizeof(BillSwitch) << "  sizeof(OutPatientBill) = "
         << sizeof(OutPatientBill) << "  sizeof(InPatientBill) = " << sizeof(InPatientBill) << "\n";
    
    return 0;
}
