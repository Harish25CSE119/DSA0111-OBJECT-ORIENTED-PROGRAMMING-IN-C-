
#include <iostream>
#include <string>
using namespace std;
#ifdef NOVDTOR
#define VD
#define OVR
#else
#define VD virtual
#define OVR override
#endif

namespace predict {
struct Bill {
    VD ~Bill() { cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { cout << total() << ' '; }   
};
struct OPBill : Bill {
    ~OPBill() OVR { cout << "~OPBill "; }
    double total() const override { return 500; }
};
void run() {
    Bill* b = new OPBill;
    b->show();
    delete b;
    cout << '\n';
}
}

namespace binding {
struct B { double early() const { return 1; } virtual double late() const { return 1; } virtual ~B() = default; };
struct D : B { double early() const { return 2; } double late() const override { return 2; } };
void run() {
    D d; B* p = &d;
    cout << "early (non-virtual): " << p->early() << "   late (virtual): " << p->late() << "\n";
}
}

namespace casts {
struct Bill { virtual ~Bill() = default; };
struct OP : Bill { double total() const { return 500; } };
struct IP : Bill { double total() const { return 47500; } };
struct DC : Bill { double total() const { return 30000; } };
double total(const Bill* b) {                     
    if (auto p = dynamic_cast<const OP*>(b)) return p->total();
    if (auto p = dynamic_cast<const IP*>(b)) return p->total();
    if (auto p = dynamic_cast<const DC*>(b)) return p->total();
    return 0;
}
void run() {
    OP a; IP b; DC c;
    const Bill* all[] = { &a, &b, &c };
    double s = 0;
    for (const Bill* x : all) s += total(x);
    cout << "dynamic_cast total = " << s << "   sizeof(OP) = " << sizeof(OP) << " (still holds a vptr)\n";
}
}

int main() {
    cout << "--- Part 1: predict ---\n";
    predict::run();
    cout << "--- Part 2: binding ---\n";
    binding::run();
    cout << "--- Part 3: dynamic_cast alternative ---\n";
    casts::run();
    return 0;
}
