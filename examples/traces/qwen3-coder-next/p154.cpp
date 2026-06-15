#include <iostream>
using namespace std;

// Function to count trailing zeros in n!
long long countTrailingZeros(long long n) {
    long long count = 0;
    while (n >= 5) {
        n /= 5;
        count += n;
    }
    return count;
}

int main() {
    long long Q;
    cin >> Q;
    
    // Special case: Q = 0, minimal N is 1 (since 0! = 1 and 1! = 1, both have 0 trailing zeros)
    if (Q == 0) {
        cout << 1 << endl;
        return 0;
    }
    
    // Binary search for the minimal N such that countTrailingZeros(N) >= Q
    long long low = 1;
    long long high = 5 * Q; // Since for N >= 5, trailing zeros <= N/5 + N/25 + ... < N/4, so N <= 4*Q, but use 5*Q for safety
    
    long long result = -1;
    while (low <= high) {
        long long mid = (low + high) / 2;
        long long zeros = countTrailingZeros(mid);
        
        if (zeros >= Q) {
            result = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    // Check if the result actually has exactly Q trailing zeros
    if (result != -1 && countTrailingZeros(result) == Q) {
        cout << result << endl;
    } else {
        cout << "No solution" << endl;
    }
    
    return 0;
}
