#include <iostream>
using namespace std;

int main() {
    int k;
    cin >> k;
    
    // Minimal parts P = k + 1
    unsigned long long P = k + 1;
    
    // Compute Catalan number C_k = (2k choose k) / (k+1)
    // Compute binom(2k, k) = (2k)*(2k-1)*...*(k+1) / k!
    unsigned long long binom = 1;
    // Compute numerator: (2k)*(2k-1)*...*(k+1)
    for (int i = 1; i <= k; i++) {
        binom *= (k + i);
        binom /= i;  // Divide at each step to avoid overflow
    }
    
    unsigned long long N = binom / (k + 1);
    
    cout << N << " " << P << endl;
    
    return 0;
}
