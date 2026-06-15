#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int64 T(int64 p) {
    return p * (p - 1) / 2;
}

int64 D(int64 p, int64 d) {
    if (d < 0 || d > p) return 0;

    if (p == 0) {
        return d == 0 ? 1 : 0;
    }

    if (d == 0) return 2;
    if (d < p) return d;
    return p - 1; // d == p
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    while (N--) {
        int64 M;
        cin >> M;

        int64 total = 0;

        for (int64 p = 0; T(p) <= M; ++p) {
            int64 d = M - T(p);
            total += D(p, d);
            if (total > 1) break;
        }

        cout << (total == 1 ? "YES" : "NO") << '\n';
    }

    return 0;
}
