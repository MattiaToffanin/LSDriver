/**
 * @author MATTIA TOFFANIN
 */

#include <ostream>
#include <vector>
#include <cmath>
#include "LaserScannerDriver.h"

LaserScannerDriver::LaserScannerDriver(double angleResolution) //Inizializzo le variabili membro
        : buffer{new Scan[BUFFER_DIM]}, head{0}, tail{0}, size{0} {
    if (angleResolution < 0.1 || angleResolution > 1)
        throw AngleResolutionNotValid {};
    for (int i = 0; i < BUFFER_DIM; ++i)
        buffer[i] = Scan(angleResolution); //Imposto la risoluzione angolare per ogni Scansione nel buffer
}

Scan::Scan(double angleResolution) //Inizializzo le variabili membro (scan = nullptr)
        : scan{nullptr}, angle_resolution{angleResolution} {
    length = std::floor(180 / angleResolution) + 1;
};

void LaserScannerDriver::new_scan(const std::vector<double> &v) {
    buffer[tail].insert_vector(v); //Inserisco scansione in coda
    if (size == BUFFER_DIM)
        head = increment(head); //Incremento head solo se il buffer è pieno
    tail = increment(tail); //Incremento sempre tail
    if (size < BUFFER_DIM)
        size++; //Incremento size solo se il buffer non è pieno
}

void Scan::insert_vector(const std::vector<double> &v) {
    delete[] scan; //Dealloco scan (ok anche nel caso nullptr)
    scan = new double[length]; //Alloco un nuovo array di double (letture)
    for (int i = 0; i < length; ++i) //Tronca se supero 180 gradi
        if (i >= v.size())
            scan[i] = 0; //Assegna 0 se mancano dati
        else
            scan[i] = v[i]; //Copio il vettore con la scansione (
}

std::vector<double> LaserScannerDriver::get_scan() {
    if (size == 0)
        throw EmptyBuffer{}; //Eccezione se buffer vuoto
    std::vector<double> v = buffer[head].get_vector(); //Rimuovo scansione in testa
    head = increment(head); //Incremento sempre head
    size--; //Decremento size
    return v; //Sfrutto move di vector
}

std::vector<double> Scan::get_vector() {
    std::vector<double> v; //Creo un vettore
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

std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &driver) {
    return os << driver.get_last_scan(); //Uso operatore << di Scan
}

std::ostream &operator<<(std::ostream &os, const Scan &scan) {
    double ar = scan.get_angle_resolution();
    if (scan.is_valid()) {
        os << "[";
        for (int i = 0; i < scan.get_length(); ++i) {
            os << ar * i << ": " << scan[i]; //Mostro "angolo: distanza"
            if (i != scan.get_length() - 1)
                os << ", ";
        }
        os << "]" << std::endl;
    } else {
        throw Scan::ScanNotValid{}; //Eccezione se scansione non valida
    }
    return os;
}

double LaserScannerDriver::get_distance(double angle) const {
    return get_last_scan().get_distance_from_angle(angle);
}

double Scan::get_distance_from_angle(double angle) const {
    if (angle >= 180)
        return scan[length - 1]; //Se angle > 180 restituisco utlimo elemento
    if (angle <= 0)
        return scan[0];
    int min = round(angle / angle_resolution);
    return scan[min];
}

LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver &l)
        : head{l.head}, tail{l.tail}, size{l.size}, buffer{new Scan[BUFFER_DIM]} { //Copio variabili membro
    std::copy(l.buffer, l.buffer + BUFFER_DIM, buffer); //std::copy chiama l'operatore = per ogni scansione nel buffer
}

LaserScannerDriver &LaserScannerDriver::operator=(const LaserScannerDriver &l) {
    Scan *temp = new Scan[BUFFER_DIM];
    std::copy(l.buffer, l.buffer + BUFFER_DIM, temp); //std::copy chiama l'operatore = per ogni scansione nel buffer
    delete[] buffer; //Dealloco buffer "vecchio"
    buffer = temp; //buffer punta a temp
    head = l.head;//Copio altre varaiabili membro
    tail = l.tail;
    size = l.size;
    return *this;
}

LaserScannerDriver::LaserScannerDriver(LaserScannerDriver &&l)
        : head{l.head}, tail{l.tail}, size{l.size},
          buffer{l.buffer} { //Copio variabili membro (shallow copy per buffer)
    l.head = 0; //Annullo l
    l.tail = 0;
    l.size = 0;
    l.buffer = nullptr;
}

LaserScannerDriver &LaserScannerDriver::operator=(LaserScannerDriver &&l) {
    delete[] buffer; //Dealloco buffer "vecchio"
    buffer = l.buffer; //Copio variabili membro (shallow copy per buffer)
    head = l.head;
    tail = l.tail;
    size = l.size;
    l.buffer = nullptr; //Annullo l
    l.head = 0;
    l.tail = 0;
    l.size = 0;
    return *this;
}

Scan::Scan(const Scan &s)
        : length{s.length}, angle_resolution{s.angle_resolution}, scan{new double[s.length]} { //Copio variabili membro
    std::copy(s.scan, s.scan + s.length, scan); //Copio s.scan in this.scan
}

Scan &Scan::operator=(const Scan &s) {
    double *temp = new double[s.length];
    if (s.scan)
        std::copy(s.scan, s.scan + s.length, temp); //Copio s.scan in temp
    else
        temp = nullptr;
    delete[] scan; //Dealloco scan "vecchio"
    scan = temp; //scan punta a temp
    length = s.length; //Copio altre variabili membro
    angle_resolution = s.angle_resolution;
    return *this;
}

Scan::Scan(Scan &&s)
        : length{s.length}, angle_resolution{s.angle_resolution},
          scan{s.scan} { //Copio variabili membro (shallow copy per scan)
    s.length = 0; //Annullo s
    s.angle_resolution = 0;
    s.scan = nullptr;
}

Scan &Scan::operator=(Scan &&s) {
    delete[] scan; //Dealloco scan "vecchio"
    scan = s.scan; //Copio variabili membro (shallow copy per scan)
    length = s.length;
    angle_resolution = s.angle_resolution;
    s.scan = nullptr; //Annullo s
    s.length = 0;
    s.angle_resolution = 0;
    return *this;
}

LaserScannerDriver::~LaserScannerDriver() {
    delete[] buffer; //Dealloco buffer
    buffer = nullptr; //Annullo variabili membro
    head = 0;
    tail = 0;
    size = 0;
}

Scan::~Scan() {
    delete[] scan; //Dealloco scan
    scan = nullptr; //Annullo variabili membro
    length = 0;
    angle_resolution = 0;
}

void Scan::print() const {
    std::cout << "length: " << length << ", angle resolution: " << angle_resolution << " [";
    if (scan) {
        for (int i = 0; i < length; ++i) {
            std::cout << angle_resolution * i << ": " << scan[i];
            if (i != length - 1)
                std::cout << ", ";
        }
    } else std::cout << "Nullptr";
    std::cout << "]" << std::endl;
}


void LaserScannerDriver::print() const {
    std::cout << "head: " << head << ", tail: " << tail << ", size: " << size << std::endl;
    for (int i = 0; i < BUFFER_DIM; ++i)
        if (buffer[i].is_valid())
            buffer[i].print();
        else
            std::cout << "Nullptr" << std::endl;
}



