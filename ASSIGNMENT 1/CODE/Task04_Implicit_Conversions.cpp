// Task 4: Implicit conversions. original() is the given program (unchanged); corrected() fixes the surprises.
// Compile: g++ -std=c++17 -Wall -Wextra Task04_Implicit_Conversions.cpp -o task04   (expect a -Wsign-compare warning)
#include <iostream>

void original()
{
    int a = 7, b = 2;
    double c = a / b;
    double d = static_cast<double>(a) / b;
    std::cout << c << ' ' << d << '\n';

    char ch = 'A' + 2;
    std::cout << ch << ' ' << ch + 1 << '\n';

    bool flag = 5;
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u = 1;
    int s = -1;
    std::cout << (s < u ? "s < u" : "s >= u") << '\n';
}

void corrected()
{
    int a{7}, b{2};
    double c{static_cast<double>(a) / b};            // intended 3.5
    std::cout << "c = " << c << '\n';
    // int x{3.7};                                   // ERROR: narrowing, caught at compile time
    int x = static_cast<int>(3.7);                   // explicit truncation -> 3
    std::cout << "static_cast<int>(3.7) = " << x << '\n';

    char ch{static_cast<char>('A' + 2)};
    std::cout << ch << ' ' << static_cast<int>(ch) + 1 << '\n';

    bool flag{5 != 0};                               // explicit intent
    std::cout << flag << '\n';

    unsigned int u{1};
    int s{-1};
    std::cout << (s < static_cast<long long>(u) ? "s < u" : "s >= u") << '\n';   // true comparison
}

int main()
{
    std::cout << "--- original ---\n";
    original();
    std::cout << "--- corrected ---\n";
    corrected();
}
