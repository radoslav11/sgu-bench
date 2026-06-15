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

int n, p;
vector<int> x;

void read() {
    cin >> n >> p;
    x.resize(n);
    cin >> x;
}

void solve() {
    // Each son contributes potential^p to the sum, and we may freely choose
    // which sons to include, so the optimum simply takes every son whose
    // contribution is positive: add max(0, v^p) over all sons.

    int64_t ans = 0;
    for(int v: x) {
        int64_t term = 1;
        for(int e = 0; e < p; e++) {
            term *= v;
        }

        ans += max<int64_t>(0, term);
    }

    cout << ans << '\n';
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
