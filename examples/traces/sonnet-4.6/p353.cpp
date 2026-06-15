#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N, k1, k2, p1, p2, p3;
    cin >> N >> k1 >> k2 >> p1 >> p2 >> p3;
    
    if (N < p1) {
        cout << 0 << endl;
        return 0;
    }
    
    // Charge p1 upfront
    N -= p1;
    long long seconds = k1; // first k1 seconds are covered
    
    // Next k2 seconds at p2 each
    for (long long i = 0; i < k2; i++) {
        N -= p2;
        seconds++;
        if (N <= 0) {
            cout << seconds << endl;
            return 0;
        }
    }
    
    // Remaining at p3 each
    if (p3 <= 0) {
        // infinite, but p3 >= 1 per constraints
        cout << seconds << endl;
        return 0;
    }
    
    // How many more seconds at p3?
    // Each second: N -= p3, if N <= 0 stop (but count it)
    // Max additional = ceil(N / p3) ... 
    // If N > 0: she can do floor((N-1)/p3) + 1 more seconds before going non-positive
    long long more = (N - 1) / p3 + 1;
    seconds += more;
    
    cout << seconds << endl;
    return 0;
}
