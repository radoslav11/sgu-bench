#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> req(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> req[i][j];
        }
    }
    
    vector<vector<int>> actual(n, vector<int>(m, 0));
    
    vector<tuple<int, int, int>> cells;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cells.emplace_back(req[i][j], i, j);
        }
    }
    
    // Sort in decreasing order of req value
    sort(cells.begin(), cells.end(), [](const auto& a, const auto& b) {
        if (get<0>(a) != get<0>(b)) {
            return get<0>(a) > get<0>(b);
        }
        if (get<1>(a) != get<1>(b)) {
            return get<1>(a) < get<1>(b);
        }
        return get<2>(a) < get<2>(b);
    });
    
    vector<pair<int, int>> order;
    
    // Directions for neighbors (up, down, left, right)
    vector<int> dx = {-1, 1, 0, 0};
    vector<int> dy = {0, 0, -1, 1};
    
    for (auto& [val, i, j] : cells) {
        order.emplace_back(i + 1, j + 1);
        actual[i][j]++;
        
        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];
            if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                if (actual[ni][nj] > 0) {
                    actual[ni][nj]++;
                }
            }
        }
    }
    
    // Check if actual equals req
    bool valid = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (actual[i][j] != req[i][j]) {
                valid = false;
                break;
            }
        }
        if (!valid) break;
    }
    
    if (valid) {
        for (auto& [x, y] : order) {
            cout << x << " " << y << "\n";
        }
    } else {
        cout << "No solution\n";
    }
    
    return 0;
}
