
#include <iostream>
using namespace std;

struct Parcel {
    double kg;
    bool express;
    bool insured;
};

double price(double kg, bool express, bool insured)
{
    double total = kg * 50;
    if (express) total = total + 40;
    if (insured) total = total + 20;
    return total;
}


double charge(int grams, bool express = false, bool insured = false)
{
    cout << "grams version, ";
    return price(grams / 1000.0, express, insured);
}

double charge(double kg, bool express = false, bool insured = false)
{
    cout << "kg version, ";
    return price(kg, express, insured);
}

double charge(Parcel p)
{
    cout << "Parcel version, ";
    return price(p.kg, p.express, p.insured);
}

int main()
{

    Parcel p;
    p.kg = 3;
    p.express = true;
    p.insured = true;

    double c;

    cout << "charge(500)       : ";
    c = charge(500);
    cout << "cost = " << c << endl;

    cout << "charge(2)         : ";
    c = charge(2);
    cout << "cost = " << c << endl;

    cout << "charge(2.0f)      : ";
    c = charge(2.0f);
    cout << "cost = " << c << endl;

    cout << "charge(2.5)       : ";
    c = charge(2.5);
    cout << "cost = " << c << endl;

    cout << "charge(1.5, true) : ";
    c = charge(1.5, true);
    cout << "cost = " << c << endl;

    cout << "charge(p)         : ";
    c = charge(p);
    cout << "cost = " << c << endl;

    return 0;
}
