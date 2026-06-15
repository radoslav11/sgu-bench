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

int n, m, source, destination;
vector<tuple<bool, int, int, int>> junctions;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> source >> destination;
    source--, destination--;
    cin >> n >> m;

    junctions.resize(n);
    for(int i = 0; i < n; ++i) {
        char ci;
        int ri, tb, tp;
        cin >> ci >> ri >> tb >> tp;
        junctions[i] = make_tuple(ci == 'B', ri, tb, tp);
    }

    adj.assign(n, {});
    for(int i = 0; i < m; ++i) {
        int from, to, length;
        cin >> from >> to >> length;
        from--, to--;
        adj[from].push_back({to, length});
        adj[to].push_back({from, length});
    }
}

void solve() {
    // Dijkstra on (junction, time), where a road can only be traversed while
    // both endpoints show the same colour.
    //
    // Each junction is described by (initial colour, remaining time r of that
    // colour, blue duration tB, purple duration tP). get_color returns the
    // colour at absolute time t and time_to_next_color the wait until it next
    // flips; first_time_same_color advances t (waiting at the current
    // junction) until both endpoints agree, returning the earliest such moment
    // or -1 if they never align within a few flips.
    //
    // The Dijkstra state is the arrival time at a junction. To relax an edge we
    // wait for a matching colour, then add the road length. parent[] lets us
    // reconstruct one minimum-time path; if the destination is unreachable we
    // print 0.

    function<int(int, int)> get_color = [&](int u, int t) -> int {
        auto [is_blue, r, tb, tp] = junctions[u];
        if(t < r) {
            return is_blue;
        }

        if(is_blue && t < r + tp) {
            return 0;
        } else if(is_blue) {
            r += tp;
        }

        int cycle = tb + tp;
        int tu = (t - r) % cycle;
        return (int)(tu < tb);
    };

    function<int(int, int)> time_to_next_color = [&](int u, int t) {
        auto [is_blue, r, tb, tp] = junctions[u];
        if(t < r) {
            return r - t;
        }

        int cycle = tb + tp;
        if(is_blue && t < r + tp) {
            return r + tp - t;
        } else if(is_blue) {
            r += tp;
        }

        int tu = (t - r) % cycle;
        return tu < tb ? (tb - tu) : (cycle - tu);
    };

    function<int(int, int, int)> first_time_same_color =
        [&](int u, int v, int t) {
            for(int attempt = 0; attempt < 3; attempt++) {
                if(get_color(v, t) == get_color(u, t)) {
                    return t;
                }

                int dtu = time_to_next_color(u, t);
                int dtv = time_to_next_color(v, t);
                t += min(dtu, dtv);
            }

            return -1;
        };

    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;

    pq.push({0, source});
    dist[source] = 0;
    while(!pq.empty()) {
        auto [t, u] = pq.top();
        pq.pop();

        for(auto [v, w]: adj[u]) {
            int first_time = first_time_same_color(u, v, t);
            if(first_time == -1) {
                continue;
            }

            int new_time = first_time + w;
            if(new_time < dist[v]) {
                parent[v] = u;
                dist[v] = new_time;
                pq.push({new_time, v});
            }
        }
    }

    if(dist[destination] == INT_MAX) {
        cout << "0\n";
        return;
    }

    vector<int> path;
    int u = destination;
    while(u != -1) {
        path.push_back(u + 1);
        u = parent[u];
    }

    reverse(path.begin(), path.end());
    cout << dist[destination] << '\n';
    cout << path << '\n';
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
