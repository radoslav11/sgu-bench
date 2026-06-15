#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int child;
    int id;
    int w; // protected = +1, almost protected = -1
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<Edge>> tree(N + 1);
    vector<int> weight(N), depthEdge(N, 0);
    vector<char> isAlmost(N, false), selected(N, false);

    for (int i = 1; i <= N - 1; ++i) {
        int u, v;
        string s, t;
        cin >> u >> v >> s;
        int w;
        if (s == "almost") {
            cin >> t; // "protected"
            w = -1;
            isAlmost[i] = true;
        } else {
            w = 1;
        }

        // Direction is u -> v, and every node can reach 1,
        // so v is the parent of u in the rooted tree.
        tree[v].push_back({u, i, w});
        weight[i] = w;
    }

    vector<int> answer;
    set<pair<int, int>> available; // {depth, edge id}, unselected almost edges on current path

    long long sum = 0;       // original sum on current root-to-node path
    int selectedCnt = 0;     // selected edges on current path

    struct Event {
        int type; // 0 = enter, 1 = exit
        int node;
        int edgeId;
        int depth;
    };

    vector<Event> st;
    st.push_back({0, 1, 0, 0});

    while (!st.empty()) {
        Event cur = st.back();
        st.pop_back();

        if (cur.type == 0) {
            int node = cur.node;
            int eid = cur.edgeId;
            int depth = cur.depth;

            if (eid != 0) {
                sum += weight[eid];
                depthEdge[eid] = depth;

                if (isAlmost[eid]) {
                    if (selected[eid]) selectedCnt++;
                    else available.insert({depth, eid});
                }
            }

            int need = 0;
            if (sum < 0) need = int((-sum + 1) / 2);

            while (selectedCnt < need) {
                auto it = available.begin();
                int chooseId = it->second;
                available.erase(it);

                selected[chooseId] = true;
                selectedCnt++;
                answer.push_back(chooseId);
            }

            st.push_back({1, node, eid, depth});

            for (auto &e : tree[node]) {
                st.push_back({0, e.child, e.id, depth + 1});
            }
        } else {
            int eid = cur.edgeId;

            if (eid != 0) {
                if (isAlmost[eid]) {
                    if (selected[eid]) {
                        selectedCnt--;
                    } else {
                        available.erase({depthEdge[eid], eid});
                    }
                }

                sum -= weight[eid];
            }
        }
    }

    cout << answer.size() << '\n';
    for (size_t i = 0; i < answer.size(); ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
