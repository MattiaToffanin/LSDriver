#include <iostream>
#include <vector>
#include "LaserScannerDriver.h"

using namespace std;

void print(const vector<double> &v) {
    cout << "[";
    for (int i = 0; i < v.size(); ++i) {
        cout << i << ": " << v[i];
        if (i != v.size() - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}


vector<double> createRandomVector(const double angleResolution = 1) {
    vector<double> v;
    for (int i = 0; i < (180 / angleResolution) + 1; ++i)
        v.push_back(random());
    return v;
}

LaserScannerDriver createLaserScannerDriver(const double angleResolution = 1) {
    LaserScannerDriver l(angleResolution);
    for (int i = 0; i < 3; ++i)
        l.new_scan(createRandomVector(angleResolution));
    return l; //Test move di LaserScannerDriver
}


int main() {

    LaserScannerDriver l1 = LaserScannerDriver();

    cout << "l1: ";
    l1.print();

    l1.new_scan(createRandomVector());
    l1.new_scan(createRandomVector());
    l1.new_scan(createRandomVector());
    cout << endl << "l1: ";
    l1.print();

    l1.new_scan(createRandomVector());
    cout << endl << "l1: ";
    l1.print();

    cout << endl << "Older scan ";
    print(l1.get_scan());
    cout << "Older scan ";
    print(l1.get_scan());
    cout << "l1: ";
    l1.print();

    l1.clear_buffer();
    cout << endl << "l1 cleaned: ";
    l1.print();

    l1.new_scan(createRandomVector());
    l1.new_scan(createRandomVector());
    cout << endl << "l1: ";
    l1.print();
    cout << "Newer scan " << l1 << endl;

    double angle = 7.1223;
    cout << "Distance of " << angle << " in newer scan: " << l1.get_distance(angle) << endl;

    angle = 190.4456;
    cout << "Distance of " << angle << " in newer scan: " << l1.get_distance(angle) << endl;

    LaserScannerDriver l2 = createLaserScannerDriver(19.67);
    cout << endl << "l2: ";
    l2.print();


    l2 = l1;
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 = l1: ";
    l2.print();

    l1.new_scan(createRandomVector());
    l1.new_scan(createRandomVector());
    l1.new_scan(createRandomVector());
    l2.new_scan(createRandomVector());
    l2.new_scan(createRandomVector());
    l2.new_scan(createRandomVector());
    l2.new_scan(createRandomVector());
    l2.new_scan(createRandomVector());
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    cout << endl << "Older scan of l1 ";
    print(l1.get_scan());
    cout << "Older scan of l2 ";
    print(l2.get_scan());
    l1.get_scan();
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    l1 = l2;
    cout << endl << "l1 = l2: ";
    l1.print();
    cout << "l2: ";
    l2.print();

    return 0;
}
