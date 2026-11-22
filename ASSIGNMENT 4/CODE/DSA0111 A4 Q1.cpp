
#include <iostream>
#include <string>
using namespace std;

const double DA_RATE = 0.42;  

class Staff {
    string name;
    int id;
protected:
    double basic;
public:
    Staff(const string& n, int i, double b) : name(n), id(i), basic(b) {}
    virtual ~Staff() = default;
    virtual double pay() const = 0;          
    virtual string role() const = 0;
    void show() const { cout << role() << " " << id << " " << name << "  pay = " << pay() << "\n"; }
};

class Professor : public Staff {
    double research;
public:
    Professor(const string& n, int i, double b, double r) : Staff(n, i, b), research(r) {}
    double pay() const override { return basic + basic * DA_RATE + research; }
    string role() const override { return "Professor"; }
};

class LabAssistant : public Staff {
public:
    LabAssistant(const string& n, int i, double b) : Staff(n, i, b) {}
    double pay() const override { return basic + basic * DA_RATE; }
    string role() const override { return "LabAssistant"; }
};

class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(const string& n, int i, double h, double r) : Staff(n, i, 0), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
    string role() const override { return "GuestFaculty"; }
};


struct StaffOne {
    int type;                  
    string name; int id;
    double basic, research, hours, rate;   
    double pay() const {
        switch (type) {
            case 0: return basic + basic * DA_RATE + research;
            case 1: return basic + basic * DA_RATE;
            case 2: return hours * rate;
        }
        return 0;
    }
};

int main() {
    Staff* all[3] = { new Professor("Meena", 1, 50000, 5000),
                      new LabAssistant("Ravi", 2, 20000),
                      new GuestFaculty("Anita", 3, 10, 1500) };
    double total = 0;
    for (Staff* s : all) { s->show(); total += s->pay(); }  
    cout << "Total payroll = " << total << "\n";
    for (Staff* s : all) delete s;

    StaffOne one{0, "Meena", 1, 50000, 5000, 0, 0};
    cout << "StaffOne pay = " << one.pay() << "\n";
    cout << "sizeof(StaffOne)=" << sizeof(StaffOne) << "  sizeof(Professor)=" << sizeof(Professor)
         << "  sizeof(LabAssistant)=" << sizeof(LabAssistant) << "\n";
    return 0;
}
