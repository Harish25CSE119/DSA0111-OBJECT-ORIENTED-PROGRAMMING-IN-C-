
#include <iostream>
#include <map>
#include <new>
#include <stdexcept>
#include <string>
using namespace std;

struct BillingError : runtime_error { using runtime_error::runtime_error; };
struct NegativeChargeError : BillingError { using BillingError::BillingError; };
struct ClaimError : BillingError { using BillingError::BillingError; };
struct UnknownPackageError : BillingError { using BillingError::BillingError; };
struct MissingRecordError : BillingError { using BillingError::BillingError; };

double checkCharge(double c) {
    if (c < 0) throw NegativeChargeError("negative charge " + to_string(c));
    return c;
}
double claim(double billed, double cover) {
    if (billed > cover) throw ClaimError("claim " + to_string(billed) + " exceeds cover " + to_string(cover));
    return billed;
}
double packageRate(const string& code) {
    static const map<string, double> rates{{"APPY", 30000}, {"CATA", 20000}};
    auto it = rates.find(code);
    if (it == rates.end()) throw UnknownPackageError("unknown package " + code);
    return it->second;
}
string findPatient(const map<int, string>& db, int id) {
    auto it = db.find(id);
    if (it == db.end()) throw MissingRecordError("no record for patient " + to_string(id));
    return it->second;
}

void allocateHuge() {
#ifdef __SANITIZE_ADDRESS__
    size_t n = size_t(-1) / 4;
#else
    size_t n = size_t(1) << 45;
#endif
    double* p = new double[n];
    delete[] p;
}

class Ledger {
    int entries = 0;
public:
    void add() noexcept { ++entries; }
    int count() const noexcept { return entries; }
    ~Ledger() noexcept { cout << "[ledger closed with " << entries << " entries] "; }   // never throws
};


enum class Status { Ok, Negative, OverCover, UnknownPackage };
Status claimCode(double billed, double cover, double& out) {
    if (billed < 0 || cover < 0) return Status::Negative;
    if (billed > cover) return Status::OverCover;
    out = billed;
    return Status::Ok;                
}

int main() {
    map<int, string> db{{1, "Ravi"}, {2, "Meena"}};
    Ledger ledger;
    try { checkCharge(-50); } catch (const NegativeChargeError& e) { cout << "NegativeCharge: " << e.what() << "\n"; }
    try { claim(60000, 50000); } catch (const ClaimError& e) { cout << "Claim: " << e.what() << "\n"; }
    try { packageRate("XYZ"); } catch (const UnknownPackageError& e) { cout << "Package: " << e.what() << "\n"; }
    try { findPatient(db, 9); } catch (const BillingError& e) { cout << "Billing (base catch): " << e.what() << "\n"; }
    try { allocateHuge(); } catch (const bad_alloc& e) { cout << "bad_alloc: " << e.what() << "\n"; }
    ledger.add(); ledger.add();

    double paid = 0;
    Status s = claimCode(60000, 50000, paid);
    cout << "return-code version status = " << int(s) << " (2 = OverCover)\n";
    cout << "ledger count = " << ledger.count() << "\n";
    return 0;
}
