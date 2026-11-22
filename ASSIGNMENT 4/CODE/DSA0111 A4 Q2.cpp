
#include <iostream>
#include <string>
using namespace std;
#ifdef NOVIRTUAL
#define VIRT
#else
#define VIRT virtual
#endif


namespace design {
class Person {
protected:
    string name; int id; string phone;
public:
    Person(const string& n, int i, const string& p) : name(n), id(i), phone(p) {}
};
class Student : virtual public Person {            
protected:
    string programme; double cgpa;
public:
    Student(const string& n, int i, const string& p, const string& prog, double c)
        : Person(n, i, p), programme(prog), cgpa(c) {}
};
class Staff : virtual public Person {
protected:
    string dept; double salary;
public:
    Staff(const string& n, int i, const string& p, const string& d, double s)
        : Person(n, i, p), dept(d), salary(s) {}
};
class TeachingAssistant : public Student, public Staff {
public:
    
    TeachingAssistant(const string& n, int i, const string& p, const string& prog, double c,
                      const string& d, double s)
        : Person(n, i, p), Student(n, i, p, prog, c), Staff(n, i, p, d, s) {}
    void print() const {
        cout << name << " (" << id << ", " << phone << ") " << programme << " CGPA " << cgpa
             << " | " << dept << " Rs " << salary << "\n";  
    }
};
class ResearchScholar : public Student {
    double fellowship;
public:
    ResearchScholar(const string& n, int i, const string& p, const string& prog, double c, double f)
        : Person(n, i, p), Student(n, i, p, prog, c), fellowship(f) {}
    double fellow() const { return fellowship; }
};


struct PN { string name; };
struct StudentN : PN { int a; };
struct StaffN : PN { int b; };
struct TAN : StudentN, StaffN {};



struct TAComp { StudentN s; StaffN t; };
}


namespace predict {
struct Person { Person() { cout << "Person "; } };
struct Student : VIRT Person { Student() { cout << "Student "; } };
struct Staff : VIRT Person { Staff() { cout << "Staff "; } };
struct TeachingAssistant : Student, Staff {
    TeachingAssistant() { cout << "TA "; }
};
}

int main() {
    cout << "--- Part A: design ---\n";
    design::TeachingAssistant ta("Asha", 7, "98765", "MTech", 8.9, "CSE", 25000);
    ta.print();
    design::ResearchScholar rs("Vikram", 8, "91234", "PhD", 9.2, 30000);
    cout << "Fellowship: " << rs.fellow() << "\n";
    cout << "sizeof(TeachingAssistant, virtual) = " << sizeof(design::TeachingAssistant) << "\n";
    cout << "sizeof(TAN, non-virtual)           = " << sizeof(design::TAN) << "\n";
    cout << "sizeof(TAComp, composition)        = " << sizeof(design::TAComp) << "\n";
    design::TAN t;
    t.StudentN::name = "x";          
    t.StaffN::name = "y";
    cout << t.StudentN::name << t.StaffN::name << "\n";

    cout << "--- Part B: predict ---\n";
    predict::TeachingAssistant p;
    cout << '\n';
    return 0;
}
