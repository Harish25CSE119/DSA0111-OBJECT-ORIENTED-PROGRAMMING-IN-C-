
#include <chrono>
#include <iostream>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
using namespace std;


namespace predict {
void validate(double amount)
{
    if (amount < 0) throw invalid_argument("negative");
    if (amount > 1e6) throw out_of_range("too large");
    cout << "ok ";
}
void process(double amount)
{
    try {
        validate(amount);
    } catch (const out_of_range& e) {
        cout << "[log " << e.what() << "] ";
        throw;
    }
}
void run() {
    for (double a : {100.0, -5.0, 2e6}) {
        try {
            process(a);
            cout << "done ";
        } catch (const exception& e) {
            cout << "caught " << e.what() << ' ';
        }
    }
    cout << '\n';
}
}

namespace complete {
class ClaimError : public runtime_error {
public:
    using runtime_error::runtime_error;
};
double approve(double billed, double cover)
{
    // TODO 1
    if (billed < 0 || cover < 0) throw invalid_argument("negative amount");
    if (billed > cover) throw ClaimError("exceeds cover");
    return billed;
}
double settle(double billed, double cover)
{
    try {
        return approve(billed, cover);
    } catch (const ClaimError& e) {
        // TODO 2
        cout << "audit: " << e.what() << '\n';
        throw;
    }
}
void run() {
    double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (auto& c : cases) {
        try {
            double paid = settle(c[0], c[1]);
            cout << "approved " << paid << '\n';
        } catch (const ClaimError& e) {
            cout << "claim rejected: " << e.what() << '\n';
        } catch (const exception& e) {
            cout << "error: " << e.what() << '\n';
        }
    }
}

void sliceDemo() {
    try {
        try { approve(60000, 50000); }
        catch (const exception& e) { throw e; }       // sliced rethrow
    } catch (const ClaimError&) { cout << "slice demo: still a ClaimError\n"; }
    catch (const exception& e) { cout << "slice demo: sliced to plain exception, what() = " << e.what() << "\n"; }
}
}


namespace tests {
struct BillingError : runtime_error { using runtime_error::runtime_error; };
struct Guard {
    string n;
    explicit Guard(const string& s) : n(s) {}
    ~Guard() { cout << "~" << n << " "; }
};
double charge(double c) { if (c < 0) throw BillingError("negative charge"); return c; }
double parse(const string& s) { return stod(s); }                  
void run() {
    try { charge(-100); } catch (const BillingError& e) { cout << "negative charge -> " << e.what() << "\n"; }
    try { complete::approve(60000, 50000); } catch (const complete::ClaimError& e) { cout << "claim above cover -> " << e.what() << "\n"; }
    try { parse("abc"); } catch (const invalid_argument& e) { cout << "stod(\"abc\") -> invalid_argument: " << e.what() << "\n"; }
    try { parse("1e999"); } catch (const out_of_range& e) { cout << "stod(\"1e999\") -> out_of_range: " << e.what() << "\n"; }
#ifdef __SANITIZE_ADDRESS__
    size_t huge = size_t(-1) / 4;         
#else
    size_t huge = size_t(1) << 45;        
#endif
    try { double* p = new double[huge]; delete[] p; }
    catch (const bad_alloc& e) { cout << "huge array -> bad_alloc: " << e.what() << "\n"; }

    try {                                                           
        Guard a("bill-A");
        Guard b("bill-B");
        throw 7;                                                   
    } catch (...) { cout << "\ncatch(...) at top level: program continues, all objects destroyed\n"; }
}
}


namespace alt {
double sink = 0;
double approveThrow(double billed, double cover) { if (billed > cover) throw runtime_error("exceeds"); return billed; }
optional<double> approveOpt(double billed, double cover) { if (billed > cover) return nullopt; return billed; }
void run() {
    using namespace chrono;
    const int N = 1000000;
    for (int pct : {0, 1, 50}) {
        auto bad = [pct](int i) { return pct == 0 ? false : (pct == 1 ? i % 100 == 0 : i % 2 == 0); };
        auto t1 = steady_clock::now();
        for (int i = 0; i < N; ++i) {
            double billed = bad(i) ? 60000 : 1000;
            try { sink += approveThrow(billed, 50000); } catch (const exception&) { sink -= 1; }
        }
        auto t2 = steady_clock::now();
        for (int i = 0; i < N; ++i) {
            double billed = bad(i) ? 60000 : 1000;
            auto r = approveOpt(billed, 50000);
            sink += r ? *r : -1;
        }
        auto t3 = steady_clock::now();
        cout << pct << "% invalid: exceptions " << duration<double, milli>(t2 - t1).count()
             << " ms, optional " << duration<double, milli>(t3 - t2).count() << " ms\n";
    }
    cout << "sizeof(double) = " << sizeof(double) << ", sizeof(optional<double>) = " << sizeof(optional<double>) << "\n";
}
}

int main() {
    cout << "--- Part 1: predict ---\n";   predict::run();
    cout << "--- Part 2: complete the code ---\n"; complete::run(); complete::sliceDemo();
    cout << "--- Part 3: error tests ---\n"; tests::run();
    cout << "--- Part 4: optional vs exceptions (timings; repeat without ASan, with -O2) ---\n"; alt::run();
    return 0;
}
