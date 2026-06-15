#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double N, M;
    if (!(cin >> N >> M)) return 0;

    long double ans;
    if (M == 0) {
        ans = 0;
    } else if (N == 1) {
        ans = 1;
    } else {
        ans = N * (1.0L - expl(M * log1pl(-1.0L / N)));
    }

    cout << fixed << setprecision(12) << (double)ans << '\n';
    return 0;
}
