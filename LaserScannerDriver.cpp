#include <iostream>
#include <vector>
#include <cmath>
#include "LaserScannerDriver.h"

using namespace std;

LaserScannerDriver::LaserScannerDriver(double angleResolution)
        : buffer{new Scan[BUFFER_DIM]}, head{0}, tail{0}, size{0} {
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i] = Scan(angleResolution);
    }
}

Scan::Scan(double AngleResolution) {
    angle_resolution = AngleResolution;
    length = floor(180 / AngleResolution) + 1;
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


double Scan::get_angle_resolution() const {
    return angle_resolution;
}


Scan &LaserScannerDriver::get_last_scan() const {
    return buffer[decrement(tail)];
}

ostream &operator<<(ostream &os, const LaserScannerDriver &driver) {
    return os << driver.get_last_scan();
}


ostream &operator<<(ostream &os, const Scan &scan) {
    double ar = scan.get_angle_resolution();

    if (scan.is_valid()) {
        os << "[";
        for (int i = 0; i < scan.get_length(); ++i) {
            os << ar * i << ": " << scan[i];
            if (i != scan.get_length() - 1)
                os << ", ";
        }
        os << "]" << endl;
    } else {
        throw Scan::ScanNotValid{};
    }
    return os;
}


double Scan::get_distance_from_angle(const double angle) const {
    int min = 0;
    for (int i = 0; i < length; ++i)
        if (abs(angle - i * angle_resolution) < abs(angle -  min * angle_resolution))
            min = i;

    return scan[min];
}


double LaserScannerDriver::get_distance(const double angle) const {
    return get_last_scan().get_distance_from_angle(angle);
}