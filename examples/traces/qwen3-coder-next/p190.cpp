#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool dfs(int x, int y, vector<vector<bool>>& vis, 
        int d[4][2], 
        const vector<vector<bool>>& bad, int N,
        int blackMatchX[50][50], int blackMatchY[50][50],
        int whiteMatchX[50][50], int whiteMatchY[50][50]) {
    for (int i = 0; i < 4; i++) {
        int nx = x + d[i][0];
        int ny = y + d[i][1];
        if (nx < 1 || nx > N || ny < 1 || ny > N) 
            continue;
        if (bad[nx][ny]) 
            continue;
        if ((nx + ny) % 2 != 1) 
            continue;

        if (vis[nx][ny]) 
            continue;
        vis[nx][ny] = true;

        if (whiteMatchX[nx][ny] == 0 && whiteMatchY[nx][ny] == 0) {
            blackMatchX[x][y] = nx;
            blackMatchY[x][y] = ny;
            whiteMatchX[nx][ny] = x;
            whiteMatchY[nx][ny] = y;
            return true;
        } else {
            int bx = whiteMatchX[nx][ny];
            int by = whiteMatchY[nx][ny];
            if (dfs(bx, by, vis, d, bad, N, blackMatchX, blackMatchY, whiteMatchX, whiteMatchY)) {
                blackMatchX[x][y] = nx;
                blackMatchY[x][y] = ny;
                whiteMatchX[nx][ny] = x;
                whiteMatchY[nx][ny] = y;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int N, P;
    cin >> N >> P;

    vector<vector<bool>> bad(N+1, vector<bool>(N+1, false));
    int total = N * N - P;
    if (total % 2 != 0) {
        cout << "No" << endl;
        return 0;
    }

    for (int i = 0; i < P; i++) {
        int x, y;
        cin >> x >> y;
        bad[x][y] = true;
    }

    int blackMatchX[50][50] = {{0}};
    int blackMatchY[50][50] = {{0}};
    int whiteMatchX[50][50] = {{0}};
    int whiteMatchY[50][50] = {{0}};

    int d[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    int count = 0;
    for (int x = 1; x <= N; x++) {
        for (int y = 1; y <= N; y++) {
            if ((x + y) % 2 == 0 && !bad[x][y]) {
                vector<vector<bool>> vis(N+1, vector<bool>(N+1, false));
                if (dfs(x, y, vis, d, bad, N, blackMatchX, blackMatchY, whiteMatchX, whiteMatchY)) {
                    count++;
                }
            }
        }
    }

    if (count * 2 != total) {
        cout << "No" << endl;
        return 0;
    }

    vector<pair<int, int>> hor, ver;
    for (int x = 1; x <= N; x++) {
        for (int y = 1; y <= N; y++) {
            if ((x + y) % 2 == 0 && !bad[x][y] && blackMatchX[x][y] != 0) {
                int nx = blackMatchX[x][y];
                int ny = blackMatchY[x][y];
                if (ny == y) {
                    int left_x = min(x, nx);
                    hor.push_back(make_pair(left_x, y));
                } else if (nx == x) {
                    int bottom_y = min(y, ny);
                    ver.push_back(make_pair(x, bottom_y));
                }
            }
        }
    }

    cout << "Yes" << endl;
    cout << hor.size() << endl;
    for (auto p : hor) {
        cout << p.first << " " << p.second << endl;
    }
    cout << ver.size() << endl;
    for (auto p : ver) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}
