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
    // A square ending in ...987654321 must end in 9, so its root ends in 3 or
    // 7. Checking modulo 10^9 shows exactly two roots mod 10^9 give that
    // suffix, so squares end in 987654321 only for roots of at least 9 digits.
    // For n < 9 there are none; for n == 9 there are 8 (the two residues times
    // the choices of leading digit that keep it a 9-digit number); for n > 9
    // each extra digit multiplies the count by 10, giving 72 followed by
    // (n - 10) zeros.

    if(n < 9) {
        cout << 0 << '\n';
    } else if(n == 9) {
        cout << 8 << '\n';
    } else {
        cout << 72;
        n -= 10;
        while(n--) {
            cout << 0;
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
