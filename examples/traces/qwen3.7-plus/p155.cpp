#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int k, a, id;
    bool operator<(const Node& other) const {
        return k < other.k;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Node> nodes(n);
    for (int i = 0; i < n; ++i) {
        cin >> nodes[i].k >> nodes[i].a;
        nodes[i].id = i + 1;
    }

    sort(nodes.begin(), nodes.end());

    vector<int> parent(n + 1, 0);
    vector<int> left_child(n + 1, 0);
    vector<int> right_child(n + 1, 0);
    vector<int> st;
    st.reserve(n);

    for (int i = 0; i < n; ++i) {
        int u = nodes[i].id;
        int last_popped = 0;
        
        while (!st.empty() && nodes[st.back()].a > nodes[i].a) {
            last_popped = nodes[st.back()].id;
            st.pop_back();
        }

        if (st.empty()) {
            left_child[u] = last_popped;
            if (last_popped != 0) {
                parent[last_popped] = u;
            }
        } else {
            int p = nodes[st.back()].id;
            right_child[p] = u;
            parent[u] = p;
            left_child[u] = last_popped;
            if (last_popped != 0) {
                parent[last_popped] = u;
            }
        }
        st.push_back(i);
    }

    cout << "YES\n";
    for (int i = 1; i <= n; ++i) {
        cout << parent[i] << " " << left_child[i] << " " << right_child[i] << "\n";
    }

    return 0;
}
