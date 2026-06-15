#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long K;
    cin >> K;                 // Read the number of digits K (1 ≤ K ≤ 1e6)

    if (K == 1) {
        // Special case: single-digit perfect numbers are 1..8
        cout << 8 << "\n";
    }
    else if (K % 6 == 1) {
        // For K≥2, if K ≡ 1 mod 6, there are exactly 4 perfect numbers
        cout << 4 << "\n";
    }
    else if (K % 3 == 1) {
        // Otherwise, if K ≡ 1 mod 3 (but not mod 6), there are 3
        cout << 3 << "\n";
    }
    else {
        // In all other cases, there is exactly 1 perfect number
        cout << 1 << "\n";
    }

    return 0;
}
