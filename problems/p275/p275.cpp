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
vector<int64_t> a;
int64_t basis[64];

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void add(int64_t x) {
    for(int l = 60; l >= 0; l--) {
        if(!(x & (1ll << l))) {
            continue;
        }
        if(basis[l] == 0) {
            basis[l] = x;
            return;
        }
        x ^= basis[l];
    }
}

void solve() {
    // Maximum XOR of a subsequence via a linear basis over GF(2).
    //
    // - basis[l] holds a reduced vector whose highest set bit is l. Inserting a
    //   number repeatedly clears its top bit using existing basis vectors; if a
    //   bit position is still free, the reduced number becomes the new basis
    //   vector there.
    //
    // - To maximise the XOR, scan bit positions from high to low and XOR in
    //   basis[l] whenever it would turn the current result's bit l from 0 to 1.

    for(int64_t x: a) {
        add(x);
    }

    int64_t res = 0;
    for(int l = 60; l >= 0; l--) {
        if(!(res & (1ll << l))) {
            res ^= basis[l];
        }
    }

    cout << res << "\n";
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
