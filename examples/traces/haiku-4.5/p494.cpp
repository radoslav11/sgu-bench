#include<bits/stdc++.h>
using namespace std;

int simulate(int N, int W, int R, int C, vector<int>& L, int illRow, int illCol) {
    vector<pair<int,int>> pos(N);
    int prevRow = 0, prevCol = 0;
    int maxRow = 0;
    
    for(int i = 0; i < N; i++) {
        bool placed = false;
        for(int row = prevRow; row <= 200 && !placed; row++) {
            int startCol = (row == prevRow) ? prevCol : 0;
            for(int col = startCol; col <= W - L[i]; col++) {
                bool ok = true;
                // Check illustration overlap
                for(int c = col; c < col + L[i] && ok; c++) {
                    if(c >= illCol && c < illCol + C && row >= illRow && row < illRow + R) {
                        ok = false;
                    }
                }
                // Check gap from previous word
                if(ok && i > 0 && row == prevRow && col <= prevCol + L[i-1]) {
                    ok = false;
                }
                
                if(ok) {
                    pos[i] = {row, col};
                    prevRow = row;
                    prevCol = col;
                    maxRow = max(maxRow, row);
                    placed = true;
                    break;
                }
            }
        }
        if(!placed) return 1e9;
    }
    return max(maxRow + 1, illRow + R);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while(T--) {
        int N, W, R, C;
        cin >> N >> W >> R >> C;
        vector<int> L(N);
        for(int i = 0; i < N; i++) cin >> L[i];
        
        int ans = 1e9;
        for(int r = 0; r <= 100; r++) {
            for(int c = 0; c <= W - C; c++) {
                ans = min(ans, simulate(N, W, R, C, L, r, c));
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
