#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    int area = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int h = grid[i][j] - '0';
            if (h > 0) {
                area += 2; // top and bottom facets
                
                // up
                if (i == 0) area += h;
                else area += max(0, h - (grid[i-1][j] - '0'));
                
                // down
                if (i == n - 1) area += h;
                else area += max(0, h - (grid[i+1][j] - '0'));
                
                // left
                if (j == 0) area += h;
                else area += max(0, h - (grid[i][j-1] - '0'));
                
                // right
                if (j == m - 1) area += h;
                else area += max(0, h - (grid[i][j+1] - '0'));
            }
        }
    }

    cout << area << "\n";
    return 0;
}
