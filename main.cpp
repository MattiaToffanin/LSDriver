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
        v.push_back(rand());
    return v;
}

LaserScannerDriver createLaserScannerDriver(const double angleResolution = 1) {
    LaserScannerDriver l(angleResolution);
    for (int i = 0; i < 3; ++i)
        l.new_scan(createRandomVector(angleResolution));
    return l; //Test move di LaserScannerDriver
}


int main() {

    double al1 = 0.7;
    double al2 = 0.2;
    LaserScannerDriver l1 = LaserScannerDriver(al1);

    cout << "l1: ";
    l1.print();

    l1.new_scan(createRandomVector(al1));
    cout << endl << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    cout << endl << "l1: ";
    l1.print();

    l1.new_scan(createRandomVector(al1));
    cout << endl << "Newer scan of l1 " << l1;

    cout << endl << "l1: ";
    l1.print();

    cout << endl << "Older scan of l1 ";
    print(l1.get_scan());
    cout << "Older scan of l1 ";
    print(l1.get_scan());
    cout << endl << "l1: ";
    l1.print();

    l1.clear_buffer();
    cout << endl << "l1 cleaned: ";
    l1.print();

    l1.new_scan(createRandomVector(al1));
    cout << endl << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    cout << endl << "l1: ";
    l1.print();
    cout << endl << "Newer scan of l1 " << l1 << endl;

    double angle = 7.1223;
    cout << "Distance of " << angle << " in newer scan: " << l1.get_distance(angle) << endl;

    angle = 190.4456;
    cout << "Distance of " << angle << " in newer scan: " << l1.get_distance(angle) << endl;

    angle = -2;
    cout << "Distance of " << angle << " in newer scan: " << l1.get_distance(angle) << endl;

    LaserScannerDriver l2 = createLaserScannerDriver(0.23);
    cout << endl << "l2: ";
    l2.print();

    l2 = l1;
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 = l1: ";
    l2.print();

    l1.new_scan(createRandomVector(al1));
    cout << endl << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    cout << endl << "Older scan of l1 ";
    print(l1.get_scan());
    cout << "Older scan of l1 ";
    print(l1.get_scan());
    cout << "Older scan of l2 ";
    print(l2.get_scan());
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    l1 = l2;
    cout << endl << "l1 = l2: ";
    l1.print();
    cout << "l2: ";
    l2.print();

    cout << endl << "Older scan of l1 ";
    print(l1.get_scan());
    cout << "Older scan of l2 ";
    print(l2.get_scan());
    cout << "Older scan of l2 ";
    print(l2.get_scan());
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    l1.new_scan(createRandomVector(al1));
    cout << endl << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    l1.new_scan(createRandomVector(al1));
    cout << "Newer scan of l1 " << l1;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    l2.new_scan(createRandomVector(al2));
    cout << "Newer scan of l2 " << l2;
    cout << endl << "l1: ";
    l1.print();
    cout << "l2 != l1: ";
    l2.print();

    cout << rand();

    return 0;
}
