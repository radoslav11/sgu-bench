#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    
    vector<vector<bool>> rem(n, vector<bool>(n, true));
    int current_c = 0;
    int k = 101;
    
    int steps = 0;
    while (steps <= 250) {
        int new_c = 1 - current_c;
        vector<pair<int, int>> to_remove;
        
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (!rem[r][c]) continue;
                if (r == 0 && c == 0) continue; // Never remove the target (0,0)
                if ((r + c) % 2 != new_c) continue;
                
                int dr[] = {-1, 1, 0, 0};
                int dc[] = {0, 0, -1, 1};
                bool safe = true;
                
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n && rem[nr][nc]) {
                        int rem_neighbors = 0;
                        for (int j = 0; j < 4; ++j) {
                            int nnr = nr + dr[j], nnc = nc + dc[j];
                            if (nnr >= 0 && nnr < n && nnc >= 0 && nnc < n && rem[nnr][nnc]) {
                                if (!(nnr == r && nnc == c)) {
                                    rem_neighbors++;
                                }
                            }
                        }
                        if (rem_neighbors == 0) {
                            safe = false;
                            break;
                        }
                    }
                }
                
                if (safe) {
                    to_remove.push_back({r, c});
                }
            }
        }
        
        if (to_remove.empty()) {
            break;
        }
        
        cout << k;
        for (auto& p : to_remove) {
            cout << " " << p.first * n + p.second + 1;
            rem[p.first][p.second] = false;
        }
        cout << "\n";
        
        current_c = new_c;
        k += 2;
        steps++;
    }
    
    return 0;
}
