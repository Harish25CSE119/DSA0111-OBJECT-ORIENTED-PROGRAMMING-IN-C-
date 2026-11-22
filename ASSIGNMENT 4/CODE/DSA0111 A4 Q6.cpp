
#include <iostream>
#include <string>
using namespace std;

class Vehicle {
    string reg;
    double dailyRate;
    bool rented = false;
protected:
    double late(int lateDays) const { return lateDays * dailyRate * 1.5; }
    double base(int days) const { return days * dailyRate; }
public:
    Vehicle(const string& r, double d) : reg(r), dailyRate(d) {}
    virtual ~Vehicle() = default;
    virtual double charge(int days, int lateDays, double used) const = 0;   
    virtual string type() const = 0;
    bool rent() {
        if (rented) { cout << reg << " is not available\n"; return false; }
        rented = true; cout << type() << " " << reg << " rented\n"; return true;
    }
    double giveBack(int days, int lateDays, double used) {
        if (!rented) { cout << reg << " was not rented\n"; return 0; }
        rented = false;
        double bill = charge(days, lateDays, used);
        cout << type() << " " << reg << " returned, bill Rs " << bill << "\n";
        return bill;
    }
    void list() const { cout << "  " << type() << " " << reg << (rented ? " [rented]" : " [free]") << "\n"; }
};

struct ServiceDate {                       
    int d, m, y;
    ServiceDate(int dd, int mm, int yy) : d(dd), m(mm), y(yy) {}
};

class Bicycle : public Vehicle {
public:
    Bicycle(const string& r, double d) : Vehicle(r, d) {}
    double charge(int days, int lateDays, double) const override { return base(days) + late(lateDays); }
    string type() const override { return "Bicycle"; }
};

class MotorVehicle : public Vehicle {
protected:
    double level;                          
    ServiceDate service;
public:
    MotorVehicle(const string& r, double d, double lv, ServiceDate s) : Vehicle(r, d), level(lv), service(s) {}
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const string& r, double d, double lv, ServiceDate s) : MotorVehicle(r, d, lv, s) {}
    double charge(int days, int lateDays, double litres) const override {
        return base(days) + litres * 100 + late(lateDays);
    }
    string type() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
protected:
    int seats;
public:
    Car(const string& r, double d, double lv, ServiceDate s, int st) : MotorVehicle(r, d, lv, s), seats(st) {}
    double charge(int days, int lateDays, double litres) const override {
        return base(days) + seats * 20.0 * days + litres * 100 + late(lateDays);
    }
    string type() const override { return "Car"; }
};

class ElectricCar : public Car {
    double batteryKWh;
public:
    ElectricCar(const string& r, double d, double lv, ServiceDate s, int st, double b)
        : Car(r, d, lv, s, st), batteryKWh(b) {}
    double charge(int days, int lateDays, double kWh) const override {   
        return base(days) + seats * 20.0 * days + kWh * 12 + late(lateDays);
    }
    string type() const override { return "ElectricCar"; }
};


class ElectricBus : public MotorVehicle {
    int seats;
public:
    ElectricBus(const string& r, double d, double lv, ServiceDate s, int st)
        : MotorVehicle(r, d, lv, s), seats(st) {}
    double charge(int days, int lateDays, double kWh) const override {
        return base(days) + kWh * 15 + late(lateDays) + seats;
    }
    string type() const override { return "ElectricBus"; }
};

int main() {
    const int N = 5;
    Vehicle* fleet[N] = {
        new Bicycle("BC-01", 100),
        new TwoWheeler("TW-01", 300, 8, ServiceDate(1, 3, 2027)),
        new Car("CR-01", 1500, 30, ServiceDate(5, 4, 2027), 5),
        new ElectricCar("EC-01", 2000, 80, ServiceDate(9, 5, 2027), 5, 40),
        new ElectricBus("EB-01", 6000, 90, ServiceDate(1, 6, 2027), 30)  
    };
    cout << "Fleet:\n";
    for (Vehicle* v : fleet) v->list();

    cout << "\nRenting:\n";
    for (Vehicle* v : fleet) v->rent();
    fleet[2]->rent();                                 

    cout << "\nReturning (2 days, 1 late day):\n";
    fleet[0]->giveBack(2, 1, 0);
    fleet[1]->giveBack(2, 1, 3);      
    fleet[2]->giveBack(2, 1, 10);     
    fleet[3]->giveBack(2, 1, 25);      
    fleet[4]->giveBack(2, 1, 60);      

    cout << "\nFleet after returns:\n";
    for (Vehicle* v : fleet) v->list();
    for (Vehicle* v : fleet) delete v;

    
    return 0;
}
