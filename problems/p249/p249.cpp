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

vector<int> gray_code(int bits) {
    vector<int> res;
    for(int i = 0; i < (1 << bits); i++) {
        res.push_back(i ^ (i >> 1));
    }

    return res;
}

void solve() {
    // We want a 2^N x 2^M cyclic Gray arrangement of 0..2^(N+M)-1 where adjacent
    // cells differ in one bit. Take the standard reflected Gray code over N bits
    // for the rows and over M bits for the columns. Each Gray sequence is cyclic
    // (its first and last entries also differ in one bit), so wrapping the grid
    // is fine. The cell at row i, column j gets the high N bits from the row's
    // Gray value and the low M bits from the column's: (gn[i] << M) | gm[j].
    // Horizontal neighbours change only the column part by one bit and vertical
    // neighbours only the row part, so any two adjacent cells differ in exactly
    // one bit.

    vector<int> gn = gray_code(n), gm = gray_code(m);
    for(int i = 0; i < (int)gn.size(); i++) {
        for(int j = 0; j < (int)gm.size(); j++) {
            cout << (gn[i] << m) + gm[j] << ' ';
        }

        cout << '\n';
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
