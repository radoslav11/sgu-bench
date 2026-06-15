#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    int s, m, p;
    std::cin >> s >> m >> p;

    double x;
    if (p == 0) {
        x = static_cast<double>(s) / m;
    } else {
        double r = static_cast<double>(p) / 100.0;
        double factor = std::pow(1.0 + r, m);
        x = static_cast<double>(s) * (r * factor) / (factor - 1.0);
    }

    std::cout << std::fixed << std::setprecision(5) << x << std::endl;

    return 0;
}
