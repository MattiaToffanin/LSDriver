/**
 * @author MATTIA TOFFANIN
 */

#include <vector>
#include <iostream>
#include <cmath>

/**
 * Classe per memorizzare una scansione
 */
class Scan {

private:

    /**
     * scan: puntatore a array contenente le varie letture
     * length: lunghezza della scansione (e di scan)
     * angle_resolution: risoluzione angolare della scansione
     */
    double *scan;
    int length;
    double angle_resolution;

public:

    /**
     * Costruttore
     * @param angleResolution: risoluzione angolare (default 1)
     */
    explicit Scan(double angleResolution = 1);

    /**
     * Costruttore di copia (utile per la copia di LaserScannerDriver)
     * @param s: scansione da copiare
     */
    Scan(const Scan &s);

    /**
     * Assegnamento di copia (utile per la copia di LaserScannerDriver)
     * @param s: scansione da copiare
     * @return scansione copiata
     */
    Scan &operator=(const Scan &s);

    /**
     * Costruttore di spostamento (utile per la funzione membro privata get_last_scan di LaserScannerDriver che ritorna uno Scan)
     * @param s: scansione "da spostare"
     */
    Scan(Scan &&s);

    /**
     * Assegnamento di spostamento (utile per la funzione membro privata get_last_scan di LaserScannerDriver che ritorna uno Scan)
     * @param s: scansione "da spostare"
     * @return scansione "spostata"
     */
    Scan &operator=(Scan &&s);

    /**
     * Distruttore
     */
    ~Scan();

    /**
     * Funzione membro per allocare una scansione tramite vettore
     * @param v: vettore da inserire nella scansione
     */
    void insert_vector(const std::vector<double> &v);

    /**
     * Funzione membro per deallocare e ricevere una scansione tramite vettore
     * @return vettore della scansione
     */
    std::vector<double> get_vector();

    /**
     * Funzione membro per deallocare l'array scan
     */
    void clear_scan();

    /**
     * Funzione membro per ricevere la distanza corrispondente ad un angolo
     * @param angle: angolo da cercare
     * @return distanza corrispondente all'angolo più vicino specificato
     */
    double get_distance_from_angle(double angle) const;

    /**
     * Funzione membro per visitare il buffer tramite indice
     * @param n: indice
     * @return distanza corrispondente all'indice
     */
    double operator[](int n) const { return scan[n]; }; //Usato per operatore <<

    /**
     * Funzione membro per verificare se la scansione è valida
     * @return true se valida, false altrimenti
     */
    bool is_valid() const { return scan; } //Usato per operatore <<

    /**
     * Funzione membro per ricevere la lunghezza della scansione
     * @return lunghezza della scansione
     */
    int get_length() const { return length; } //Usato per operatore <<

    /**
     * Funzione membro per ricevere la risoluzione angolare della scansione
     * @return risoluzione angolare della scansione
     */
    double get_angle_resolution() const { return angle_resolution; } //Usato per operatore <<

    /**
     * Funzione membro di debug per stampare lo stato della scansione
     */
    void print() const;

    /**
     * Classe interna per eccezione
     */
    class ScanNotValid {
    };

};

std::ostream &operator<<(std::ostream &os, const Scan &scan);

/**
 * Classe per gestire un buffer circolare di scansioni
 */
class LaserScannerDriver {

private:

    /**
     * head: indice di rimozione
     * tail: indice di inserimento
     * size: numero di scansioni nel buffer
     * BUFFER_DIM: dimensione del buffer
     * buffer: puntatore a array contenente le varie scansioni
     */
    int head;
    int tail;
    int size;
    static const int BUFFER_DIM = 3;
    Scan *buffer;


    /**
     * Funzione membro privata per incrementare indici del buffer
     * @param n: indice da incrementare
     * @return indice incrementato
     */
    int increment(const int n) const { return (n + 1) % BUFFER_DIM; }

    /**
     * Funzione membro privata per decrementare indici del buffer
     * @param n: indice da decrementare
     * @return indice decrementato
     */
    int decrement(const int n) const {
        if (n == 0)
            return BUFFER_DIM - 1;
        return n - 1;
    }

public:

    /**
     * Costruttore
     * @param angleResolution: risoluzione angolare del LS da 0.1 a 1
     */
    explicit LaserScannerDriver(double angleResolution = 1);

    /**
     * Costruttore di copia
     * @param l: LSD da copiare
     */
    LaserScannerDriver(const LaserScannerDriver &l);

    /**
     * Assegnamento di copia
     * @param l: LSD da copiare
     * @return LSD copiato
     */
    LaserScannerDriver &operator=(const LaserScannerDriver &l);

    /**
     * Costruttore di spostamento
     * @param l: LSD "da spostare"
     */
    LaserScannerDriver(LaserScannerDriver &&l);

    /**
     * Assegnamento di spostamento
     * @param l: LSA "da spostare"
     * @return LSD "spostato"
     */
    LaserScannerDriver &operator=(LaserScannerDriver &&l);

    /**
     * Distruttore
     */
    ~LaserScannerDriver();

    /**
     * Funzione membro per inserire nel buffer una scansione tramite vettore (se il buffer è pieno sovrascrive la scansione più vecchia)
     * @param v: vettore rappresentante la scansione da inserire
     */
    void new_scan(const std::vector<double> &v);

    /**
     * Funzione membro per ricevere e rimuovere dal buffer la scansione più vecchia
     * Non si può chiamare se buffer vuoto
     * @return vettore rappresentante la scansione più vecchia
     */
    std::vector<double> get_scan();

    /**
     * Funione membro che elimina tutte le scansioni nel buffer
     */
    void clear_buffer();

    /**
     * Funzione membro per ricevere la distanza corrispondente ad un angolo nell'ultima scansione inserita
     * @param angle: angolo da cercare
     * @return distanza corrispondente all'angolo più vicino specificato
     */
    double get_distance(double angle) const;

    /**
     * Funzione membro per ricevere l'ultima scansione inserita senza rimuoverla dal buffer
     * @return ultima scansione inserita
     */
    Scan get_last_scan() const;

    /**
     * Funzione membro di debug per stampare lo stato del LSA (comprese tutte le scansiioni all'interno)
     */
    void print() const;

    /**
     * Classe interna per eccezione
     */
    class EmptyBuffer {
    };

    /**
     * Classe interna per eccezione
     */
    class AngleResolutionNotValid {
    };

};

std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &driver);


