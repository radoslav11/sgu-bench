#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

bool dfs(int a, vector<vector<int>>& adjA, vector<int>& matchA, vector<int>& matchB, vector<int>& dist, vector<int>& visited) {
    for (int b : adjA[a]) {
        int next_a = matchB[b];
        if (next_a == -1 || (dist[next_a] == dist[a] + 1 && !visited[next_a] && dfs(next_a, adjA, matchA, matchB, dist, visited))) {
            matchA[a] = b;
            matchB[b] = a;
            return true;
        }
    }
    dist[a] = INF;
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int M, N;
    cin >> M >> N;
    vector<vector<int>> grid(M, vector<int>(N));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    vector<pair<int, int>> cellsA, cellsB;
    vector<vector<int>> inA(M, vector<int>(N, -1));
    vector<vector<int>> inB(M, vector<int>(N, -1));

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1) {
                if ((i + j) % 2 == 0) {
                    inA[i][j] = cellsA.size();
                    cellsA.push_back({i, j});
                } else {
                    inB[i][j] = cellsB.size();
                    cellsB.push_back({i, j});
                }
            }
        }
    }

    int sizeA = cellsA.size();
    int sizeB = cellsB.size();

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    vector<vector<int>> adjA(sizeA);
    for (int idx = 0; idx < sizeA; idx++) {
        int i = cellsA[idx].first;
        int j = cellsA[idx].second;
        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];
            if (ni >= 0 && ni < M && nj >= 0 && nj < N && grid[ni][nj] == 1) {
                int b_idx = inB[ni][nj];
                adjA[idx].push_back(b_idx);
            }
        }
    }

    vector<int> matchA(sizeA, -1);
    vector<int> matchB(sizeB, -1);
    int matching_size = 0;

    while (true) {
        vector<int> dist(sizeA, INF);
        queue<int> Q;
        for (int i = 0; i < sizeA; i++) {
            if (matchA[i] == -1) {
                dist[i] = 0;
                Q.push(i);
            }
        }

        int min_dist = INF;
        while (!Q.empty()) {
            int a = Q.front();
            Q.pop();
            if (dist[a] >= min_dist) {
                continue;
            }
            for (int b : adjA[a]) {
                int next_a = matchB[b];
                if (next_a == -1) {
                    min_dist = dist[a] + 1;
                } else if (dist[next_a] == INF) {
                    dist[next_a] = dist[a] + 1;
                    Q.push(next_a);
                }
            }
        }

        if (min_dist == INF) {
            break;
        }

        vector<int> visited(sizeA, 0);
        for (int i = 0; i < sizeA; i++) {
            if (matchA[i] == -1 && !visited[i] && dfs(i, adjA, matchA, matchB, dist, visited)) {
                matching_size++;
            }
        }
    }

    vector<bool> visitedA(sizeA, false);
    vector<bool> visitedB(sizeB, false);
    queue<pair<int, int>> Q; // (index, type: 0 for A, 1 for B)

    for (int i = 0; i < sizeA; i++) {
        if (matchA[i] == -1) {
            visitedA[i] = true;
            Q.push({i, 0});
        }
    }

    while (!Q.empty()) {
        int idx = Q.front().first;
        int type = Q.front().second;
        Q.pop();
        if (type == 0) {
            for (int b : adjA[idx]) {
                if (matchA[idx] != b) {
                    if (!visitedB[b]) {
                        visitedB[b] = true;
                        Q.push({b, 1});
                    }
                }
            }
        } else {
            if (matchB[idx] != -1) {
                int a_next = matchB[idx];
                if (!visitedA[a_next]) {
                    visitedA[a_next] = true;
                    Q.push({a_next, 0});
                }
            }
        }
    }

    vector<vector<char>> result(M, vector<char>(N, '.'));
    int count = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                result[i][j] = '#';
            } else {
                if ((i + j) % 2 == 0) {
                    int idx = inA[i][j];
                    if (visitedA[idx]) {
                        result[i][j] = 'G';
                        count++;
                    } else {
                        result[i][j] = '.';
                    }
                } else {
                    int idx = inB[i][j];
                    if (!visitedB[idx]) {
                        result[i][j] = 'G';
                        count++;
                    } else {
                        result[i][j] = '.';
                    }
                }
            }
        }
    }

    cout << count << '\n';
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << result[i][j];
        }
        cout << '\n';
    }

    return 0;
}
