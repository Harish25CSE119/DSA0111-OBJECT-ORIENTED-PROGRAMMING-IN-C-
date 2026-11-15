
#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

class Complex {
    double re, im;
public:
    Complex() : re(0), im(0) {}                         
    Complex(double r) : re(r), im(0) {}                 
    Complex(double r, double i) : re(r), im(i) {}        

    Complex operator+(const Complex& o) const { return Complex(re + o.re, im + o.im); }
    Complex operator-(const Complex& o) const { return Complex(re - o.re, im - o.im); }
    Complex operator*(const Complex& o) const {         
        return Complex(re * o.re - im * o.im, re * o.im + im * o.re);
    }
    Complex operator/(const Complex& o) const {          
        double den = o.re * o.re + o.im * o.im;
        if (den == 0) {
            cout << "Error: division by zero impedance\n";
            return Complex(0, 0);
        }
        return Complex((re * o.re + im * o.im) / den, (im * o.re - re * o.im) / den);
    }
    bool operator==(const Complex& o) const {
        return fabs(re - o.re) < 1e-9 && fabs(im - o.im) < 1e-9;
    }
    double magnitude() const { return sqrt(re * re + im * im); }

    
    friend Complex operator+(double a, const Complex& b) { return Complex(a) + b; }
    friend ostream& operator<<(ostream& os, const Complex& z) {
        os << z.re << (z.im < 0 ? " - " : " + ") << fabs(z.im) << "j";
        return os;
    }
};

int main() {
    double w = 1000, R = 100, L = 0.1, C = 5e-6;    
    Complex ZR(R, 0);
    Complex ZL(0, w * L);            
    Complex ZC(0, -1.0 / (w * C));   

   
    Complex Zs = ZR + ZL + ZC;
    cout << "Series Z   = " << Zs << "   |Z| = " << Zs.magnitude() << "\n";
    

    
    Complex Zlc = ZL + ZC;                       
    Complex Zp = (ZR * Zlc) / (ZR + Zlc);
    cout << "Parallel Z = " << Zp << "   |Z| = " << Zp.magnitude() << "\n";
    

    
    Complex zero;
    Complex bad = ZR / zero;
    cout << "Result after divide by zero: " << bad << "\n";

    cout << "Equality test: " << boolalpha << (Zp == Complex(50, -50)) << "\n";
    complex<double> s(R, 0), l(0, w * L), c(0, -1.0 / (w * C));
    complex<double> sp = (s * (l + c)) / (s + (l + c));
    cout << "std::complex parallel = " << sp << "  |Z| = " << abs(sp) << "\n";
    return 0;
}
