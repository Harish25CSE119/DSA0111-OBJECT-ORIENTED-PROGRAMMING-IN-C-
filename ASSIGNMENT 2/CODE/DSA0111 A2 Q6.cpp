
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

class Time {
    int h, m;
public:
    Time(int hh = 0, int mm = 0) { h = hh; m = mm; }

    int getH() const { return h; }
    int getM() const { return m; }

    
    int minutesTo(const Time &other) const
    {
        return abs((other.h * 60 + other.m) - (h * 60 + m));
    }

    friend int minutesInline(const Time &a, const Time &b);
    friend int minutesNoInline(const Time &a, const Time &b);
};

inline int minutesInline(const Time &a, const Time &b)
{
    return abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}


__attribute__((noinline)) int minutesNoInline(const Time &a, const Time &b)
{
    return abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}


inline int minutesGetters(const Time &a, const Time &b)
{
    return abs((b.getH() * 60 + b.getM()) - (a.getH() * 60 + a.getM()));
}

int main()
{
    const int CALLS = 10000000;
    Time t[1000];
    for (int i = 0; i < 1000; i++) t[i] = Time(i % 24, i % 60);

    long long sum;
    clock_t start;
    double ms;

    sum = 0;
    start = clock();
    for (int i = 0; i < CALLS; i++) sum += minutesInline(t[i % 1000], t[(i + 1) % 1000]);
    ms = (clock() - start) * 1000.0 / CLOCKS_PER_SEC;
    cout << "friend inline    : " << ms << " ms  (sum " << sum << ")" << endl;

    sum = 0;
    start = clock();
    for (int i = 0; i < CALLS; i++) sum += minutesNoInline(t[i % 1000], t[(i + 1) % 1000]);
    ms = (clock() - start) * 1000.0 / CLOCKS_PER_SEC;
    cout << "friend no inline : " << ms << " ms  (sum " << sum << ")" << endl;

    sum = 0;
    start = clock();
    for (int i = 0; i < CALLS; i++) sum += t[i % 1000].minutesTo(t[(i + 1) % 1000]);
    ms = (clock() - start) * 1000.0 / CLOCKS_PER_SEC;
    cout << "member function  : " << ms << " ms  (sum " << sum << ")" << endl;

    sum = 0;
    start = clock();
    for (int i = 0; i < CALLS; i++) sum += minutesGetters(t[i % 1000], t[(i + 1) % 1000]);
    ms = (clock() - start) * 1000.0 / CLOCKS_PER_SEC;
    cout << "public getters   : " << ms << " ms  (sum " << sum << ")" << endl;

    return 0;
}
