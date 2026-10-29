// Task 3: Data types, sizes, ranges, precision and overflow.
// Compile: g++ -std=c++17 -Wall -Wextra Task03_DataTypes.cpp -o task03
#include <climits>
#include <cfloat>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "---- sizeof on this system ----\n";
    std::cout << "bool " << sizeof(bool) << ", char " << sizeof(char) << ", int " << sizeof(int)
              << ", unsigned " << sizeof(unsigned) << ", long long " << sizeof(long long)
              << ", float " << sizeof(float) << ", double " << sizeof(double) << '\n';
    std::cout << "---- ranges ----\n";
    std::cout << "int:       " << INT_MIN << " .. " << INT_MAX << '\n';
    std::cout << "unsigned:  0 .. " << UINT_MAX << '\n';
    std::cout << "long long: " << LLONG_MIN << " .. " << LLONG_MAX << '\n';
    std::cout << "double digits10 = " << DBL_DIG << ", float digits10 = " << FLT_DIG << '\n';

    std::cout << "---- values chosen for the metro system ----\n";
    constexpr long long maxFare = 3600;                  // paise (Outer, peak)
    constexpr long long journeys = 3000000;              // whole network, one day
    long long dayTotal = maxFare * journeys;
    std::cout << "worst-case day total (paise) = " << dayTotal << '\n';
    std::cout << "fits in int?       " << std::boolalpha << (dayTotal <= INT_MAX) << '\n';
    std::cout << "fits in long long? " << (dayTotal <= LLONG_MAX) << '\n';

    std::cout << "---- precision problem: double rupees ----\n";
    double sumD = 0.0; long long sumP = 0;
    for (int i = 0; i < 1000000; ++i) { sumD += 0.10; sumP += 10; }
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "double sum of 1,000,000 x Rs 0.10 = " << sumD << " (exact Rs 100000)\n";
    std::cout << "error = " << sumD - 100000.0 << '\n';
    std::cout << "paise sum = " << sumP << " -> Rs " << sumP / 100 << '.' << sumP % 100 << "0\n";
    std::cout << std::setprecision(17) << "0.1 + 0.2 = " << 0.1 + 0.2 << ", == 0.3 ? " << (0.1 + 0.2 == 0.3) << '\n';

    std::cout << "---- mixed-type expression ----\n";
    int i = 3; double d = 2.5; bool b = true;
    auto r = i + d + b;                                  // int -> double, bool -> int -> double
    std::cout << "3 + 2.5 + true = " << r << " (type double, " << sizeof(r) << " bytes)\n";

    std::cout << "---- overflow resolved ----\n";
    // Signed int overflow is undefined behaviour, so we show the wrapped value with unsigned arithmetic.
    unsigned wrapped = static_cast<unsigned>(maxFare) * static_cast<unsigned>(journeys);
    std::cout << "32-bit wrapped result = " << static_cast<long long>(static_cast<int>(wrapped))
              << "  vs correct " << dayTotal << '\n';
}
