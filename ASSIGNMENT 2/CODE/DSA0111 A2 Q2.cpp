
#include <iostream>
#include <vector>
using namespace std;

int& highest(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; i++)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

const int& highestConst(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; i++)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

int highestCopy(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; i++)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}


int highestIndex(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; i++)
        if (marks[i] > marks[best]) best = i;
    return best;
}


int* highestPointer(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; i++)
        if (marks[i] > marks[best]) best = i;
    return &marks[best];
}

#ifdef SHOW_DANGLING

int& badFunction()
{
    int local = 42;
    return local;   
}
#endif

void printMarks(int marks[], int n)
{
    for (int i = 0; i < n; i++) cout << marks[i] << " ";
    cout << endl;
}

int main()
{
    int marks[5] = {56, 78, 91, 67, 85};

    cout << "Original      : ";
    printMarks(marks, 5);

    highest(marks, 5) += 2;          
    cout << "After +2      : ";
    printMarks(marks, 5);

    highest(marks, 5) = 95;         
    cout << "After = 95    : ";
    printMarks(marks, 5);

    int copy = highestCopy(marks, 5);
    copy = 0;                        
    const int &r = highestConst(marks, 5);
    // r = 1;                        
    cout << "copy = " << copy << ", const reference sees " << r << endl;

    int index = highestIndex(marks, 5);
    marks[index] = marks[index] + 1; 
    int *p = highestPointer(marks, 5);
    *p = *p + 1;                     
    cout << "After index/pointer +1 each: ";
    printMarks(marks, 5);

#ifdef SHOW_DANGLING
    int &bad = badFunction();
    cout << "bad = " << bad << " (undefined behaviour)" << endl;
#endif

   
    vector<int> v;
    v.push_back(56);
    v.push_back(78);
    v.push_back(91);

    int *before = &v[2];            
    for (int i = 0; i < 20; i++) v.push_back(i);  

    if (before != &v[2])
        cout << "The vector moved to new memory. Old reference/pointer is now INVALID." << endl;
    else
        cout << "The vector did not move this time." << endl;

    
    v[2] = v[2] + 2;
    cout << "Using the index: v[2] = " << v[2] << endl;

    return 0;
}
