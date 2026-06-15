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
    // Expected prizes = sum over winners of the probability that the winner
    // picks a box not yet opened. Before a winner picks, the probability that
    // any particular box is still sealed is ((n-1)/n) raised to the number of
    // previous picks, so the chance this winner opens a fresh box is that same
    // power. Summing the geometric series term by term: each term starts at 1
    // and is multiplied by (n-1)/n for the next winner.

    if(n == 1) {
        cout << "1.0\n";
        return;
    }

    double ans = 0, prob = 1.0 / n, add = 1.0;
    for(int pos = 0; pos < m; pos++) {
        ans += add;
        add *= (n - 1) * prob;
    }

    cout << fixed << setprecision(10) << ans << '\n';
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
