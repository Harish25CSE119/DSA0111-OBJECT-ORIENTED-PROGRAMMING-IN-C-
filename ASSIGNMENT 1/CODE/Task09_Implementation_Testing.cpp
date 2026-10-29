// Task 9: complete fare logic with a table-driven test suite.
// Compile: g++ -std=c++17 -Wall -Wextra Task09_Implementation_Testing.cpp -o task09
#include <iostream>
#include <iomanip>
#include <string>

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int ZONE_COUNT = 3;
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

bool isPeak(int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); }

// returns -1 for an invalid zone or hour
long long fare(int zone, int hour)
{
    if (zone < 0 || zone >= ZONE_COUNT || hour < 0 || hour > 23) return -1;
    long long f = BASE_PAISE[zone];
    if (isPeak(hour)) f += f * PEAK_SURCHARGE_PERCENT / 100;
    return f;
}
bool canTravel(long long balance, long long f) { return f >= 0 && balance - f >= MIN_BALANCE_PAISE; }
bool recharge(long long& balance, long long amount) { if (amount <= 0) return false; balance += amount; return true; }
bool parseHour(const std::string& s, int& h)
{
    try { std::size_t p = 0; h = std::stoi(s, &p); return p == s.size(); }
    catch (const std::exception&) { return false; }
}

int total = 0, passed = 0;
void check(const std::string& name, long long expected, long long actual)
{
    ++total; bool ok = expected == actual; if (ok) ++passed;
    std::cout << std::left << std::setw(34) << name << std::right << std::setw(8) << expected
              << std::setw(8) << actual << "  " << (ok ? "PASS" : "FAIL") << '\n';
}

int main()
{
    std::cout << std::left << std::setw(34) << "Case" << std::right << std::setw(8) << "Expect"
              << std::setw(8) << "Actual" << "  Result\n";

    // table of fare cases: name, zone, hour, expected paise
    struct Case { const char* name; int zone, hour; long long expected; };
    const Case cases[] = {
        {"Inner off-peak 12:00", 0, 12, 1000}, {"Middle off-peak 12:00", 1, 12, 2000},
        {"Outer off-peak 12:00", 2, 12, 3000}, {"Inner peak 09:00", 0, 9, 1200},
        {"Middle peak 18:00", 1, 18, 2400},    {"Outer peak 08:00", 2, 8, 3600},
        {"Boundary 07:59 ~ hour 7", 2, 7, 3000}, {"Boundary 08:00 (peak)", 2, 8, 3600},
        {"Boundary 10:00 (off-peak)", 2, 10, 3000}, {"Boundary 17:00 (peak)", 2, 17, 3600},
        {"Boundary 19 (peak)", 2, 19, 3600},   {"Boundary 20:00 (off-peak)", 2, 20, 3000},
        {"Invalid zone 3", 3, 12, -1},         {"Invalid zone -1", -1, 12, -1},
        {"Invalid hour 24", 0, 24, -1},
    };
    for (const Case& c : cases) check(c.name, c.expected, fare(c.zone, c.hour));

    // balance boundaries
    check("Balance 8600, fare 3600 -> allowed", 1, canTravel(8600, 3600));      // leaves exactly 5000
    check("Balance 8599, fare 3600 -> denied", 0, canTravel(8599, 3600));
    check("Balance 5000, fare 1000 -> denied", 0, canTravel(5000, 1000));

    // recharge
    long long bal = 6000;
    check("Recharge -500 rejected", 0, recharge(bal, -500));
    check("Balance unchanged after bad recharge", 6000, bal);
    check("Recharge 4000 accepted", 1, recharge(bal, 4000));
    check("Balance after recharge", 10000, bal);

    // letters typed for hour
    int h = 0;
    check("parseHour(\"abc\") rejected", 0, parseHour("abc", h));
    check("parseHour(\"9x\") rejected", 0, parseHour("9x", h));
    check("parseHour(\"9\") accepted", 1, parseHour("9", h));

    // five journeys with hand-calculated total: 1000+2400+3000+1200+3000 = 10600
    long long sum = 0;
    sum += fare(0, 7); sum += fare(1, 9); sum += fare(2, 12); sum += fare(0, 18); sum += fare(2, 20);
    check("5-journey total (hand: 10600)", 10600, sum);

    std::cout << "\nPassed " << passed << " of " << total << " tests\n";
    return passed == total ? 0 : 1;
}
