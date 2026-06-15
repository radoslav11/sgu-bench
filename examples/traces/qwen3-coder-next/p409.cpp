#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    int size = n * n;
    vector<string> grid(size, string(size, '.'));

    if (k == 0) {
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    } else if (k == size) {
        for (int i = 0; i < size; i++) {
            cout << string(size, '*') << '\n';
        }
        return 0;
    } else if (k == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int r = i * n + j;
                int c = j * n + i;
                grid[r][c] = '*';
            }
        }
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    } else if (k == size - 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int r = i * n + j;
                int c = j * n + i;
                grid[r][c] = '*';
            }
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == '*') {
                    grid[i][j] = '.';
                } else {
                    grid[i][j] = '*';
                }
            }
        }
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    } else if (k == n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int x = 0; x < n; x++) {
                    int y = (x + i + j) % n;
                    grid[i * n + x][j * n + y] = '*';
                }
            }
        }
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    } else if (k == size - n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int x = 0; x < n; x++) {
                    int y = (x + i + j) % n;
                    grid[i * n + x][j * n + y] = '*';
                }
            }
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == '*') {
                    grid[i][j] = '.';
                } else {
                    grid[i][j] = '*';
                }
            }
        }
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int x = 0; x < n; x++) {
                    int y = (x + i + j) % n;
                    if (y < k) {
                        grid[i * n + x][j * n + y] = '*';
                    }
                }
            }
        }
        for (int i = 0; i < size; i++) {
            cout << grid[i] << '\n';
        }
        return 0;
    }
}
