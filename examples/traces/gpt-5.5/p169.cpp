#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long K;
    cin >> K;

    if (K == 1) {
        cout << 8 << '\n';
        return 0;
    }

    long long t = K - 1;
    int ans = 1; // digit d = 1 always works

    if (t % 3 == 0) ans += 2; // d = 2 and d = 5
    if (t % 6 == 0) ans += 1; // d = 6

    cout << ans << '\n';
    return 0;
}
