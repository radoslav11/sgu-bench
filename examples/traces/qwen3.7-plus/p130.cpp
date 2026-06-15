#include <iostream>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long k;
    if (cin >> k) {
        // Use unsigned __int128 to completely prevent any intermediate overflow
        // although unsigned long long would also be sufficient for k <= 30.
        unsigned __int128 n = 1;
        for (long long i = 1; i <= k; ++i) {
            n = n * (4 * i - 2) / (i + 1);
        }
        
        unsigned long long ans = (unsigned long long)n;
        // The minimal number of parts is achieved when no two chords intersect.
        // In this case, each of the k chords adds exactly 1 new region, 
        // starting from the 1 initial region (the circle itself).
        // Thus, minimal parts P = k + 1.
        // The number of such non-intersecting configurations is the k-th Catalan number.
        cout << ans << " " << k + 1 << "\n";
    }
    
    return 0;
}
