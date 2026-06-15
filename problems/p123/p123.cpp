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

int n;

void read() { cin >> n; }

void solve() {
    // Sum of the first K Fibonacci numbers. We accumulate F3..FK iteratively
    // into sum and add F1 = F2 = 1 separately when K reaches those indices.
    // K < 41 so the result fits comfortably in a 64-bit integer.

    int64_t sum = 0;
    int64_t f1 = 1, f2 = 1;
    for(int i = 3; i <= n; i++) {
        int64_t fi = f1 + f2;
        sum += fi;
        f1 = f2;
        f2 = fi;
    }

    if(n >= 1) {
        sum += 1;
    }

    if(n >= 2) {
        sum += 1;
    }

    cout << sum << '\n';
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
