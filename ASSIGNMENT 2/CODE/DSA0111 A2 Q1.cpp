
#include <iostream>
#include <string>
using namespace std;

void applyByValue(long long fee, int percent)
{
    fee = fee - fee * percent / 100;
    cout << "  inside by value     : " << fee << endl;
}

void applyByRef(long long &fee, int percent)
{
    fee = fee - fee * percent / 100;
    cout << "  inside by reference : " << fee << endl;
}

void applyByPointer(long long *fee, int percent)
{
    *fee = *fee - *fee * percent / 100;
    cout << "  inside by pointer   : " << *fee << endl;
}

long long newFee(long long fee, int percent)
{
    return fee - fee * percent / 100;
}

struct Student {
    string name;
    long long fee;
};

void printStudent(const Student &s)
{
    cout << "  " << s.name << " owes " << s.fee << endl;
}

int main()
{
    

    long long fee = 45000;

    applyByValue(fee, 10);
    cout << "after by value     : " << fee << endl;

    applyByRef(fee, 10);
    cout << "after by reference : " << fee << endl;

    applyByPointer(&fee, 10);
    cout << "after by pointer   : " << fee << endl;

    long long f = 45000;
    f = newFee(f, 10);
    f = newFee(f, 15);
    f = newFee(f, 25);
    cout << "after 3 concessions (returned value): " << f << endl;

    Student s;
    s.name = "Asha";
    s.fee = 45000;
    printStudent(s);

    return 0;
}
