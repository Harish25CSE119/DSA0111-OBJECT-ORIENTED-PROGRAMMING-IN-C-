// Task 5: Operators and precedence. original() is the given program; the rest adds the metro expressions.
// Compile: g++ -std=c++17 -Wall -Wextra Task05_Operators_Precedence.cpp -o task05
#include <iostream>

void original()
{
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';
    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';
    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';
    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';
}

bool isPeak(int hour) { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }

constexpr bool PEAK_TABLE[24] = {
    false, false, false, false, false, false, false, false,   // 0-7
    true,  true,  false, false, false, false, false, false,   // 8-9 peak
    false, true,  true,  true,  false, false, false, false};  // 17-19 peak
bool isPeakTable(int hour) { return PEAK_TABLE[hour]; }

int main()
{
    std::cout << "--- given program ---\n";
    original();

    std::cout << "--- metro expressions ---\n";
    long long base = 3000;
    bool peak = isPeak(18);
    long long fare = base + (peak ? base * 20 / 100 : 0);        // ?: has lower precedence than + so parentheses are needed
    fare -= 0; fare += 0;                                        // compound assignment (right-to-left)
    std::cout << "Outer, 18h fare = " << fare << " paise\n";
    long long concession = fare * 75 / 100;                      // * and / left-to-right: (fare*75)/100
    std::cout << "with 25% concession = " << concession << " paise\n";

    std::cout << "hour: expression vs table\n";
    bool allSame = true;
    for (int h = 0; h < 24; ++h) {
        if (isPeak(h) != isPeakTable(h)) allSame = false;
        if (h == 7 || h == 8 || h == 9 || h == 10 || h == 17 || h == 19 || h == 20)
            std::cout << "  " << h << ": " << isPeak(h) << ' ' << isPeakTable(h) << '\n';
    }
    std::cout << "table matches expression for all 24 hours: " << std::boolalpha << allSame << '\n';

    std::cout << "--- ++ forms ---\n";
    int i = 5;
    int a = i++;     // a = 5, i = 6
    int b = ++i;     // i = 7, b = 7
    std::cout << a << ' ' << b << ' ' << i << '\n';
    // i = i++;      // undefined/unspecified before C++17 and pointless: i is modified twice
}
