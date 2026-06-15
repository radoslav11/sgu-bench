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

int n, q;

void read() {
    cin >> n >> q;
}

int rec(int len, int pos) {
    if(len == 1) {
        return 1;
    }

    int k = len / 2;
    if(pos <= k) {
        return len - k + rec(k, k - pos + 1);
    }

    return rec(len - k, len - pos + 1);
}

void solve() {
    // phi splits W into the second half (reversed) followed by the first
    // half (reversed), recursively. Mirror the same recursion to locate where
    // letter w_q lands: with k = len / 2, position pos in the first half maps
    // into the encoded second block (offset by len - k) at mirrored position
    // k - pos + 1, while a position in the second half maps into the first
    // block at mirrored position len - pos + 1. Recurse until length 1.

    cout << rec(n, q) << '\n';
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
