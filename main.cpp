#include <iostream>
#include "LaserScannerDriver.h"

using namespace std;

void print(const vector<double> &v) {
    cout << "v: ";
    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << ", ";
    cout << endl;
}


int main() {
    LaserScannerDriver l1 = LaserScannerDriver(18);
    l1.new_scan({1});
    l1.new_scan({2});
    cout << l1;

    vector<double> v1 = l1.get_scan();
    print(v1);
    l1.new_scan({3});
    cout << l1;

    l1.new_scan({4});
    v1 = l1.get_scan();
    print(v1);
    l1.new_scan({5});
    l1.new_scan({6});
    v1 = l1.get_scan();
    print(v1);
    v1 = l1.get_scan();
    print(v1);



    l1.clear_buffer();

    l1.new_scan({1});
    l1.new_scan({2});
    l1.new_scan({3});
    v1 = l1.get_scan();
    print(v1);
    l1.new_scan({4});
    v1 = l1.get_scan();
    v1 = l1.get_scan();
    v1 = l1.get_scan();
    print(v1);


    cout << l1;

    return 0;
}
