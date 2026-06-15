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
    // Closed-form count of perfect k-digit numbers (derived from the digit
    // structure of good/perfect numbers): k == 1 gives 8, otherwise the answer
    // depends only on k mod 6 / k mod 3 -- 4 when k % 6 == 1, 3 when
    // k % 3 == 1, and 1 in every other case.

    if(n == 1) {
        cout << 8 << '\n';
    } else if(n % 6 == 1) {
        cout << 4 << '\n';
    } else if(n % 3 == 1) {
        cout << 3 << '\n';
    } else {
        cout << 1 << '\n';
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
