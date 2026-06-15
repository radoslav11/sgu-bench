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

int n, m, k;
vector<int> a;

void read() {
    cin >> n >> m >> k;
    a.resize(n);
    cin >> a;
}

int pw(int x, int p) {
    int r = 1 % k;
    while(p) {
        if(p & 1) {
            r = r * 1ll * x % k;
        }

        x = x * 1ll * x % k;
        p >>= 1;
    }

    return r;
}

void solve() {
    // A number v^M is divisible by K iff v^M mod K == 0, computed with fast
    // modular exponentiation. Count how many values in the sequence satisfy
    // this.

    int answer = 0;
    for(int v: a) {
        answer += pw(v, m) == 0;
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
