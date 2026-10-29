// Task 1: Object-oriented analysis of the Namma Metro fare problem.
// Objects: Zone, Category/Passenger, Journey, FareRule (abstract) + two rules, FareTable, SmartCard.
// Compile: g++ -std=c++17 -Wall -Wextra Task01_OOP_Analysis.cpp -o task01
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

enum class Zone { Inner, Middle, Outer };
enum class Category { General, Student, Senior };

// ---- FareTable : one place for all fare data (abstraction) ----
class FareTable {
public:
    static constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
    static constexpr int PEAK_SURCHARGE_PERCENT = 20;
    static constexpr int CONCESSION_PERCENT = 25;      // assumption: 25% off for students/seniors
    static constexpr long long MIN_BALANCE_PAISE = 5000;
    static long long base(Zone z) { return BASE_PAISE[static_cast<int>(z)]; }
};
constexpr long long FareTable::BASE_PAISE[];

// ---- Journey : data of one trip ----
struct Journey {
    Zone zone;   // farthest zone travelled
    int hour;    // 0..23
    bool isPeak() const { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }
};

// ---- Abstract rule + derived rules (inheritance + polymorphism) ----
class FareRule {
public:
    virtual ~FareRule() = default;
    virtual long long apply(long long paise, const Journey& j, Category c) const = 0;
    virtual std::string name() const = 0;
};
class PeakRule : public FareRule {
public:
    long long apply(long long p, const Journey& j, Category) const override {
        return j.isPeak() ? p + p * FareTable::PEAK_SURCHARGE_PERCENT / 100 : p;
    }
    std::string name() const override { return "PeakRule"; }
};
class ConcessionRule : public FareRule {
public:
    long long apply(long long p, const Journey&, Category c) const override {
        return c == Category::General ? p : p * (100 - FareTable::CONCESSION_PERCENT) / 100;
    }
    std::string name() const override { return "ConcessionRule"; }
};

// ---- Passenger ----
class Passenger {
    std::string name_;
    Category cat_;
public:
    Passenger(std::string n, Category c) : name_(std::move(n)), cat_(c) {}
    const std::string& name() const { return name_; }
    Category category() const { return cat_; }
};

// ---- SmartCard : encapsulation guards the balance invariant ----
class SmartCard {
    long long balance_;   // paise, private
public:
    explicit SmartCard(long long initial) : balance_(initial < 0 ? 0 : initial) {}
    long long balance() const { return balance_; }
    bool recharge(long long amount) { if (amount <= 0) return false; balance_ += amount; return true; }
    bool canTravel(long long fare) const { return fare >= 0 && balance_ - fare >= FareTable::MIN_BALANCE_PAISE; }
    bool deduct(long long fare) { if (!canTravel(fare)) return false; balance_ -= fare; return true; }
};

std::string rs(long long paise) {
    std::ostringstream os;
    os << "Rs " << paise / 100 << '.' << std::setw(2) << std::setfill('0') << paise % 100;
    return os.str();
}

int main()
{
    Passenger p("Asha", Category::Student);
    SmartCard card(10000);
    std::vector<std::unique_ptr<FareRule>> rules;
    rules.push_back(std::make_unique<PeakRule>());
    rules.push_back(std::make_unique<ConcessionRule>());

    Journey trips[] = {{Zone::Inner, 7}, {Zone::Outer, 9}, {Zone::Middle, 12}, {Zone::Outer, 18}};
    std::cout << "Passenger: " << p.name() << "\n";
    for (const Journey& j : trips) {
        long long f = FareTable::base(j.zone);
        for (const auto& r : rules) f = r->apply(f, j, p.category());   // polymorphic call
        if (card.deduct(f)) std::cout << "Fare " << rs(f) << "  balance " << rs(card.balance()) << '\n';
        else                std::cout << "Denied (fare " << rs(f) << ")  balance " << rs(card.balance()) << '\n';
    }
    std::cout << "Recharge -100 accepted? " << std::boolalpha << card.recharge(-100) << '\n';
}
