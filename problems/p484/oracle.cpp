#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    int startR = -1, startC = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'P') {
                startR = i;
                startC = j;
                break;
            }
        }
        if (startR != -1) break;
    }

    int row = startR + 1;
    int col = startC;

    while (row < n) {
        char cell = grid[row][col];
        if (cell == '.') {
            row++;
        } else if (cell == '/') {
            col--;
            if (col < 0 || grid[row][col] == '\\') {
                col = -2;
                break;
            }
            row++;
        } else if (cell == '\\') {
            col++;
            if (col >= m || grid[row][col] == '/') {
                col = -2;
                break;
            }
            row++;
        }
    }

    if (col < 0) {
        cout << -1 << "\n";
    } else {
        cout << (col + 1) << "\n";
    }

    return 0;
}
