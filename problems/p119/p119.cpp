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

int n, a0, b0;

void read() { cin >> n >> a0 >> b0; }

void solve() {
    // The valid pairs (A, B) are exactly the integer multiples of (A0, B0)
    // taken modulo N: (k*A0 mod N, k*B0 mod N) for k = 0..N-1. Generate all of
    // them, deduplicate, and output sorted by A then B.

    set<pair<int, int>> pairs;
    for(int k = 0; k < n; k++) {
        pairs.insert({(int)((int64_t)k * a0 % n), (int)((int64_t)k * b0 % n)});
    }

    cout << pairs.size() << '\n';
    for(auto& p: pairs) {
        cout << p.first << ' ' << p.second << '\n';
    }
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
