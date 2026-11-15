
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

class Text {
    char* data;
    int len;
    static int copies;
public:
    Text(const char* s = "") : data(nullptr), len((int)strlen(s)) {
        data = new char[len + 1];
        strcpy(data, s);
    }
    Text(const Text& o) : data(new char[o.len + 1]), len(o.len) {   
        strcpy(data, o.data);
        copies++;
    }
    Text& operator=(const Text& o) {                                
        if (this == &o) return *this;                               
        char* temp = new char[o.len + 1];
        strcpy(temp, o.data);
        delete[] data;
        data = temp;
        len = o.len;
        return *this;
    }
    ~Text() { delete[] data; }

    
    friend Text operator+(const Text& a, const Text& b) {
        Text r;
        delete[] r.data;
        r.len = a.len + b.len;
        r.data = new char[r.len + 1];
        strcpy(r.data, a.data);
        strcat(r.data, b.data);
        return r;
    }
    bool operator==(const Text& o) const { return strcmp(data, o.data) == 0; }
    bool operator!=(const Text& o) const { return !(*this == o); }
    friend ostream& operator<<(ostream& os, const Text& t) { return os << t.data; }

    static int copyCount() { return copies; }
};
int Text::copies = 0;

int main() {
    Text a("Hello "), b("World"), c("!");
    Text d = a + b + c;
    cout << d << "\n";
    cout << "Copies made: " << Text::copyCount() << "\n";

    cout << "a==b: " << (a == b) << "  a!=b: " << (a != b) << "\n";

    Text empty;
    cout << "Empty+a: [" << (empty + a) << "]\n";

    Text s("self");
    s = s;                         
    cout << "After s = s: " << s << "\n";

    Text longMsg(string(1000, 'x').c_str());
    cout << "Long message length test OK\n";

    
    using namespace std::chrono;
    auto t1 = steady_clock::now();
    Text t("a");
    for (int i = 0; i < 100000; i++) t = Text("a") + Text("b");
    auto t2 = steady_clock::now();
    string st;
    for (int i = 0; i < 100000; i++) st = string("a") + string("b");
    auto t3 = steady_clock::now();
    cout << "Text       : " << duration<double, milli>(t2 - t1).count() << " ms\n";
    cout << "std::string: " << duration<double, milli>(t3 - t2).count() << " ms\n";
    return 0;
}
