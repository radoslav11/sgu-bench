#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> graph(N + 1);

    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // color[i] = -1 means unvisited, 0 and 1 are the two colors
    vector<int> color(N + 1, -1);
    bool isBipartite = true;

    for (int i = 1; i <= N; i++) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;

            while (!q.empty() && isBipartite) {
                int u = q.front();
                q.pop();

                for (int v : graph[u]) {
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        isBipartite = false;
                        break;
                    }
                }
            }

            if (!isBipartite) break;
        }
    }

    if (!isBipartite) {
        cout << "no" << endl;
    } else {
        vector<int> day1;
        for (int i = 1; i <= N; i++) {
            if (color[i] == 0) {
                day1.push_back(i);
            }
        }

        cout << "yes" << endl;
        cout << day1.size() << endl;
        if (!day1.empty()) {
            for (int i = 0; i < day1.size(); i++) {
                if (i > 0) cout << " ";
                cout << day1[i];
            }
            cout << endl;
        } else {
            cout << endl;
        }
    }

    return 0;
}
