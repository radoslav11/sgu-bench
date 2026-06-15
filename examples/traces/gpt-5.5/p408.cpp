#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    long long ans = 0;
    for (int k = 1; k <= N; ++k) {
        long long x = k + 2;
        ans += (x * x) / 4; // floor((k + 2)^2 / 4)
    }

    cout << fixed << setprecision(3) << (double)ans << '\n';
    return 0;
}
