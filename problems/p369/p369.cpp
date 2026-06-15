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
vector<int> x, y;
vector<vector<int>> adj;

void read() {
    cin >> n;
    x.resize(n);
    y.resize(n);
    adj.assign(n, {});

    map<int, int> col_last, row_last;
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];

        if(col_last.count(y[i])) {
            adj[i].push_back(col_last[y[i]]);
            adj[col_last[y[i]]].push_back(i);
        } else {
            col_last[y[i]] = i;
        }

        if(row_last.count(x[i])) {
            adj[i].push_back(row_last[x[i]]);
            adj[row_last[x[i]]].push_back(i);
        } else {
            row_last[x[i]] = i;
        }
    }
}

void solve() {
    // Each black cell links the previous cell sharing its row and the previous
    // cell sharing its column, building a graph whose connected components are
    // closed under the rectangle-completion move. Within one component every
    // (row, column) pair that appears can eventually be filled, so the final
    // count of black cells in that component is exactly the number of distinct
    // rows times the number of distinct columns it touches.
    //
    // Flood fill each component, collect its distinct row and column
    // coordinates in sets, and accumulate rows * cols across all components.

    vector<char> used(n, 0);
    int64_t answer = 0;

    for(int s = 0; s < n; s++) {
        if(used[s]) {
            continue;
        }

        set<int> rw, cl;
        vector<int> stack = {s};
        used[s] = 1;
        while(!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            rw.insert(x[u]);
            cl.insert(y[u]);
            for(int v: adj[u]) {
                if(!used[v]) {
                    used[v] = 1;
                    stack.push_back(v);
                }
            }
        }

        answer += (int64_t)rw.size() * (int64_t)cl.size();
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
