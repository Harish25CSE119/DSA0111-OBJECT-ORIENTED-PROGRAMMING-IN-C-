
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; 
public:
    
    Cart& add(const std::string& item, double price)
    {
        if (n < 10) { items[n] = item; prices[n] = price; ++n; }
        return *this;
    }
   
    Cart& setDiscount(double discount)
    {
        this->discount = discount;
        return *this;
    }
    double total() const
    {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
};


int main()
{
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);   
    Cart* p = &c;                                         
    p->add("Oil", 180);
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';   

    Cart& r = c;                                          
    r.add("Salt", 20);
    std::cout << "via reference: " << r.total() << '\n';

    Cart* h = new Cart;                                   
    h->add("Milk", 50).setDiscount(0);
    std::cout << "heap cart: " << h->total() << '\n';
    delete h;                                             
    Cart c2, c3;
    c2.add("TV", 5000);
    c3.add("Pen", 10);
    Cart* carts[3] = { &c, &c2, &c3 };
    Cart* best = carts[0];
    for (int i = 1; i < 3; ++i)
        if (carts[i]->total() > best->total()) best = carts[i];
    std::cout << "highest total: " << best->total() << '\n';

    
    std::vector<Cart> vc{c, c2, c3};
    double mx = 0;
    for (const Cart& x : vc) if (x.total() > mx) mx = x.total();
    std::cout << "vector max: " << mx << '\n';
    return 0;
}
