
#include <iostream>
#include <ctime>
using namespace std;

const int MAX = 50;                 
const int ADDS = 1000000;

struct Transaction {
    char type;
    double amount;
};


class ShiftLog {
public:
    Transaction data[MAX];
    int count;
    long long copies;

    ShiftLog() { count = 0; copies = 0; }

    void add(char type, double amount)
    {
        if (count == MAX) {
            for (int i = 1; i < MAX; i++) {
                data[i - 1] = data[i];
                copies++;
            }
            count--;
        }
        data[count].type = type;
        data[count].amount = amount;
        count++;
        copies++;
    }

    Transaction get(int i) { return data[i]; }
};


class CircularLog {
public:
    Transaction data[MAX];
    int start;          
    int count;
    long long copies;

    CircularLog() { start = 0; count = 0; copies = 0; }

    void add(char type, double amount)
    {
        if (count < MAX) {
            int pos = (start + count) % MAX;
            data[pos].type = type;
            data[pos].amount = amount;
            count++;
        } else {
            data[start].type = type;        
            data[start].amount = amount;
            start = (start + 1) % MAX;
        }
        copies++;
    }

    Transaction get(int i) { return data[(start + i) % MAX]; }
};

int main()
{
    ShiftLog oldLog;
    CircularLog newLog;
    clock_t begin;
    double msOld, msNew;

    begin = clock();
    for (int i = 0; i < ADDS; i++) oldLog.add('D', i);
    msOld = (clock() - begin) * 1000.0 / CLOCKS_PER_SEC;

    begin = clock();
    for (int i = 0; i < ADDS; i++) newLog.add('D', i);
    msNew = (clock() - begin) * 1000.0 / CLOCKS_PER_SEC;

    
    double sumOld = 0, sumNew = 0;
    for (int i = 0; i < MAX; i++) {
        sumOld += oldLog.get(i).amount;
        sumNew += newLog.get(i).amount;
    }

    cout << "MAX = " << MAX << ", transactions added = " << ADDS << endl;
    cout << "Original (shifting): " << msOld << " ms, copies per add = "
         << (double)oldLog.copies / ADDS << endl;
    cout << "Redesign (circular): " << msNew << " ms, copies per add = "
         << (double)newLog.copies / ADDS << endl;
    cout << "Same contents? " << (sumOld == sumNew ? "yes" : "NO") << endl;

    
    return 0;
}
