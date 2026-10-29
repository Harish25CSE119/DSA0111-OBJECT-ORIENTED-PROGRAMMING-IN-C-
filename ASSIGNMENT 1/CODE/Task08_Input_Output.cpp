// Task 8: safe input with cin, recovery from errors, formatted output; alternative getline+stoi.
// Compile: g++ -std=c++17 -Wall -Wextra Task08_Input_Output.cpp -o task08
// Run:     ./task08 < input08.txt     (or type the values yourself)
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr long long MIN_BALANCE_PAISE = 5000;

bool isPeak(int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); }
long long fareFor(int zone, int hour)
{
    long long f = BASE_PAISE[zone];
    return isPeak(hour) ? f + f * 20 / 100 : f;
}
std::string rs(long long paise)
{
    std::ostringstream os;
    os << "Rs " << paise / 100 << '.' << std::setw(2) << std::setfill('0') << paise % 100;
    return os.str();
}

// ---- Method 1: cin >> with fail() check ----
bool readInt(const std::string& prompt, int lo, int hi, int& out)
{
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> out)) {
            if (std::cin.eof()) return false;                        // input finished
            std::cout << "  Invalid input: type a number.\n";
            std::cin.clear();                                        // clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   // discard bad text
            continue;
        }
        std::string rest;
        std::getline(std::cin, rest);                                // rest of the line
        if (rest.find_first_not_of(" \t\r") != std::string::npos) {   // e.g. "8x"
            std::cout << "  Invalid input: extra characters after the number.\n";
            continue;
        }
        if (out < lo || out > hi) {
            std::cout << "  Out of range (" << lo << ".." << hi << ").\n";
            continue;
        }
        return true;
    }
}

// ---- Method 2 (alternative): getline + stoi + try/catch ----
bool parseInt(const std::string& line, int& out)
{
    try {
        std::size_t pos = 0;
        out = std::stoi(line, &pos);
        return pos == line.size();                                   // reject "12abc"
    } catch (const std::exception&) {                                // invalid_argument or out_of_range
        return false;
    }
}

int main()
{
    long long balance = 10000;
    int n = 0;
    if (!readInt("Number of journeys (1-5): ", 1, 5, n)) return 1;

    struct Row { int zone, hour; long long fare; bool ok; };
    std::vector<Row> rows;
    for (int i = 1; i <= n; ++i) {
        int zone = 0, hour = 0;
        if (!readInt("Journey " + std::to_string(i) + " zone (0=Inner,1=Middle,2=Outer): ", 0, 2, zone)) break;
        if (!readInt("Journey " + std::to_string(i) + " hour (0-23): ", 0, 23, hour)) break;
        long long f = fareFor(zone, hour);
        bool ok = balance - f >= MIN_BALANCE_PAISE;
        if (ok) balance -= f;
        rows.push_back({zone, hour, f, ok});
    }

    std::cout << "\n===== Journey report =====\n";
    std::cout << std::left << std::setw(4) << "No" << std::setw(8) << "Zone" << std::setw(6) << "Hour"
              << std::right << std::setw(12) << "Fare" << "  Status\n";
    const char* names[] = {"Inner", "Middle", "Outer"};
    int no = 1;
    for (const Row& r : rows)
        std::cout << std::left << std::setw(4) << no++ << std::setw(8) << names[r.zone] << std::setw(6) << r.hour
                  << std::right << std::setw(12) << rs(r.fare) << "  " << (r.ok ? "OK" : "DENIED") << '\n';
    std::cout << "Closing balance: " << rs(balance) << '\n';
    std::cout << std::fixed << std::setprecision(2) << "Closing balance (double view): " << balance / 100.0 << '\n';

    std::cout << "\n--- getline + stoi alternative ---\n";
    for (const std::string s : {"8", "abc", "12x", "99999999999", ""}) {
        int v = 0;
        std::cout << "\"" << s << "\" -> " << (parseInt(s, v) ? "accepted " + std::to_string(v) : std::string("rejected")) << '\n';
    }
}
