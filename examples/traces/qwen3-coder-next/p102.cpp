#include <iostream>
#include <cmath>

int main() {
    int N;
    std::cin >> N;
    
    if (N == 1) {
        std::cout << 1 << std::endl;
        return 0;
    }
    
    int result = N;
    int temp = N;
    
    // Check for factor 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
        result = result / 2 * (2 - 1);
    }
    
    // Check for odd factors from 3 to sqrt(temp)
    for (int i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            while (temp % i == 0) {
                temp /= i;
            }
            result = result / i * (i - 1);
        }
    }
    
    // If remaining temp is a prime number greater than 2
    if (temp > 1) {
        result = result / temp * (temp - 1);
    }
    
    std::cout << result << std::endl;
    
    return 0;
}
