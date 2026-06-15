#include <iostream>
#include <vector>
using namespace std;

int N;
int count[4];
int up_edge[5][5][3];
int down_edge[5][5][3];

vector<tuple<int, int, char>> order;

int dfs(int idx) {
    if (idx == (int)order.size()) {
        return 1;
    }
    int i = get<0>(order[idx]);
    int j = get<1>(order[idx]);
    char type_tri = get<2>(order[idx]);
    int ways = 0;

    for (int t = 0; t < 4; t++) {
        if (count[t] == 0) continue;
        count[t]--;

        vector<vector<int>> orientations;
        if (t == 0) {
            orientations.push_back({0, 0, 0});
        } else {
            orientations.push_back({1, 1, 0});
            orientations.push_back({1, 0, 1});
            orientations.push_back({0, 1, 1});
        }

        for (auto& orient : orientations) {
            bool valid = true;
            if (type_tri == 'U') {
                if (i > 0 && j > 0 && down_edge[i-1][j-1][2] != -1) {
                    if (orient[1] != down_edge[i-1][j-1][2]) {
                        valid = false;
                    }
                }
                if (valid && i > 0 && j < i && down_edge[i-1][j][1] != -1) {
                    if (orient[2] != down_edge[i-1][j][1]) {
                        valid = false;
                    }
                }
            } else {
                if (up_edge[i][j][0] != -1) {
                    if (orient[0] != up_edge[i][j][0]) {
                        valid = false;
                    }
                }
                if (valid && up_edge[i+1][j][2] != -1) {
                    if (orient[1] != up_edge[i+1][j][2]) {
                        valid = false;
                    }
                }
                if (valid && up_edge[i+1][j+1][1] != -1) {
                    if (orient[2] != up_edge[i+1][j+1][1]) {
                        valid = false;
                    }
                }
            }

            if (valid) {
                if (type_tri == 'U') {
                    for (int k = 0; k < 3; k++) {
                        up_edge[i][j][k] = orient[k];
                    }
                } else {
                    for (int k = 0; k < 3; k++) {
                        down_edge[i][j][k] = orient[k];
                    }
                }

                ways += dfs(idx + 1);

                if (type_tri == 'U') {
                    for (int k = 0; k < 3; k++) {
                        up_edge[i][j][k] = -1;
                    }
                } else {
                    for (int k = 0; k < 3; k++) {
                        down_edge[i][j][k] = -1;
                    }
                }
            }
        }
        count[t]++;
    }
    return ways;
}

int main() {
    cin >> N;
    cin >> count[0] >> count[1] >> count[2] >> count[3];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            order.push_back(make_tuple(i, j, 'U'));
        }
    }
    for (int i = 0; i < N-1; i++) {
        for (int j = 0; j <= i; j++) {
            order.push_back(make_tuple(i, j, 'D'));
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 3; k++) {
                up_edge[i][j][k] = -1;
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 3; k++) {
                down_edge[i][j][k] = -1;
            }
        }
    }

    int result = dfs(0);
    cout << result << endl;

    return 0;
}
