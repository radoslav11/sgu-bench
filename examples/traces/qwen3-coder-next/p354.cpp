#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> top(n, vector<int>(n));
    vector<vector<int>> left(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> top[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> left[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (top[i][j] > i || left[i][j] > j) {
                cout << 0 << endl;
                return 0;
            }
        }
    }

    vector<vector<int>> A(n, vector<int>(n, 0));
    vector<vector<bool>> used(n, vector<bool>(n, false));
    vector<vector<int>> row_prefix(n, vector<int>(n+1, 0));
    vector<vector<int>> col_prefix(n, vector<int>(n+1, 0));

    for (int value = n * n; value >= 1; value--) {
        bool found = false;
        for (int i = n - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (used[i][j]) 
                    continue;
                int left_count = row_prefix[i][j];
                int top_count = col_prefix[j][i];
                if (left_count == left[i][j] && top_count == top[i][j]) {
                    A[i][j] = value;
                    used[i][j] = true;
                    for (int j2 = j + 1; j2 <= n; j2++) {
                        row_prefix[i][j2]++;
                    }
                    for (int i2 = i + 1; i2 <= n; i2++) {
                        col_prefix[j][i2]++;
                    }
                    found = true;
                    break;
                }
            }
            if (found) 
                break;
        }
        if (!found) {
            cout << 0 << endl;
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) 
                cout << " ";
            cout << A[i][j];
        }
        cout << "\n";
    }

    return 0;
}
