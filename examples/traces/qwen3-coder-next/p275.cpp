#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    // Gaussian elimination to find basis
    // basis[i] will store a number whose highest set bit is at position i
    vector<long long> basis(61, 0); // 61 bits for numbers up to 10^18
    
    // Build the basis
    for (int i = 0; i < n; i++) {
        long long x = a[i];
        for (int j = 60; j >= 0; j--) {
            if ((x >> j) & 1) {
                if (basis[j] == 0) {
                    basis[j] = x;
                    break;
                } else {
                    x ^= basis[j];
                }
            }
        }
    }
    
    // Find maximum XOR value
    long long max_xor = 0;
    for (int j = 60; j >= 0; j--) {
        // If we can set the j-th bit in our result, do it
        if ((max_xor ^ basis[j]) > max_xor) {
            max_xor ^= basis[j];
        }
    }
    
    cout << max_xor << endl;
    
    return 0;
}
