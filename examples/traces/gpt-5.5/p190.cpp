#include <bits/stdc++.h>
using namespace std;

int N, P;
bool removedCell[41][41];

vector<pair<int,int>> blackCells;
vector<vector<int>> adjList;
vector<int> matchWhite;
vector<int> used;

int cellId(int x, int y) {
    return (x - 1) * N + (y - 1);
}

pair<int,int> cellCoord(int id) {
    return {id / N + 1, id % N + 1};
}

bool dfs(int v) {
    if (used[v]) return false;
    used[v] = true;

    for (int w : adjList[v]) {
        if (matchWhite[w] == -1 || dfs(matchWhite[w])) {
            matchWhite[w] = v;
            return true;
        }
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> P;

    for (int i = 0; i < P; ++i) {
        int x, y;
        cin >> x >> y;
        removedCell[x][y] = true;
    }

    int blackCount = 0, whiteCount = 0;

    for (int x = 1; x <= N; ++x) {
        for (int y = 1; y <= N; ++y) {
            if (removedCell[x][y]) continue;

            if ((x + y) % 2 == 0) {
                blackCells.push_back({x, y});
                blackCount++;
            } else {
                whiteCount++;
            }
        }
    }

    int freeCells = N * N - P;

    if (freeCells % 2 == 1 || blackCount != whiteCount) {
        cout << "No\n";
        return 0;
    }

    adjList.resize(blackCells.size());

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int i = 0; i < (int)blackCells.size(); ++i) {
        int x = blackCells[i].first;
        int y = blackCells[i].second;

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 1 || nx > N || ny < 1 || ny > N) continue;
            if (removedCell[nx][ny]) continue;

            adjList[i].push_back(cellId(nx, ny));
        }
    }

    matchWhite.assign(N * N, -1);

    int matching = 0;

    for (int i = 0; i < (int)blackCells.size(); ++i) {
        used.assign(blackCells.size(), 0);
        if (dfs(i)) matching++;
    }

    if (matching != freeCells / 2) {
        cout << "No\n";
        return 0;
    }

    vector<pair<int,int>> horizontal;
    vector<pair<int,int>> vertical;

    for (int w = 0; w < N * N; ++w) {
        if (matchWhite[w] == -1) continue;

        auto [wx, wy] = cellCoord(w);
        auto [bx, by] = blackCells[matchWhite[w]];

        if (by == wy) {
            horizontal.push_back({min(bx, wx), by});
        } else {
            vertical.push_back({bx, min(by, wy)});
        }
    }

    cout << "Yes\n";

    cout << horizontal.size() << '\n';
    for (auto [x, y] : horizontal) {
        cout << x << ' ' << y << '\n';
    }

    cout << vertical.size() << '\n';
    for (auto [x, y] : vertical) {
        cout << x << ' ' << y << '\n';
    }

    return 0;
}
