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

int64_t s, m, p;

void read() {
    cin >> s >> m >> p;
}

void solve() {
    // Annuity payment: each month the same x is paid, of which p% of the
    // current debt is interest and the rest reduces the principal, so the debt
    // must reach 0 after m months. With monthly rate r = p/100 the standard
    // annuity formula gives x = s * r * (1+r)^m / ((1+r)^m - 1). When p = 0
    // there is no interest and the payment is simply s / m.

    double x;
    if(p == 0) {
        x = (double)s / m;
    } else {
        double r = p / 100.0;
        double pw = pow(1 + r, (double)m);
        x = s * (r * pw) / (pw - 1);
    }

    cout << fixed << setprecision(5) << x << '\n';
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
