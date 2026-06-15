#include <iostream>
#include <algorithm>
using namespace std;

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    int A, B;
    cin >> A >> B;
    
    if (A == 0 || B == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    int g = gcd(A, B);
    int T = A + B;
    int k = T / g;
    
    // Check if k is a power of 2 and at least 2
    if (k < 2 || (k & (k - 1)) != 0) {
        cout << -1 << endl;
        return 0;
    }
    
    unsigned int k_uint = static_cast<unsigned int>(k);
    int moves = __builtin_ctz(k_uint);
    
    cout << moves << endl;
    
    return 0;
}
