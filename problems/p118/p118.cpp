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
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // The digital root of a positive number equals 1 + (n - 1) mod 9, i.e. its
    // value mod 9 with 0 mapped to 9. So we evaluate the nested sum
    // A1 + A1*A2 + ... + A1*...*AN modulo 9: keep a running prefix product s
    // mod 9 and accumulate it. The only special case is A1 == 0, where the
    // whole expression is 0 and the digital root is 0.

    if(a[0] == 0) {
        cout << 0 << '\n';
        return;
    }

    int ans = 0, s = 1;
    for(int i = 0; i < n; i++) {
        s = s * (a[i] % 9) % 9;
        ans = (ans + s) % 9;
    }

    cout << (ans == 0 ? 9 : ans) << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
