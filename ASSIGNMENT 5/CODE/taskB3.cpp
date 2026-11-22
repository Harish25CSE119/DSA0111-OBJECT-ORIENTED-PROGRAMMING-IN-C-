
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
using namespace std;
using namespace std::chrono;


class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};
class OP : public Bill { double c, t; public: OP(double a, double b) : c(a), t(b) {} double total() const override { return c + t; } };
class IP : public Bill { double d, r, s; public: IP(double a, double b, double e) : d(a), r(b), s(e) {} double total() const override { return d * r + s; } };
class DC : public Bill { double p; public: explicit DC(double a) : p(a) {} double total() const override { return p; } };
class ER : public Bill { double b, h, r; public: ER(double a, double c, double d) : b(a), h(c), r(d) {} double total() const override { return b + h * r; } };

struct SBill {
    int type; double a, b, c;
    double total() const {
        switch (type) {
            case 0: return a + b;
            case 1: return a * b + c;
            case 2: return a;
            default: return a + b * c;
        }
    }
};

struct VOP { double c, t; double total() const { return c + t; } };
struct VIP { double d, r, s; double total() const { return d * r + s; } };
struct VDC { double p; double total() const { return p; } };
struct VER { double b, h, r; double total() const { return b + h * r; } };
using VBill = variant<VOP, VIP, VDC, VER>;


SBill makeS(int i) { double x = 1 + i % 7; return SBill{i % 4, 100.0 * x, x, 50.0 + i % 5}; }
unique_ptr<Bill> makeV(int i) {
    SBill s = makeS(i);
    switch (s.type) {
        case 0: return make_unique<OP>(s.a, s.b);
        case 1: return make_unique<IP>(s.a, s.b, s.c);
        case 2: return make_unique<DC>(s.a);
        default: return make_unique<ER>(s.a, s.b, s.c);
    }
}
VBill makeVar(int i) {
    SBill s = makeS(i);
    switch (s.type) {
        case 0: return VOP{s.a, s.b};
        case 1: return VIP{s.a, s.b, s.c};
        case 2: return VDC{s.a};
        default: return VER{s.a, s.b, s.c};
    }
}

volatile double sinkOut;

double timeVirtual(int n, double& result) {
    vector<unique_ptr<Bill>> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(makeV(i));
    auto t0 = steady_clock::now();
    double s = 0; for (const auto& b : v) s += b->total();
    auto t1 = steady_clock::now();
    result = s; return duration<double, milli>(t1 - t0).count();
}
double timeSwitch(int n, double& result) {
    vector<SBill> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(makeS(i));
    auto t0 = steady_clock::now();
    double s = 0; for (const auto& b : v) s += b.total();
    auto t1 = steady_clock::now();
    result = s; return duration<double, milli>(t1 - t0).count();
}
double timeVariant(int n, double& result) {
    vector<VBill> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(makeVar(i));
    auto t0 = steady_clock::now();
    double s = 0; for (const auto& b : v) s += visit([](const auto& x) { return x.total(); }, b);
    auto t1 = steady_clock::now();
    result = s; return duration<double, milli>(t1 - t0).count();
}


enum Status { OK, NEGATIVE };
Status validateCode(double amount) { return amount < 0 ? NEGATIVE : OK; }
void validateThrow(double amount) { if (amount < 0) throw invalid_argument("negative"); }

int main(int argc, char** argv) {
    bool small = argc > 1 && string(argv[1]) == "small";

    cout << "=== B3.1 correctness on 10 bills ===\n";
    double sv = 0, ss = 0, sva = 0;
    {
        vector<unique_ptr<Bill>> v; vector<SBill> s; vector<VBill> va;
        for (int i = 0; i < 10; ++i) { v.push_back(makeV(i)); s.push_back(makeS(i)); va.push_back(makeVar(i)); }
        for (int i = 0; i < 10; ++i) {
            double a = v[i]->total(), b = s[i].total(), c = visit([](const auto& x) { return x.total(); }, va[i]);
            cout << "bill " << i << ": virtual " << a << "  switch " << b << "  variant " << c
                 << (a == b && b == c ? "  agree\n" : "  MISMATCH\n");
            sv += a; ss += b; sva += c;
        }
    }
    cout << "totals: " << sv << " / " << ss << " / " << sva << "\n";

    cout << "=== B3.2 time to total n bills (ms, summing pass only, -O2) ===\n";
    cout << "n            virtual      switch       variant\n";
    for (int n : {1000, 1000000, 10000000}) {
        if (small && n > 1000000) break;
        double r1, r2, r3;
        double a = timeVirtual(n, r1), b = timeSwitch(n, r2), c = timeVariant(n, r3);
        cout << n << "\t" << a << "\t" << b << "\t" << c << (r1 == r2 && r2 == r3 ? "" : "   (sums differ!)") << "\n";
    }
    cout << "sizeof: OP " << sizeof(OP) << ", SBill " << sizeof(SBill) << ", VBill " << sizeof(VBill)
         << " (largest alternative " << sizeof(VIP) << " + tag)\n";

    cout << "=== B3.3 validating 1,000,000 bills: return codes vs exceptions (ms) ===\n";
    const int N = 1000000;
    for (int pct : {0, 1, 50}) {
        vector<double> amt(N);
        for (int i = 0; i < N; ++i) {
            bool bad = pct == 0 ? false : (pct == 1 ? i % 100 == 0 : i % 2 == 0);
            amt[i] = bad ? -1 : 100;
        }
        int bad1 = 0, bad2 = 0;
        auto t0 = steady_clock::now();
        for (double a : amt) if (validateCode(a) != OK) ++bad1;
        auto t1 = steady_clock::now();
        for (double a : amt) { try { validateThrow(a); } catch (const invalid_argument&) { ++bad2; } }
        auto t2 = steady_clock::now();
        cout << pct << "% invalid: return codes " << duration<double, milli>(t1 - t0).count()
             << "   exceptions " << duration<double, milli>(t2 - t1).count() << "   (errors found " << bad1 << " / " << bad2 << ")\n";
    }
    sinkOut = sv;
    
    return 0;
}
