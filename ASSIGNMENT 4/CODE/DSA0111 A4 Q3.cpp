
#include <iostream>
#include <string>
using namespace std;
#ifdef NOVIRTUAL
#define VIRT
#else
#define VIRT virtual
#endif


namespace trace {
struct Address {                      
    string city; int pin;
    Address(const string& c, int p) : city(c), pin(p) { cout << "Address "; }
    ~Address() { cout << "~Address "; }
};
struct Course {                       
    string title;
    Course(const string& t) : title(t) { cout << "Course "; }
    ~Course() { cout << "~Course "; }
};
struct Person {
    string name; Address addr;
    Person(const string& n, const string& c, int p) : name(n), addr(c, p) { cout << "Person "; }
    ~Person() { cout << "~Person "; }
};
struct Student : VIRT Person {
    Course course;
    Student(const string& n, const string& c, int p, const string& crs)
        : Person(n, c, p), course(crs) { cout << "Student "; }
    ~Student() { cout << "~Student "; }
};
struct Staff : VIRT Person {
    string dept;
    Staff(const string& n, const string& c, int p, const string& d)
        : Person(n, c, p), dept(d) { cout << "Staff "; }
    ~Staff() { cout << "~Staff "; }
};
struct TeachingAssistant : Student, Staff {
    TeachingAssistant(const string& n, const string& c, int p, const string& crs, const string& d)
#ifdef NOVIRTUAL
        
        : Student(n, c, p, crs), Staff(n, c, p, d) { cout << "TA "; }
#else
        : Person(n, c, p), Student(n, c, p, crs), Staff(n, c, p, d) { cout << "TA "; }
#endif
    ~TeachingAssistant() { cout << "~TA "; }
};
}


namespace predict {
struct Engine {
    Engine() { cout << "Engine "; }
    ~Engine() { cout << "~Engine "; }
};
struct Vehicle {
    Vehicle() { cout << "Vehicle "; }
    ~Vehicle() { cout << "~Vehicle "; }
};
struct Car : Vehicle {
    Engine e;
    Car() { cout << "Car "; }
    ~Car() { cout << "~Car "; }
};
}


namespace complete {
class Person {
protected:
    string name;
    int age;
public:
    Person(const string& n, int a) : name(n), age(a) {}
};
class Address {   
    string city;
    int pin;
public:
    Address(const string& c, int p) : city(c), pin(p) {}
    string str() const { return city + "-" + to_string(pin); }
};
class Student : public Person {
    long long regNo;
    Address home;
public:
    
    Student(const string& n, int a, long long r, const string& city, int pin)
        : Person(n, a), regNo(r), home(city, pin) {}

    void print() const
    {
        cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};
}

int main() {
    cout << "--- Part A: construction order ---\n";
    cout << "Student:  ";  { trace::Student s("Kavya", "Salem", 636001, "C++"); }  cout << "\n";
    cout << "Staff:    ";  { trace::Staff f("Raj", "Chennai", 600001, "CSE"); }    cout << "\n";
    cout << "TA:       ";  { trace::TeachingAssistant t("Asha", "Madurai", 625001, "DSA", "CSE"); } cout << "\n";
    cout << "sizeof(TeachingAssistant) = " << sizeof(trace::TeachingAssistant) << "\n";

    cout << "--- Part B: predict ---\n";
    {
        predict::Car c;
        cout << "| ";
    }
    cout << '\n';

    cout << "--- Part C: complete the code ---\n";
    complete::Student s("Kavya", 19, 192311001, "Salem", 636001);
    s.print();
    return 0;
}
