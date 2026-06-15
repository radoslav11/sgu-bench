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

int64_t n;

void read() { cin >> n; }

void solve() {
    // The k-th term 123...k is divisible by 3 exactly when its digit sum, which
    // is 1+2+...+k = k(k+1)/2, is divisible by 3, i.e. when k mod 3 is 0 or 2.
    // So among k = 1..n there are 2 such terms per full block of 3, plus one
    // extra when n mod 3 == 2.

    int64_t answer = n / 3 * 2;
    if(n % 3 == 2) {
        answer++;
    }

    cout << answer << '\n';
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
