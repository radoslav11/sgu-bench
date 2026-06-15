#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, m;

void read() { cin >> n >> m; }

void solve() {
    // The minimum number of shots equals the number of distinct directions
    // from Petya's corner to the other cells, i.e. the number of visible
    // lattice points in the (N-1) x (M-1) interior block plus the two axis
    // directions (the rest of the bottom row and the left column).
    //
    // A point (a, b) with 1 <= a <= N-1, 1 <= b <= M-1 is visible exactly when
    // gcd(a, b) = 1. Count coprime pairs by inclusion-exclusion over the gcd d:
    // dp[d] = (number of pairs whose gcd is exactly d) is ((N-1)/d)*((M-1)/d)
    // minus dp of all multiples of d. Visible interior pairs is dp[1], computed
    // as total pairs minus dp[d] for d >= 2. Finally add the two axis shots
    // when the corresponding side has more than one cell.

    int64_t ans = (n - 1) * 1ll * (m - 1);
    vector<int64_t> dp(min(n, m) + 1, 0);
    for(int d = min(n, m); d >= 2; d--) {
        dp[d] = ((n - 1) / d) * 1ll * ((m - 1) / d);
        for(int d2 = 2 * d; d2 <= min(n, m); d2 += d) {
            dp[d] -= dp[d2];
        }

        ans -= dp[d];
    }
    cout << ans + (n > 1) + (m > 1) << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
