#include <iostream>
#include <vector>
#include "LaserScannerDriver.h"

using namespace std;

LaserScannerDriver::LaserScannerDriver(double angleResolution)
        : angle_resolution{angleResolution}, buffer{new Scan[BUFFER_DIM]}, head{0}, tail{0}, size{0} {
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i] = Scan(angleResolution);
    }
}

Scan::Scan(double AngleResolution) {
    length = 180 / AngleResolution;
    scan = nullptr;
}


void LaserScannerDriver::new_scan(const vector<double> &v) {
    buffer[tail].insert_vector(v);
    if (size == BUFFER_DIM)
        head = increment(head);
    tail = increment(tail);
    if (size < BUFFER_DIM)
        size++;
}

void Scan::insert_vector(const vector<double> &v) {
    delete[] scan;
    scan = new double[length];
    for (int i = 0; i < length; ++i)
        if (i >= v.size())
            scan[i] = 0;
        else
            scan[i] = v[i];
}


vector<double> LaserScannerDriver::get_scan() {
    if (size == 0)
        throw EmptyBuffer{};
    vector<double> v = buffer[head].get_vector();
    head = increment(head);
    size--;
    return v;
}

vector<double> Scan::get_vector() {
    vector<double> v;
    v.reserve(length);
    for (int i = 0; i < length; ++i)
        v.push_back(scan[i]);
    delete[] scan;
    scan = nullptr;
    return v;
}


void LaserScannerDriver::clear_buffer() {
    head = 0;
    tail = 0;
    size = 0;
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i].clear_scan();
    }
}


void Scan::clear_scan() {
    delete[] scan;
    scan = nullptr;
}


double Scan::operator[](int n) const {
    return scan[n];
}


const Scan &LaserScannerDriver::get_last() const {
    return buffer[decrement(tail)];
}

ostream &operator<<(ostream &os, const LaserScannerDriver &driver) {
    return os << driver.get_last();
}

ostream &operator<<(ostream &os, const Scan &scan) {
    if (scan.is_valid()) {
        for (int i = 0; i < scan.get_length(); ++i) {
            os << scan[i];
            if (i != scan[i] - 1)
                os << ", ";
        }
    } else {
        throw Scan::ScanNotValid{};
    }
    os << endl;
    return os;
}
