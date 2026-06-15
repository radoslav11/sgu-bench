#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int from, to;
    long long w;
};

struct ACNode {
    vector<pair<int,int>> child; // edge_id -> node
    vector<pair<int,int>> memo;  // edge_id -> go result
    int fail = 0;
    int endVertex = 0;
    long long out = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, r, S, T;
    if (!(cin >> n >> m >> r >> S >> T)) return 0;

    vector<Edge> edges(m + 1);
    vector<vector<int>> adj(n + 1);

    for (int i = 1; i <= m; ++i) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        edges[i] = {a, b, c};
        adj[a].push_back(i);
    }

    vector<ACNode> ac(1);

    auto findChild = [&](int v, int eid) -> int {
        for (auto [e, to] : ac[v].child) {
            if (e == eid) return to;
        }
        return -1;
    };

    for (int i = 0; i < r; ++i) {
        int k;
        cin >> k;

        int cur = 0;
        long long routeCost = 0;

        for (int j = 0; j < k; ++j) {
            int eid;
            cin >> eid;
            routeCost += edges[eid].w;

            int nxt = -1;
            for (auto [e, to] : ac[cur].child) {
                if (e == eid) {
                    nxt = to;
                    break;
                }
            }

            if (nxt == -1) {
                nxt = (int)ac.size();
                ac.push_back(ACNode());
                ac[nxt].endVertex = edges[eid].to;
                ac[cur].child.push_back({eid, nxt});
            }

            cur = nxt;
        }

        ac[cur].out += routeCost;
    }

    queue<int> q;

    for (auto [eid, to] : ac[0].child) {
        ac[to].fail = 0;
        q.push(to);
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        ac[v].out += ac[ac[v].fail].out;

        for (auto [eid, to] : ac[v].child) {
            int f = ac[v].fail;
            while (f != 0 && findChild(f, eid) == -1) {
                f = ac[f].fail;
            }

            int go = findChild(f, eid);
            if (go == -1) go = 0;

            ac[to].fail = go;
            q.push(to);
        }
    }

    auto getGo = [&](int start, int eid) {
        vector<int> visited;
        int v = start;
        int result = 0;

        while (true) {
            bool foundMemo = false;
            for (auto [e, to] : ac[v].memo) {
                if (e == eid) {
                    result = to;
                    foundMemo = true;
                    break;
                }
            }
            if (foundMemo) break;

            int ch = findChild(v, eid);
            if (ch != -1) {
                result = ch;
                break;
            }

            if (v == 0) {
                result = 0;
                break;
            }

            visited.push_back(v);
            v = ac[v].fail;
        }

        for (int x : visited) {
            ac[x].memo.push_back({eid, result});
        }

        return result;
    };

    int acCnt = (int)ac.size();
    int totalStates = n + acCnt - 1;

    auto stateId = [&](int vertex, int acState) {
        if (acState == 0) return vertex - 1;
        return n + acState - 1;
    };

    auto getVertex = [&](int id) {
        if (id < n) return id + 1;
        int acState = id - n + 1;
        return ac[acState].endVertex;
    };

    auto getACState = [&](int id) {
        if (id < n) return 0;
        return id - n + 1;
    };

    const long long INF = (1LL << 62);

    vector<long long> dist(totalStates, INF);
    vector<int> parent(totalStates, -1);
    vector<int> parentEdge(totalStates, -1);

    int start = stateId(S, 0);
    dist[start] = 0;

    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    pq.push({0, start});

    int finishState = -1;

    while (!pq.empty()) {
        auto [d, id] = pq.top();
        pq.pop();

        if (d != dist[id]) continue;

        int v = getVertex(id);
        int acState = getACState(id);

        if (v == T) {
            finishState = id;
            break;
        }

        for (int eid : adj[v]) {
            int nac = getGo(acState, eid);
            int toVertex = edges[eid].to;
            int nid = stateId(toVertex, nac);

            long long nd = d + edges[eid].w + ac[nac].out;

            if (nd < dist[nid]) {
                dist[nid] = nd;
                parent[nid] = id;
                parentEdge[nid] = eid;
                pq.push({nd, nid});
            }
        }
    }

    if (finishState == -1) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> path;
    int cur = finishState;

    while (cur != start) {
        path.push_back(parentEdge[cur]);
        cur = parent[cur];
    }

    reverse(path.begin(), path.end());

    cout << dist[finishState] << '\n';
    cout << path.size() << '\n';
    for (int eid : path) {
        cout << eid << ' ';
    }
    cout << '\n';

    return 0;
}
