#include <vector>
#include <iostream>

class Scan {
private:
    double *scan;
    int length;
    double angle_resolution;

public:
    Scan() {};

    Scan(double AngleResolution);

    void insert_vector(const std::vector<double> &v);

    std::vector<double> get_vector();

    void clear_scan();

    double operator[](int n) const;

    const bool is_valid() const { return scan; }

    const int get_length() const { return length; }

    double get_angle_resolution() const;

    double get_distance_from_angle(const double angle) const;

    class ScanNotValid {
    };

    friend std::ostream &operator<<(std::ostream &os, const Scan &scan);
};
