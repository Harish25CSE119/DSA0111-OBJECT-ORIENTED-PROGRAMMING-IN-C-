
#include <iostream>
#include <string>
using namespace std;

class Event {
    string name;
    int rows, seats;
    char** map;      
    int booked;

public:
    
    Event(const string& n, int r, int s) : name(n), rows(r), seats(s), map(nullptr), booked(0) {
        map = new char*[rows];
        for (int i = 0; i < rows; i++) {
            map[i] = new char[seats];
            for (int j = 0; j < seats; j++) map[i][j] = 'O';   
        }
        cout << "+ Event " << name << " created (" << rows << "x" << seats << ")\n";
    }

    
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    bool book(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'X') return false;
        map[r][s] = 'X'; booked++; return true;
    }
    bool cancel(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'O') return false;
        map[r][s] = 'O'; booked--; return true;
    }

    
    ~Event() {
        for (int i = 0; i < rows; i++) delete[] map[i];   
        delete[] map;
        cout << "- Event " << name << " closed. Booked seats at close: " << booked
             << " of " << rows * seats << "\n";
    }

    
    size_t bytes() const { return rows * sizeof(char*) + (size_t)rows * seats; }
};

int main() {
    Event e1("Stack Show", 5, 10);              
    e1.book(0, 0); e1.book(0, 1); e1.cancel(0, 1);

    {
        Event e2("Block Show", 3, 4);          
        e2.book(1, 1);
        cout << "leaving block\n";
    }                                          

    Event* e3 = new Event("Heap Show", 4, 4);   
    e3->book(2, 2);
    delete e3;                                 

    Event zero("Zero Rows", 0, 10);            
    cout << "Memory for 10x20: " << Event("m1", 10, 20).bytes() << " bytes\n";
    cout << "Memory for 100x200: " << Event("m2", 100, 200).bytes() << " bytes\n";
    cout << "Memory for 1000x2000: " << Event("m3", 1000, 2000).bytes() << " bytes\n";

    cout << "end of main\n";
    return 0;   
}
