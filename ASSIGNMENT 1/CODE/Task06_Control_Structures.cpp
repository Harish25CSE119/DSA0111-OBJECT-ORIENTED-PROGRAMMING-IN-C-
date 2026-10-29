// Task 6: Control structures. TODO 1 and TODO 2 completed (switch + integer paise).
// Compile: g++ -std=c++17 -Wall -Wextra Task06_Control_Structures.cpp -o task06
#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
constexpr int BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;    // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {                                  // TODO 1
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
        default:           return -1;             // unknown zone: invalid
    }
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

bool canTravel(long long balance, long long f)
{
    return f >= 0 && balance - f >= MIN_BALANCE_PAISE;   // TODO 2
}

// ---- extra: peak rule and boundary check requested by the task ----
bool isPeak(int hour) { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }

int main()
{
    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << " balance Rs " << balance / 100 << '\n';
    }

    std::cout << "--- boundary hours (added) ---\n";
    for (int h : {7, 8, 9, 10, 17, 19, 20})
        std::cout << "hour " << std::setw(2) << h << " -> " << (isPeak(h) ? "peak" : "off-peak")
                  << ", Outer fare = " << fare(Zone::Outer, isPeak(h)) << " paise\n";
    std::cout << "unknown zone -> " << fare(static_cast<Zone>(7), false) << " (invalid)\n";
}
