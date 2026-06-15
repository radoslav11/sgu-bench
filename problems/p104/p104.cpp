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
vector<vector<int>> a;

void read() {
    cin >> n >> m;
    a.assign(n, vector<int>(m));
    cin >> a;
}

vector<vector<int>> dp;
vector<vector<int>> best;

int rec(int i, int j) {
    if(i == n) {
        return 0;
    }
    if(j == m) {
        return -1e9;
    }

    int& memo = dp[i][j];
    if(memo != -1) {
        return memo;
    }

    best[i][j] = 0;
    memo = rec(i, j + 1);

    int take = a[i][j] + rec(i + 1, j + 1);
    if(take > memo) {
        memo = take;
        best[i][j] = 1;
    }

    return memo;
}

void solve() {
    // - dp[i][j] = best total aesthetic value placing bunches i..F-1 into vases
    //   j..V-1, preserving the order (bunch i must sit left of bunch i+1).
    //
    // - At each state we either skip vase j (move to dp[i][j+1]) or place bunch
    //   i in vase j and take a[i][j] + dp[i+1][j+1]; best[i][j] records which
    //   choice was optimal so the placement can be reconstructed.
    //
    // - Walk the best[] choices from (0, 0) to read off the chosen vase for
    //   each bunch.

    dp.assign(n, vector<int>(m, -1));
    best.assign(n, vector<int>(m, 0));

    vector<int> ans(n);
    cout << rec(0, 0) << '\n';

    int i = 0, j = 0;
    while(i != n) {
        rec(i, j);
        if(best[i][j] == 0) {
            j++;
        } else {
            ans[i] = j;
            i++;
            j++;
        }
    }

    for(int k = 0; k < n; k++) {
        cout << ans[k] + 1 << " ";
    }
    cout << '\n';
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
