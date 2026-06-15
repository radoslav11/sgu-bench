#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int K;
    if (!(cin >> K)) return 0;
    long long a = 1, b = 1;
    if (K == 1) { cout << 1 << '\n'; return 0; }
    if (K == 2) { cout << 2 << '\n'; return 0; } // F1+F2 = 2
    // compute F_{K+2} then subtract 1
    for (int i = 3; i <= K+2; ++i) {
        long long c = a + b;
        a = b;
        b = c;
    }
    // after loop, b = F_{K+2}
    cout << (b - 1) << '\n';
    return 0;
}
