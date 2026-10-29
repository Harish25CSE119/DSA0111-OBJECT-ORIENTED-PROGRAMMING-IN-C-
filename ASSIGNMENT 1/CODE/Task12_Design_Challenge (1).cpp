// Task 12: redesign for a 4th zone (Airport Rs 60), 10% weekend discount (not combined with peak),
// and a monthly pass (unlimited journeys when the farthest zone is one of the two chosen zones).
// Compiles with C++11 or later. Compile: g++ -std=c++11 -Wall -Wextra Task12_Design_Challenge.cpp -o task12
#include <array>
#include <iomanip>
#include <iostream>
#include <string>

enum class Zone { Inner, Middle, Outer, Airport };
constexpr int ZONE_COUNT = 4;

// ---- single table indexed by zone: adding a zone = one new entry + one enum value ----
constexpr std::array<long long, ZONE_COUNT> BASE_PAISE{1000, 2000, 3000, 6000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
static_assert(BASE_PAISE.size() == static_cast<std::size_t>(ZONE_COUNT), "one fare per zone");

enum class DayType { Weekday, Weekend };

bool isPeak(int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); }

// Separate discount rule: weekend discount replaces the peak surcharge (they never combine).
long long fare(Zone z, int hour, DayType d)
{
    long long f = BASE_PAISE[static_cast<std::size_t>(z)];
    if (d == DayType::Weekend) return f - f * WEEKEND_DISCOUNT_PERCENT / 100;
    if (isPeak(hour))          return f + f * PEAK_SURCHARGE_PERCENT / 100;
    return f;
}

// ---- new Pass class ----
class Pass {
    Zone a_, b_;
    int validUntilDay_;
public:
    Pass(Zone a, Zone b, int validUntilDay) : a_(a), b_(b), validUntilDay_(validUntilDay) {}
    bool covers(Zone farthest, int today) const {
        return today <= validUntilDay_ && (farthest == a_ || farthest == b_);
    }
};

class SmartCard {
    long long balance_;
    Pass pass_;
    bool hasPass_;
public:
    explicit SmartCard(long long b) : balance_(b), pass_(Zone::Inner, Zone::Inner, -1), hasPass_(false) {}
    long long balance() const { return balance_; }
    void buyPass(const Pass& p) { pass_ = p; hasPass_ = true; }
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    // returns fare charged, or -1 if denied
    long long travel(Zone z, int hour, DayType d, int today) {
        if (hasPass_ && pass_.covers(z, today)) return 0;           // pass: no deduction, no minimum-balance check
        long long f = fare(z, hour, d);
        if (!canTravel(f)) return -1;
        balance_ -= f;
        return f;
    }
};

int total = 0, passed = 0;
void check(const std::string& name, long long expected, long long actual)
{
    ++total; bool ok = expected == actual; if (ok) ++passed;
    std::cout << std::left << std::setw(46) << name << std::right << std::setw(7) << expected
              << std::setw(7) << actual << "  " << (ok ? "PASS" : "FAIL") << '\n';
}

int main()
{
    std::cout << std::left << std::setw(46) << "Case" << std::right << std::setw(7) << "Expect"
              << std::setw(7) << "Actual" << "  Result\n";
    check("Airport weekday off-peak (Rs 60)",          6000, fare(Zone::Airport, 12, DayType::Weekday));
    check("Airport weekday peak (Rs 72)",              7200, fare(Zone::Airport, 9, DayType::Weekday));
    check("Outer weekend off-peak (Rs 27)",            2700, fare(Zone::Outer, 12, DayType::Weekend));
    check("Outer weekend 18:00: discount only, no surcharge", 2700, fare(Zone::Outer, 18, DayType::Weekend));
    check("Airport weekend (Rs 54)",                   5400, fare(Zone::Airport, 8, DayType::Weekend));
    check("Inner weekend (Rs 9)",                       900, fare(Zone::Inner, 12, DayType::Weekend));
    check("Original Outer peak weekday still Rs 36",   3600, fare(Zone::Outer, 17, DayType::Weekday));
    check("Original boundary 10:00 off-peak",          3000, fare(Zone::Outer, 10, DayType::Weekday));

    SmartCard c(10000);
    c.buyPass(Pass(Zone::Middle, Zone::Airport, 30));
    check("Pass: Airport journey free",                   0, c.travel(Zone::Airport, 9, DayType::Weekday, 10));
    check("Pass: Middle journey free",                    0, c.travel(Zone::Middle, 9, DayType::Weekday, 10));
    check("Pass: Outer not covered -> normal fare",    3000, c.travel(Zone::Outer, 12, DayType::Weekday, 10));
    check("Balance after the three journeys",          7000, c.balance());
    check("Pass expired (day 31) -> Airport charged", -1, c.travel(Zone::Airport, 12, DayType::Weekday, 31));
    check("Balance never below Rs 50 (>= 5000)",          1, c.balance() >= MIN_BALANCE_PAISE);

    std::cout << "\nPassed " << passed << " of " << total << " tests\n";
    return passed == total ? 0 : 1;
}
