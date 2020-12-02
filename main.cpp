#include <iostream>
#include "LaserScannerDriver.h"

using namespace std;

void print(const vector<double> &v) {
    cout << "[";
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i != v.size() - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}


int main() {

    /*LaserScannerDriver l1(18);
    l1.new_scan({1});
    l1.new_scan({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
    cout << l1;

    cout << l1.get_distance(19.45) << endl;

    LaserScannerDriver l1 = l1;
    l1.new_scan({34});
    print(l2.get_scan());


    Scan s1 = Scan();
    s1.insert_vector({32423,3,4,5});

    Scan s2 = s1;
    cout << s1;
    cout << s2;

    s2.insert_vector({12,1232,43});
    cout << s1;
    cout << s2;
*/


    LaserScannerDriver l1 = LaserScannerDriver(18);
    l1.new_scan({1});
    l1.new_scan({2});
    l1.new_scan({3});
    LaserScannerDriver l2 = l1;
    cout << "l2: " << l2;
    cout << "l1: " << l1;

    l1.new_scan({4});
    l1.new_scan({5});
    l1.new_scan({6});
    l1.new_scan({7});
    cout << "l2: " << l2;
    cout << "l1: " << l1;


    return 0;
}
