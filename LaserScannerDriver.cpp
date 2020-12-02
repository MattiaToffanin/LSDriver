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


Scan LaserScannerDriver::get_last_scan() const {
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


double Scan::get_distance_from_angle(double angle) const {
    int min = 0;
    for (int i = 0; i < length; ++i)
        if (abs(angle - i * angle_resolution) < abs(angle - min * angle_resolution))
            min = i;

    return scan[min];
}


double LaserScannerDriver::get_distance(double angle) const {
    return get_last_scan().get_distance_from_angle(angle);
}


Scan::Scan(const Scan &s)
        : length{s.length}, angle_resolution{s.angle_resolution}, scan{new double[s.length]} {
    copy(s.scan, s.scan + s.length, scan);
}


Scan &Scan::operator=(const Scan &s) {
    double *temp = new double[s.length];
    copy(s.scan, s.scan + s.length, temp);
    delete[] scan;
    scan = temp;
    length = s.length;
    angle_resolution = s.angle_resolution;
    return *this;
}


Scan::Scan(Scan &&s)
        : length{s.length}, angle_resolution{s.angle_resolution}, scan{s.scan} {
    s.length = 0;
    s.angle_resolution = 0;
    s.scan = nullptr;
}

Scan &Scan::operator=(Scan &&s) {
    delete[] scan;
    scan = s.scan;
    length = s.length;
    angle_resolution = s.angle_resolution;
    s.scan = nullptr;
    s.length = 0;
    s.angle_resolution = 0;
    return *this;
}

Scan::~Scan() {
    delete[] scan;
    scan = nullptr;
    length = 0;
    angle_resolution = 0;
}

void Scan::print() const {
    cout << "[";
    for (int i = 0; i < length; ++i) {
        cout << angle_resolution * i << ": " << scan[i];
        if (i != length - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}


LaserScannerDriver::~LaserScannerDriver() {
    delete[] buffer;
    buffer = nullptr;
    head = 0;
    tail = 0;
    size = 0;
}

LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver &l)
        : head{l.head}, tail{l.tail}, size{l.size}, buffer{new Scan[BUFFER_DIM]} {
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i] = l.buffer[i];
    }
    //copy(l.buffer, l.buffer + BUFFER_DIM, buffer);
}

LaserScannerDriver &LaserScannerDriver::operator=(const LaserScannerDriver &l) {
    Scan *temp = new Scan[BUFFER_DIM];
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i] = l.buffer[i];
    }
    //copy(l.buffer, l.buffer + BUFFER_DIM, temp);
    delete[] buffer;
    buffer = temp;
    head = l.head;
    tail = l.tail;
    size = l.size;
    return *this;
}

LaserScannerDriver::LaserScannerDriver(LaserScannerDriver &&l)
        : head{l.head}, tail{l.tail}, size{l.size}, buffer{l.buffer} {
    l.head = 0;
    l.tail = 0;
    l.size = 0;
    l.buffer = nullptr;
}

LaserScannerDriver &LaserScannerDriver::operator=(LaserScannerDriver &&l) {
    delete[] buffer;
    buffer = l.buffer;
    head = l.head;
    tail = l.tail;
    size = l.size;
    l.buffer = nullptr;
    l.head = 0;
    l.tail = 0;
    l.size = 0;
    return *this;
}

void LaserScannerDriver::print() const {
    cout << "head: " << head << ", tail: " << tail << ", size: " << size << endl;
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i].print();
    }
}



