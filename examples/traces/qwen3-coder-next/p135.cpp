#include <iostream>

int main() {
    long long n;
    std::cin >> n;
    
    long long result = 1 + n * (n + 1) / 2;
    
    std::cout << result << std::endl;
    
    return 0;
}
