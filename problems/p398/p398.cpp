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

int n, x;
vector<vector<int>> adj;

void read() {
    cin >> n >> x;
    adj.assign(n + 1, vector<int>(n + 1, 0));
    for(int i = 1; i <= n; i++) {
        int cnt;
        cin >> cnt;
        while(cnt--) {
            int f;
            cin >> f;
            adj[f][i] = 1;
            adj[i][f] = 1;
        }
    }
}

void solve() {
    // A friend of a friend of x is any user i that is not x, is not a direct
    // friend of x, yet shares a common friend o with x (o is a friend of x and
    // o is a friend of i). Scan every candidate i and, using the adjacency
    // matrix, look for such an intermediate o; emit the matching users in
    // increasing order.

    vector<int> li;
    for(int i = 1; i <= n; i++) {
        if(i == x || adj[i][x]) {
            continue;
        }

        bool ok = false;
        for(int o = 1; o <= n; o++) {
            if(adj[x][o] && adj[o][i]) {
                ok = true;
            }
        }

        if(ok) {
            li.push_back(i);
        }
    }

    cout << li.size() << '\n';
    cout << li << '\n';
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
