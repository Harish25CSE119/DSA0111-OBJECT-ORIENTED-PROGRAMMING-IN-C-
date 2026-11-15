
#include <iostream>
#include <string>
using namespace std;

class BusTicket {
    static int issued;             
    static const int CAPACITY = 3;
    int seat;
    string name;
public:
    BusTicket(string n)
    {
        name = n;
        seat = 0;
        
        if (issued < CAPACITY) {
            issued++;
            seat = issued;
        }
    }

    static int count()
    {
        
        return issued;
    }

    void print() const
    {
        if (seat != 0) cout << name << ": Seat " << seat << endl;
        else           cout << name << ": Waiting" << endl;
    }
};

int BusTicket::issued = 0;          
void makeTickets()
{
    BusTicket a("Esha");
    BusTicket b("Farid");
    a.print();
    b.print();
}

int main()
{
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};

    for (int i = 0; i < 4; i++) trip[i].print();
    cout << "Issued: " << BusTicket::count() << endl;

   
    makeTickets();
    makeTickets();
    cout << "Issued after two calls: " << BusTicket::count() << endl;

    return 0;
}
