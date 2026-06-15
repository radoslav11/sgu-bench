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

int n, k;

void read() { cin >> n >> k; }

void solve() {
    /*
     * A funny (Euclid) string of length n with sum k is the discretized line
     * y = (k/n) x: every term is either floor(k/n) or that plus one. Start with
     * all terms equal to a = k/n; d = k % n of them must be bumped by one. Find
     * the step t (1 <= t < n) with t*d == -1 (mod n), then walk positions
     * x = t, 2t, ... mod n incrementing each until reaching index n-1; those d
     * positions form the staircase pattern that makes the string funny.
     */

    int a = k / n;
    int d = k % n;

    vector<int> ans(n, a);

    for(int t = 1; t < n; t++) {
        if(t * d % n == n - 1) {
            int x = 0;
            do {
                x = (x + t) % n;
                ans[x]++;
            } while(x != n - 1);
            break;
        }
    }

    cout << ans << '\n';
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
