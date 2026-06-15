#include <bits/stdc++.h>
using namespace std;

struct Node {
    int k, a, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Node> nodes(N);
    for (int i = 0; i < N; ++i) {
        cin >> nodes[i].k >> nodes[i].a;
        nodes[i].id = i + 1;
    }

    sort(nodes.begin(), nodes.end(), [](const Node& x, const Node& y) {
        return x.k < y.k;
    });

    vector<int> parent(N + 1, 0), leftChild(N + 1, 0), rightChild(N + 1, 0);
    vector<int> aux(N + 1);

    for (const auto& node : nodes) {
        aux[node.id] = node.a;
    }

    vector<int> st;

    for (const auto& node : nodes) {
        int cur = node.id;
        int last = 0;

        while (!st.empty() && aux[st.back()] > aux[cur]) {
            last = st.back();
            st.pop_back();
        }

        if (!st.empty()) {
            rightChild[st.back()] = cur;
            parent[cur] = st.back();
        }

        if (last != 0) {
            leftChild[cur] = last;
            parent[last] = cur;
        }

        st.push_back(cur);
    }

    cout << "YES\n";
    for (int i = 1; i <= N; ++i) {
        cout << parent[i] << ' ' << leftChild[i] << ' ' << rightChild[i] << '\n';
    }

    return 0;
}
