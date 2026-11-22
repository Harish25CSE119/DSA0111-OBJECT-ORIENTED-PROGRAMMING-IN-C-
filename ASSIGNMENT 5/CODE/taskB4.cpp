
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;


struct BillingError : runtime_error { using runtime_error::runtime_error; };
struct NegativeChargeError : BillingError { using BillingError::BillingError; };
struct UnknownPackageError : BillingError { using BillingError::BillingError; };

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
    OutPatientBill(const string& p, double c, double t) : Bill(p), consultation(c), tests(t) {
        if (c < 0 || t < 0) throw NegativeChargeError("negative charge");
    }
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
};
class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0; }
};
class Insurance : public Payer {
    double limit, copay;
public:
    Insurance(double l, double c) : limit(l), copay(c) {}
    double amountCovered(const Bill& b) const override { return min(limit, b.total() * (100 - copay) / 100); }
};
class GovernmentScheme : public Payer {
    map<string, double> rates;
public:
    GovernmentScheme() : rates{{"OP", 1000}, {"IP", 45000}, {"DC", 30000}, {"ER", 3500}} {}
    double amountCovered(const Bill& b) const override {
        auto it = rates.find(b.type());
        if (it == rates.end()) throw UnknownPackageError("no package rate for " + b.type());
        return min(it->second, b.total());
    }
};


class TeleConsultBill : public Bill {                
    double minutes, perMinute;
public:
    TeleConsultBill(const string& p, double m, double r) : Bill(p), minutes(m), perMinute(r) {}
    double total() const override { return minutes * perMinute; }
    string type() const override { return "TC"; }
};
class CorporateTieUp : public Payer {                  // new payer
    double deductible;
public:
    explicit CorporateTieUp(double d) : deductible(d) {}
    double amountCovered(const Bill& b) const override { return max(0.0, b.total() - deductible); }
};


class RuleBill : public Bill {
    double rate, units, fixed; string name;
public:
    RuleBill(const string& p, const string& line) : Bill(p) {
        istringstream in(line);
        if (!(in >> name >> rate >> units >> fixed)) throw BillingError("bad rule line: " + line);
    }
    double total() const override { return rate * units + fixed; }
    string type() const override { return "RULE:" + name; }
};

int failures = 0;
void check(const string& label, double got, double expected) {
    bool ok = got == expected;
    if (!ok) ++failures;
    cout << (ok ? "PASS " : "FAIL ") << label << ": " << got << (ok ? "" : "  expected " + to_string(expected)) << "\n";
}

int main() {
    cout << "--- earlier tests (hand-verified values) ---\n";
    vector<unique_ptr<Bill>> bills;
    bills.push_back(make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(make_unique<InPatientBill>("Meena", 3, 2500, 40000, 3000));
    bills.push_back(make_unique<DayCareBill>("Anil", 30000));
    bills.push_back(make_unique<EmergencyBill>("Sara", 2000, 4, 500));
    double expectedTotal[] = { 1700, 50500, 30000, 4000 };
    double expectedInsurance[] = { 1360, 40400, 24000, 3200 };
    double expectedGov[] = { 1000, 45000, 30000, 3500 };
    Insurance ins(50000, 20); GovernmentScheme gov; SelfPay sp;
    for (size_t i = 0; i < bills.size(); ++i) {
        check(bills[i]->type() + " total", bills[i]->total(), expectedTotal[i]);
        check(bills[i]->type() + " insurance", ins.amountCovered(*bills[i]), expectedInsurance[i]);
        check(bills[i]->type() + " government", gov.amountCovered(*bills[i]), expectedGov[i]);
        check(bills[i]->type() + " selfpay", sp.amountCovered(*bills[i]), 0);
    }

    cout << "--- new bill type and new payer (no existing class edited) ---\n";
    TeleConsultBill tc("Kavya", 20, 25);                  // 500
    CorporateTieUp corp(100);
    check("TC total", tc.total(), 500);
    check("TC insurance", ins.amountCovered(tc), 400);
    check("TC corporate", corp.amountCovered(tc), 400);
    check("IP corporate", corp.amountCovered(*bills[1]), 50400);
    try { gov.amountCovered(tc); cout << "FAIL government should reject TC\n"; ++failures; }
    catch (const UnknownPackageError& e) { cout << "PASS government rejects TC: " << e.what() << "\n"; }

    cout << "--- B4.2 exceptions thrown and caught (ASan must stay clean) ---\n";
    try { OutPatientBill bad("X", -5, 10); } catch (const BillingError& e) { cout << "PASS caught: " << e.what() << "\n"; }
    try { RuleBill r("Y", "oops"); } catch (const BillingError& e) { cout << "PASS caught: " << e.what() << "\n"; }

    cout << "--- plug-in style rule (configuration text instead of a C++ class) ---\n";
    RuleBill dialysis("Dev", "dialysis 1500 4 500");
    check("RULE dialysis", dialysis.total(), 6500);

    cout << (failures == 0 ? "ALL TESTS PASS\n" : "SOME TESTS FAILED\n");
    return failures == 0 ? 0 : 1;
}
