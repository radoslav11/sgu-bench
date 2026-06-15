#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    long long N, M;
    std::cin >> N >> M;

    if (M == 0) {
        std::cout << std::fixed << std::setprecision(15) << 0.0 << '\n';
    } else {
        double x = 1.0 / static_cast<double>(N);
        double log_val = std::log1p(-x);
        double term = std::exp(M * log_val);
        double result = static_cast<double>(N) * (1.0 - term);
        std::cout << std::fixed << std::setprecision(15) << result << '\n';
    }

    return 0;
}
