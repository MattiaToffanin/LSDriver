#include <vector>
#include <ostream>

#include "Scan.h"

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

    Scan &get_last_scan() const;


public:

    explicit LaserScannerDriver(double angleResolution = 1);

    void new_scan(const std::vector<double> &v);

    std::vector<double> get_scan();

    void clear_buffer();

    double get_distance(const double angle) const;

    //~LaserScannerDriver();

    class EmptyBuffer {
    };


    friend std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &driver);


};

