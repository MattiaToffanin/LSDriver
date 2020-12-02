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
    LaserScannerDriver l1(0.33);
    l1.new_scan({1});
    l1.new_scan({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
    cout << l1;

    cout << l1.get_distance(0.45);
    return 0;
}
