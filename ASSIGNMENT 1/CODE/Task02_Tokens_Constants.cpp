// Task 2: Tokens, keywords, identifiers and constants.
// Compile: g++ -std=c++17 -Wall -Wextra Task02_Tokens_Constants.cpp -o task02
#include <iostream>

#define FARE_INNER_MACRO 1000            // macro: no type, no scope, no storage
const int  BASE_CONST = 1000;            // const: typed, scoped, run-time or compile-time
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};   // constexpr: guaranteed compile-time
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

enum class Zone { Inner, Middle, Outer };          // scoped, no implicit int conversion
enum OldZone    { OZ_Inner, OZ_Middle, OZ_Outer }; // plain enum: leaks names, converts to int
constexpr int ZONE_COUNT = static_cast<int>(Zone::Outer) + 1;

static_assert(sizeof(BASE_PAISE) / sizeof(BASE_PAISE[0]) == ZONE_COUNT,
              "fare table must have one entry per zone");

int main()
{
    // Tokens in this line:  keyword=long/int  identifier=total  operator==,+  constant=1000,20  punctuator=; ( ) { }
    long long total = BASE_PAISE[0] + BASE_PAISE[0] * PEAK_SURCHARGE_PERCENT / 100;
    std::cout << "Inner peak fare (paise): " << total << '\n';

    // enum vs enum class
    OldZone oz = OZ_Outer;
    int asInt = oz;                                    // allowed silently
    Zone z = Zone::Outer;
    // int bad = z;                                    // ERROR: no implicit conversion
    // if (z == OZ_Outer) {}                           // ERROR: different types
    int ok = static_cast<int>(z);                      // explicit and visible
    std::cout << "old enum as int: " << asInt << ", enum class as int: " << ok << '\n';

    // macro pitfalls
#define SQUARE_BAD(x) x * x
    std::cout << "SQUARE_BAD(2+1) = " << SQUARE_BAD(2 + 1) << "  (expected 9, got 5)\n";
    constexpr int sq = (2 + 1) * (2 + 1);
    std::cout << "constexpr square  = " << sq << '\n';

    std::cout << "macro value = " << FARE_INNER_MACRO << ", const = " << BASE_CONST
              << ", constexpr table[2] = " << BASE_PAISE[2] << '\n';
    std::cout << "Min balance (paise): " << MIN_BALANCE_PAISE << '\n';
}
