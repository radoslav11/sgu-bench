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
    // Build the divisibility DAG: every proper divisor of i is a neighbour of
    // i. A valid painting needs different colors for any A divisible by B, so
    // the chromatic requirement is exactly a graph coloring on this DAG.
    //
    // Process numbers in increasing order and greedily assign each i the
    // smallest color not used by its (already colored) divisors. The number of
    // colors equals the length of the longest divisor chain, i.e. one plus the
    // number of prime factors counted with multiplicity, which is optimal.

    vector<int> answer(n + 1, 1);
    vector<vector<int>> adj(n + 1);
    for(int i = 1; i <= n; i++) {
        for(int j = i * 2; j <= n; j += i) {
            adj[j].push_back(i);
        }
    }

    vector<char> used(n + 2, 0);
    for(int i = 1; i <= n; i++) {
        for(int v: adj[i]) {
            used[answer[v]] = 1;
        }
        while(used[answer[i]]) {
            answer[i]++;
        }
        for(int v: adj[i]) {
            used[answer[v]] = 0;
        }
    }

    int mx_col = 1;
    for(int i = 1; i <= n; i++) {
        mx_col = max(mx_col, answer[i]);
    }

    cout << mx_col << '\n';
    for(int i = 1; i <= n; i++) {
        cout << answer[i] << " ";
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
