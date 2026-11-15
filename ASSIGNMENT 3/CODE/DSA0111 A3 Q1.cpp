
#include <iostream>
#include <string>
using namespace std;

class Book {
    string title;
    string author;
    int copies;   

public:
    
    Book(const string& t, const string& a, int c)
        : title(t), author(a), copies(c < 0 ? 0 : c) {
        if (c < 0) cout << "  [Warning] negative copies (" << c << ") -> set to 0\n";
    }

    
    explicit Book(const string& t) : Book(t, "Unknown", 1) {}

    
    Book() : Book("Untitled", "Unknown", 0) {}

    void print() const {
        cout << "  Title: " << title << " | Author: " << author
             << " | Copies: " << copies << "\n";
    }
};

void show(Book b) { b.print(); }

int main() {
    cout << "Default constructor:\n";
    Book b1;
    b1.print();

    cout << "Title-only (donated) constructor:\n";
    Book b2("Wings of Fire");
    b2.print();

    cout << "Full constructor:\n";
    Book b3("C++ Primer", "Lippman", 5);
    b3.print();

    cout << "Negative copies test:\n";
    Book b4("Bad Book", "Nobody", -7);
    b4.print();

    
    show(Book("Gitanjali"));

    
    Book arr[2];
    arr[0].print();

    
    return 0;
}
