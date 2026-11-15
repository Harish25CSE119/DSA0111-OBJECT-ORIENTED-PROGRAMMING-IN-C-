
#include <iomanip>
#include <iostream>
using namespace std;

class Money {
    long long paise;   
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    
    Money operator+(const Money& o) const {
        Money m;
        m.paise = paise + o.paise;
        return m;
    }
    Money operator-(const Money& o) const {
        Money m;
        m.paise = paise - o.paise;    
        return m;
    }
    
    Money& operator+=(const Money& o) { paise += o.paise; return *this; }

    
    bool operator==(const Money& o) const { return paise == o.paise; }
    bool operator<(const Money& o) const { return paise < o.paise; }

   
    friend ostream& operator<<(ostream& os, const Money& m) {
        long long v = m.paise < 0 ? -m.paise : m.paise;
        if (m.paise < 0) os << "-";
        os << "Rs " << v / 100 << "." << setw(2) << setfill('0') << v % 100 << setfill(' ');
        return os;
    }
    friend istream& operator>>(istream& is, Money& m) {
        long long r; int p;
        is >> r >> p;              
        m = Money(r, p);
        return is;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    cout << c << '\n' << boolalpha << (c == Money(200)) << '\n';

   
    cout << "0.75 + 0.50 = " << Money(0, 75) + Money(0, 50) << "\n";   

    
    cout << "5.00 - 8.50 = " << Money(5) - Money(8, 50) << "\n";

    
    Money bill;
    bill += Money(250, 50);     
    bill += Money(99, 99);      
    bill += Money(10);          
    cout << "Bill total: " << bill << "\n";
    cout << "a < b ? " << (a < b) << "\n";

    
    double d = 0; Money m;
    for (int i = 0; i < 1000000; i++) { d += 0.10; m += Money(0, 10); }
    cout << fixed << setprecision(10) << "double: " << d << "\n";
    cout << "Money : " << m << "  (exact: Rs 100000.00)\n";
    return 0;
}
