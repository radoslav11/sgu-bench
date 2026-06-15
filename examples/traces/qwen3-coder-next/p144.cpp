#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    int X, Y;
    double Z;
    std::cin >> X >> Y >> Z;

    double T = static_cast<double>(Y - X);
    double w = Z / 60.0;

    // Since Z <= 60*(Y-X), w is at most T.
    double numerator = 2.0 * w * T - w * w;
    double denominator = T * T;
    double prob = numerator / denominator;

    std::cout << std::fixed << std::setprecision(7) << prob << std::endl;

    return 0;
}
