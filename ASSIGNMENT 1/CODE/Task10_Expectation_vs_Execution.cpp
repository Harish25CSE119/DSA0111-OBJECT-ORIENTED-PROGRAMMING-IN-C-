// Task 10: compare double rupees vs integer paise vs int for 20,000 / 500,000 / 3,000,000 journeys.
// Compile: g++ -std=c++17 -Wall -Wextra Task10_Expectation_vs_Execution.cpp -o task10
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

int main()
{
    const long long fares[] = {1000, 2000, 3600, 810, 1620};   // paise (the last two are 10% weekend fares)
    const int nFares = 5;
    const long long counts[] = {20000, 500000, 3000000};

    std::cout << std::fixed;
    for (long long n : counts) {
        long long sumPaise = 0;
        double sumRupees = 0.0;
        float sumFloat = 0.0f;
        std::uint32_t wrap = 0;                                // what a 32-bit int would hold
        for (long long i = 0; i < n; ++i) {
            long long f = fares[i % nFares];
            sumPaise += f;
            sumRupees += static_cast<double>(f) / 100.0;
            sumFloat  += static_cast<float>(f) / 100.0f;
            wrap += static_cast<std::uint32_t>(f);
        }
        std::cout << "n = " << n << '\n';
        std::cout << "  long long paise : " << sumPaise << "  (Rs " << sumPaise / 100 << '.' << std::setw(2)
                  << std::setfill('0') << sumPaise % 100 << std::setfill(' ') << ")\n";
        std::cout << std::setprecision(6) << "  double rupees   : " << sumRupees
                  << "   difference from exact = " << std::setprecision(9) << sumRupees - sumPaise / 100.0 << '\n';
        std::cout << std::setprecision(2) << "  float rupees    : " << sumFloat
                  << "   difference from exact = " << sumFloat - sumPaise / 100.0 << '\n';
        std::cout << "  32-bit int      : " << static_cast<std::int32_t>(wrap)
                  << (sumPaise > INT32_MAX ? "   <-- OVERFLOW" : "   (fits)") << '\n';
    }

    // time per addition
    const long long N = 100000000;
    auto t0 = std::chrono::steady_clock::now();
    long long a = 0; for (long long i = 0; i < N; ++i) a += i % 7;
    auto t1 = std::chrono::steady_clock::now();
    double b = 0; for (long long i = 0; i < N; ++i) b += static_cast<double>(i % 7);
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "time for 1e8 additions: long long "
              << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, double "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms  (checks " << a << ' ' << b << ")\n";
}
