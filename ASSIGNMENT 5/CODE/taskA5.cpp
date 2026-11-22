
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;


namespace predict {
struct Guard {
    string n;
    explicit Guard(const string& s) : n(s) {}
    ~Guard() { cout << '~' << n << ' '; }
};
void f() {
    Guard g("f");
    throw 42;
}
void run() {
    try {
        Guard m("main");
        f();
        cout << "not printed ";
    } catch (int x) {
        cout << "caught " << x;
    } catch (...) {
        cout << "other";
    }
    cout << '\n';
}
}

struct BillingError : runtime_error { using runtime_error::runtime_error; };
void validateCharge(double c) { if (c < 0) throw BillingError("negative charge"); }
void processBill(double c) {
    try { validateCharge(c); }
    catch (const BillingError& e) { cout << "[log: " << e.what() << "] "; throw; }   
}
void processAll() {
    for (double c : {100.0, -1.0, 250.0}) {
        try { processBill(c); cout << "billed " << c << "\n"; }
        catch (const BillingError& e) { cout << "skipped (" << e.what() << ")\n"; }
    }
}


struct Resource {
    static int live;
    Resource() { ++live; }
    ~Resource() { --live; }
};
int Resource::live = 0;

void manual(int mode) {                      
    Resource* r = new Resource;
    try {
        if (mode == 1) throw runtime_error("error path 1");
        if (mode == 2) throw logic_error("error path 2 (added later, catch forgotten)");
        delete r;
    } catch (const runtime_error&) {
        delete r;                            
        throw;
    }                                         
}
void raii(int mode) {
    auto r = make_unique<Resource>();         
    if (mode == 1) throw runtime_error("error path 1");
    if (mode == 2) throw logic_error("error path 2");
}

int main() {
    cout << "--- predict ---\n";
    predict::run();
    cout << "--- catch, log, rethrow ---\n";
    processAll();
    cout << "--- manual delete versus RAII ---\n";
    for (int m : {0, 1, 2}) {
        try { manual(m); } catch (const exception&) {}
        cout << "after manual(" << m << "): live resources = " << Resource::live << "\n";
    }
    Resource::live = 0;
    for (int m : {0, 1, 2}) {
        try { raii(m); } catch (const exception&) {}
        cout << "after raii(" << m << "):   live resources = " << Resource::live << "\n";
    }
    return 0;
}
