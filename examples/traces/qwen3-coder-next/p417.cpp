#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    int x0, y0, r;
    std::cin >> x0 >> y0 >> r;
    double d2 = static_cast<double>(x0) * x0 + static_cast<double>(y0) * y0;
    const double PI = std::acos(-1.0);
    double mass = PI * r * r * std::log(d2);
    std::cout << std::fixed << std::setprecision(15) << mass << std::endl;
    return 0;
}
