#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

bool overlap(int y1, int y2, int y3, int y4) {
    int min1 = min(y1, y2), max1 = max(y1, y2);
    int min2 = min(y3, y4), max2 = max(y3, y4);
    return max(min1, min2) <= min(max1, max2);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> y(n);
    for (int i = 0; i < n; i++) {
        cin >> y[i];
    }
    
    if (n <= 2) {
        cout << "YES\n";
        return 0;
    }
    
    int num_segments = n - 1;
    vector<vector<bool>> vis(num_segments, vector<bool>(num_segments, false));
    queue<pair<int, int>> q;
    
    q.push({0, num_segments - 1});
    vis[0][num_segments - 1] = true;
    
    int dr[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dc[] = {0, 0, -1, 1, -1, -1, 1, 1};
    
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        
        if (r == c || r == c + 1 || c == r + 1) {
            cout << "YES\n";
            return 0;
        }
        
        for (int i = 0; i < 8; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr >= 0 && nr < num_segments && nc >= 0 && nc < num_segments) {
                if (!vis[nr][nc]) {
                    if (overlap(y[nr], y[nr+1], y[nc], y[nc+1])) {
                        vis[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        }
    }
    
    cout << "NO\n";
    return 0;
}
