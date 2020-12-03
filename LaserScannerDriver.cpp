#include <iostream>
#include <vector>
#include <cmath>
#include "LaserScannerDriver.h"

using namespace std;

LaserScannerDriver::LaserScannerDriver(double angleResolution) //Inizializzo le variabili membro
        : buffer{new Scan[BUFFER_DIM]}, head{0}, tail{0}, size{0} {
    for (int i = 0; i < BUFFER_DIM; ++i)
        buffer[i] = Scan(angleResolution); //Imposto la risoluzione angolare per ogni Scansione nel buffer
}

Scan::Scan(double angleResolution) //Inizializzo le variabili membro (scan = nullptr)
        : scan{nullptr}, angle_resolution{angleResolution} {
    length = std::floor(180 / angleResolution) + 1;
};

void LaserScannerDriver::new_scan(const vector<double> &v) {
    buffer[tail].insert_vector(v); //Inserisco scansione in coda
    if (size == BUFFER_DIM)
        head = increment(head); //Incremento head solo se il buffer è pieno
    tail = increment(tail); //Incremento sempre tail
    if (size < BUFFER_DIM)
        size++; //Incremento size solo se il buffer non è pieno
}

void Scan::insert_vector(const vector<double> &v) {
    delete[] scan; //Dealloco scan (ok anche nel caso nullptr)
    scan = new double[length]; //Alloco un nuovo array di double (letture)
    for (int i = 0; i < length; ++i) //Tronca se supero 180 gradi
        if (i >= v.size())
            scan[i] = 0; //Assegna 0 se mancano dati
        else
            scan[i] = v[i]; //Copio il vettore con la scansione (
}

vector<double> LaserScannerDriver::get_scan() {
    if (size == 0)
        throw EmptyBuffer{}; //Eccezione se buffer vuoto
    vector<double> v = buffer[head].get_vector(); //Rimuovo scansione in testa
    head = increment(head); //Incremento sempre head
    size--; //Decremento size
    return v; //Sfrutto move di vector
}

vector<double> Scan::get_vector() {
    vector<double> v; //Creo un vettore
    v.reserve(length); //Riservo spazio al vettore
    for (int i = 0; i < length; ++i)
        v.push_back(scan[i]); //Inserisco nel vettore ogni lettura della scansione
    delete[] scan; //Dealloco scan (ok anche se nullptr (anche se non dovrebbe mai esserlo))
    scan = nullptr;
    return v; //Sfrutto move di vector
}

void LaserScannerDriver::clear_buffer() {
    head = 0;
    tail = 0;
    size = 0;
    for (int i = 0; i < BUFFER_DIM; ++i)
        buffer[i].clear_scan(); //Pulisco ogni scansione del buffer
}

void Scan::clear_scan() {
    delete[] scan; //Dealloco scan (ok anche se nullptr)
    scan = nullptr;
}

Scan LaserScannerDriver::get_last_scan() const {
    return buffer[decrement(tail)]; //Restituisco l'ultima scansione inserita
}

ostream &operator<<(ostream &os, const LaserScannerDriver &driver) {
    return os << driver.get_last_scan(); //Uso operatore << di Scan
}

ostream &operator<<(ostream &os, const Scan &scan) {
    double ar = scan.get_angle_resolution();
    if (scan.is_valid()) {
        os << "[";
        for (int i = 0; i < scan.get_length(); ++i) {
            os << ar * i << ": " << scan[i]; //Mostro "angolo: distanza"
            if (i != scan.get_length() - 1)
                os << ", ";
        }
        os << "]" << endl;
    } else {
        throw Scan::ScanNotValid{}; //Eccezione se scansione non valida
    }
    return os;
}

double LaserScannerDriver::get_distance(double angle) const {
    return get_last_scan().get_distance_from_angle(angle);
}

double Scan::get_distance_from_angle(double angle) const {
    int min = 0;
    for (int i = 0; i < length; ++i)
        if (abs(angle - i * angle_resolution) < abs(angle - min * angle_resolution))
            min = i; //Cerco l'indice con la minima distanza tra angolo cercato e angoli della scansione
    return scan[min];
}

Scan::Scan(const Scan &s)
        : length{s.length}, angle_resolution{s.angle_resolution}, scan{new double[s.length]} {
    copy(s.scan, s.scan + s.length, scan); //Copio s.scan in s
}

Scan &Scan::operator=(const Scan &s) {
    double *temp = new double[s.length];
    copy(s.scan, s.scan + s.length, temp); //Copio s.scan in s
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
    cout << "length: " << length << ", angle resolution: " << angle_resolution << " [";
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
}

LaserScannerDriver &LaserScannerDriver::operator=(const LaserScannerDriver &l) {
    Scan *temp = new Scan[BUFFER_DIM];
    for (int i = 0; i < BUFFER_DIM; ++i) {
        buffer[i] = l.buffer[i];
    }
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



