
#include <iostream>
#include <string>
using namespace std;

class Prescription {
    int n;
    string* medicines;  
    static int copyCount;

public:
    explicit Prescription(int count) : n(count), medicines(new string[count]) {}

#ifndef SHALLOW
   
    Prescription(const Prescription& other)
        : n(other.n), medicines(new string[other.n]) {
        copyCount++;
        cout << "  [copy ctor called]\n";
        for (int i = 0; i < n; i++) medicines[i] = other.medicines[i];
    }

    
    Prescription& operator=(const Prescription& other) {
        if (this == &other) return *this;           
        string* temp = new string[other.n];         
        for (int i = 0; i < other.n; i++) temp[i] = other.medicines[i];
        delete[] medicines;                         
        medicines = temp;
        n = other.n;
        return *this;
    }

#endif
   
    ~Prescription() { delete[] medicines; }

    void set(int i, const string& m) { medicines[i] = m; }
    string get(int i) const { return medicines[i]; }
    static int copies() { return copyCount; }
};
int Prescription::copyCount = 0;

void byValue(Prescription p) { (void)p; }
void byRef(const Prescription& p) { (void)p; }

int main() {
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");
    Prescription q = p;          
    q.set(1, "Cetirizine");
    Prescription r(1);
    r = p;                      
    p.set(0, "Ibuprofen");
    cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
         << " | " << r.get(0) << ' ' << r.get(1) << '\n';
   

#ifndef SHALLOW
    cout << "Passing by value:\n";
    byValue(p);
    cout << "Passing by const reference:\n";
    byRef(p);
    cout << "Total copies made: " << Prescription::copies() << "\n";

    p = p;  
    cout << "After p = p: " << p.get(0) << ' ' << p.get(1) << "\n";
#endif
    return 0;
}
