
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Time {
    int h, m, s;
public:
    Time(int hh = 0, int mm = 0, int ss = 0) : h(hh), m(mm), s(ss) {}

    Time& operator++() {           
        if (++s == 60) { s = 0; if (++m == 60) { m = 0; h++; } }
        return *this;
    }
    Time operator++(int) {          
        Time old = *this;           
        ++(*this);
        return old;
    }
    bool operator==(const Time& o) const { return h == o.h && m == o.m && s == o.s; }
    bool operator<(const Time& o) const {
        return h != o.h ? h < o.h : (m != o.m ? m < o.m : s < o.s);
    }
    friend ostream& operator<<(ostream& os, const Time& t) {
        os << t.h << ":" << (t.m < 10 ? "0" : "") << t.m << ":" << (t.s < 10 ? "0" : "") << t.s;
        return os;
    }
};

class Playlist {
    vector<string> tracks;
public:
    void add(const string& t) { tracks.push_back(t); }
    int size() const { return (int)tracks.size(); }

    string& operator[](int i) {                 
        static string bad = "<invalid>";
        if (i < 0 || i >= size()) { cout << "Error: index " << i << " out of range\n"; return bad; }
        return tracks[i];
    }
    const string& operator[](int i) const {      
        static const string bad = "<invalid>";
        if (i < 0 || i >= size()) { cout << "Error: index " << i << " out of range\n"; return bad; }
        return tracks[i];
    }
};


struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; }
    Counter operator++(int) { Counter old = *this; ++v; return old; }
};

int main() {
    Time t(0, 59, 59);
    Time a = t++;
    Time b = ++t;
    cout << "a=" << a << " b=" << b << " t=" << t << "\n";
    cout << "a<b: " << (a < b) << "  a==b: " << (a == b) << "\n";

    Playlist pl;
    pl.add("Song One"); pl.add("Song Two"); pl.add("Song Three");
    const Playlist& cpl = pl;
    pl[1] = "Song Two (remix)";
    cout << pl[0] << " | " << cpl[1] << "\n";
    cout << pl[-1] << "\n";
    cout << pl[pl.size()] << "\n";

    cout << "Playback: ";
    Time pos;
    for (int i = 0; i < 3; i++) cout << pos++ << " ";
    cout << "\n";

   
    using namespace std::chrono;
    Time x;
    auto t1 = steady_clock::now();
    for (int i = 0; i < 10000000; i++) ++x;
    auto t2 = steady_clock::now();
    for (int i = 0; i < 10000000; i++) x++;
    auto t3 = steady_clock::now();
    cout << "prefix : " << duration<double, milli>(t2 - t1).count() << " ms\n";
    cout << "postfix: " << duration<double, milli>(t3 - t2).count() << " ms\n";
    cout << x << "\n";  

    Counter c{5};
    Counter ca = c++;
    Counter cb = ++c;
    cout << ca.v << ' ' << cb.v << ' ' << c.v << '\n';   
    return 0;
}
