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
    // A nearly prime number is a product of exactly two primes. For each value
    // trial-divide out prime factors counting them with multiplicity; the
    // number is nearly prime iff the total count of prime factors is exactly 2.

    for(int i = 0; i < n; i++) {
        int x = a[i];
        int cnt = 0;
        for(int d = 2; d * 1ll * d <= x; d++) {
            while(x % d == 0) {
                cnt++;
                x /= d;
            }
        }

        if(x != 1) {
            cnt++;
        }

        cout << (cnt == 2 ? "Yes" : "No") << '\n';
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
