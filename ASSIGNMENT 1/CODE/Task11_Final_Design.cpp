// Task 11: recommended final design: constexpr FareTable, encapsulated SmartCard, Gate that processes journeys.
// Optional: fare table loaded from "fares.txt" (one integer paise per zone) with safe fallback to compiled defaults.
// Compile: g++ -std=c++17 -Wall -Wextra Task11_Final_Design.cpp -o task11
#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

enum class Zone { Inner, Middle, Outer };
constexpr int ZONE_COUNT = 3;

struct FareConfig {
    std::array<long long, ZONE_COUNT> base{1000, 2000, 3000};      // compiled defaults, paise
    int peakSurchargePercent = 20;
    long long minBalance = 5000;
};

// Alternative concept: read fares from a file at start-up (O(z) time, z values).
bool loadFares(const char* path, FareConfig& cfg)
{
    std::ifstream in(path);
    if (!in) return false;
    std::array<long long, ZONE_COUNT> tmp{};
    for (auto& v : tmp) if (!(in >> v) || v <= 0) return false;    // reject bad file, keep defaults
    cfg.base = tmp;
    return true;
}

bool isPeak(int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); }

class SmartCard {
    long long balance_;
    long long minBalance_;
public:
    SmartCard(long long b, long long minB) : balance_(b), minBalance_(minB) {}
    long long balance() const { return balance_; }
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= minBalance_; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    bool recharge(long long a) { if (a <= 0) return false; balance_ += a; return true; }
};

class Gate {
    FareConfig cfg_;
public:
    explicit Gate(const FareConfig& c) : cfg_(c) {}
    long long fare(Zone z, int hour) const {
        long long f = cfg_.base[static_cast<std::size_t>(z)];
        return isPeak(hour) ? f + f * cfg_.peakSurchargePercent / 100 : f;
    }
    bool process(SmartCard& c, Zone z, int hour, long long& collected) const {
        long long f = fare(z, hour);
        if (!c.deduct(f)) return false;
        collected += f;
        return true;
    }
};

int main()
{
    FareConfig cfg;
    bool fromFile = loadFares("fares.txt", cfg);
    std::cout << "Fare table source: " << (fromFile ? "fares.txt" : "compiled defaults") << '\n';
    Gate gate(cfg);

    // Simulate a whole-network day: 3,000,000 journeys over 1000 cards, auto-recharge when denied.
    constexpr long long JOURNEYS = 3000000;
    std::vector<SmartCard> cards(1000, SmartCard(20000, cfg.minBalance));
    long long collected = 0, denied = 0, negative = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (long long j = 0; j < JOURNEYS; ++j) {
        SmartCard& c = cards[static_cast<std::size_t>(j % 1000)];
        Zone z = static_cast<Zone>(j % ZONE_COUNT);
        int hour = static_cast<int>(j % 24);
        if (!gate.process(c, z, hour, collected)) { ++denied; c.recharge(20000); }
        if (c.balance() < cfg.minBalance) ++negative;              // invariant check
    }
    auto t1 = std::chrono::steady_clock::now();
    std::cout << "Journeys processed : " << JOURNEYS << '\n';
    std::cout << "Collected          : Rs " << collected / 100 << '.' << std::setw(2) << std::setfill('0')
              << collected % 100 << std::setfill(' ') << '\n';
    std::cout << "Denied (recharged) : " << denied << '\n';
    std::cout << "Invariant violations (balance < Rs 50): " << negative << '\n';
    std::cout << "Elapsed: " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";
}
