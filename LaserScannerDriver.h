#include <vector>
#include <ostream>


class Scan {
private:
    double *scan;
    int length;
    double angle_resolution;

public:
    Scan() : scan{nullptr}, length{0}, angle_resolution{0} {};

    Scan(double AngleResolution);

    void insert_vector(const std::vector<double> &v);

    std::vector<double> get_vector();

    void clear_scan();

    double get_distance_from_angle(double angle) const;

    double operator[](int n) const;

    bool is_valid() const { return scan; }

    int get_length() const { return length; }

    double get_angle_resolution() const { return angle_resolution; }


    class ScanNotValid {
    };

    friend std::ostream &operator<<(std::ostream &os, const Scan &scan);

    //Costruttore di copia
    Scan(const Scan &s);

    //Assegnamento di copia
    Scan &operator=(const Scan &s);

    //Costruttore di spostamento
    Scan(Scan &&s);

    //Assegnamento di spostamento
    Scan &operator=(Scan &&s);

    virtual ~Scan();

};


class LaserScannerDriver {
private:
    int head;
    int tail;
    int size;
    static const int BUFFER_DIM = 3;
    Scan *buffer;

    int increment(const int n) const { return (n + 1) % BUFFER_DIM; }

    int decrement(const int n) const {
        if (n == 0)
            return BUFFER_DIM - 1;
        return n - 1;
    }

    Scan get_last_scan() const;


public:

    explicit LaserScannerDriver(double angleResolution = 1);

    void new_scan(const std::vector<double> &v);

    std::vector<double> get_scan();

    void clear_buffer();

    double get_distance(double angle) const;

    //~LaserScannerDriver();

    class EmptyBuffer {
    };


    friend std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &driver);


};



