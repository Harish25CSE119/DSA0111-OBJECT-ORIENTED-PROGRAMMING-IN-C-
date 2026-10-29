// Task 7: the same fare/balance logic in two styles.
// Compile: g++ -std=c++17 -Wall -Wextra Task07_Procedural_vs_OOP.cpp -o task07
#include <iostream>

// ================= Version A: procedural =================
namespace procedural {
    long long balance = 10000;                 // global: any code can change it
    const long long MIN_BALANCE = 5000;
    bool canTravel(long long f) { return balance - f >= MIN_BALANCE; }
    void deduct(long long f)    { balance -= f; }       // caller must remember to call canTravel first
}

// ================= Version B: object-oriented =================
class SmartCard {
    long long balance_;
    static constexpr long long MIN_BALANCE = 5000;
public:
    explicit SmartCard(long long b) : balance_(b) {}
    long long balance() const { return balance_; }
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
};

// ================= Version C: public member (alternative concept) =================
struct OpenCard { long long balance; };

int main()
{
    std::cout << "--- procedural ---\n";
    if (procedural::canTravel(3600)) procedural::deduct(3600);
    procedural::deduct(9000);                                   // bug: forgot canTravel -> negative balance!
    std::cout << "balance after careless call = " << procedural::balance << " paise\n";

    std::cout << "--- object-oriented ---\n";
    SmartCard c(10000);
    std::cout << "deduct 3600: " << std::boolalpha << c.deduct(3600) << ", balance " << c.balance() << '\n';
    std::cout << "deduct 9000: " << c.deduct(9000) << ", balance " << c.balance() << " (refused)\n";
    // c.balance_ = -5;   // ERROR: balance_ is private

    std::cout << "--- public member ---\n";
    OpenCard o{10000};
    o.balance = -5;                                             // allowed: any code can break the rule
    std::cout << "balance set directly = " << o.balance << '\n';
}
