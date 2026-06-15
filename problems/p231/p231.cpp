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

const int MAXN = (1 << 20);

int n;
bitset<MAXN> composite;

void read() {
    cin >> n;
    composite[0] = composite[1] = 1;
    for(int x = 2; x <= n; x++) {
        for(int y = 2 * x; y <= n; y += x) {
            composite[y] = 1;
        }
    }
}

void solve() {
    // We need prime pairs A <= B with A + B prime and A + B <= N. The sum of
    // two odd primes is even, so it can only be prime when one addend is 2.
    // Hence every valid pair is (2, B) where both B and B + 2 are prime, i.e.
    // B is the smaller member of a twin-prime pair. We sieve up to N and collect
    // all such B with B + 2 <= N.

    vector<pair<int, int>> ans;
    for(int b = 1; b + 2 <= n; b++) {
        if(!composite[b] && !composite[b + 2]) {
            ans.push_back({2, b});
        }
    }

    cout << ans.size() << '\n';
    for(auto& p: ans) {
        cout << p << '\n';
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
