#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<string> beauty(n);
    for (int i = 0; i < n; i++) {
        cin >> beauty[i];
    }

    string path;
    cin >> path;

    int r = 0, c = 0;
    int dir = 0; // 0: East, 1: South, 2: West, 3: North

    vector<vector<int>> vis_count(n, vector<int>(m, 0));
    int satisfaction = 0;

    for (char move : path) {
        if (move == 'L') {
            dir = (dir + 3) % 4;
        } else if (move == 'R') {
            dir = (dir + 1) % 4;
        } else if (move == 'M') {
            int next_r = r;
            int next_c = c;

            if (dir == 0) { // East
                next_c = c + 1;
                // block below: r, c
                if (r < n && c < m) {
                    vis_count[r][c]++;
                    int b = beauty[r][c] - '0';
                    if (vis_count[r][c] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
                // block above: r-1, c
                if (r - 1 >= 0 && c < m) {
                    vis_count[r-1][c]++;
                    int b = beauty[r-1][c] - '0';
                    if (vis_count[r-1][c] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
            } else if (dir == 1) { // South
                next_r = r + 1;
                // block right: r, c
                if (r < n && c < m) {
                    vis_count[r][c]++;
                    int b = beauty[r][c] - '0';
                    if (vis_count[r][c] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
                // block left: r, c-1
                if (r < n && c - 1 >= 0) {
                    vis_count[r][c-1]++;
                    int b = beauty[r][c-1] - '0';
                    if (vis_count[r][c-1] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
            } else if (dir == 2) { // West
                next_c = c - 1;
                // block below: r, c-1
                if (r < n && c - 1 >= 0) {
                    vis_count[r][c-1]++;
                    int b = beauty[r][c-1] - '0';
                    if (vis_count[r][c-1] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
                // block above: r-1, c-1
                if (r - 1 >= 0 && c - 1 >= 0) {
                    vis_count[r-1][c-1]++;
                    int b = beauty[r-1][c-1] - '0';
                    if (vis_count[r-1][c-1] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
            } else if (dir == 3) { // North
                next_r = r - 1;
                // block right: r-1, c
                if (r - 1 >= 0 && c < m) {
                    vis_count[r-1][c]++;
                    int b = beauty[r-1][c] - '0';
                    if (vis_count[r-1][c] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
                // block left: r-1, c-1
                if (r - 1 >= 0 && c - 1 >= 0) {
                    vis_count[r-1][c-1]++;
                    int b = beauty[r-1][c-1] - '0';
                    if (vis_count[r-1][c-1] == 1) satisfaction += b;
                    else satisfaction += b / 2;
                }
            }

            r = next_r;
            c = next_c;
        }
    }

    cout << satisfaction << "\n";

    return 0;
}
