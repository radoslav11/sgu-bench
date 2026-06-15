#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    if (cin >> n) {
        // The sum of dots on the side faces of h dice is always 14 * h.
        // The sum of dots on the top and bottom faces is between 2 and 12.
        // So we need 2 <= n - 14 * h <= 12.
        // This implies 14 * h >= n - 12, so h >= (n - 12) / 14.
        // Using ceiling division for positive numerators: ceil((n - 12) / 14) = (n - 12 + 13) / 14 = (n + 1) / 14.
        // Also, the height h must be at least 1.
        long long h = max(1LL, (n + 1) / 14);
        
        // Check if the chosen minimum height satisfies the condition n - 14 * h >= 2
        // (The upper bound n - 14 * h <= 12 is guaranteed by the ceiling division)
        if (n - 14 * h >= 2) {
            cout << h << "\n";
        } else {
            cout << -1 << "\n";
        }
    }
    
    return 0;
}
