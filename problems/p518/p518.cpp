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
vector<vector<int>> graph;
vector<int> path;

void read() {
    cin >> n;
    graph.assign(n, vector<int>(n));
    cin >> graph;
    cin >> k;
    path.resize(k);
    cin >> path;
}

void solve() {
    // BFS over states (node, pos): we are at intersection node having
    // matched the first pos road lengths of the route. From (node, pos) we
    // step to any next_node whose connecting road length graph[node][next]
    // equals path[pos], advancing to (next, pos + 1). visited[node][pos]
    // prevents revisiting a state. Every state reaching pos == k marks node
    // as a possible secret place. We collect those nodes (1-indexed),
    // sorted ascending.

    deque<pair<int, int>> queue = {{0, 0}};
    vector<vector<bool>> visited(n, vector<bool>(k + 1, false));
    visited[0][0] = true;
    set<int> possible;

    while(!queue.empty()) {
        auto [node, pos] = queue.front();
        queue.pop_front();

        if(pos == k) {
            possible.insert(node);
            continue;
        }

        for(int next_node = 0; next_node < n; next_node++) {
            if(graph[node][next_node] == path[pos] &&
               !visited[next_node][pos + 1]) {
                visited[next_node][pos + 1] = true;
                queue.push_back({next_node, pos + 1});
            }
        }
    }

    cout << possible.size() << '\n';
    if(!possible.empty()) {
        bool first = true;
        for(int x: possible) {
            cout << (first ? "" : " ") << x + 1;
            first = false;
        }
        cout << '\n';
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
