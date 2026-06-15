#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    // Create grid, initially all white (true = white, false = black)
    vector<vector<bool>> grid(N + 1, vector<bool>(N + 1, true));
    
    for(int i = 0; i < M; i++){
        int x1, y1, x2, y2;
        char c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;
        
        // Normalize coordinates so x1 <= x2 and y1 <= y2
        if(x1 > x2) swap(x1, x2);
        if(y1 > y2) swap(y1, y2);
        
        // Paint the rectangle
        bool color = (c == 'w'); // true for white, false for black
        for(int x = x1; x <= x2; x++){
            for(int y = y1; y <= y2; y++){
                grid[x][y] = color;
            }
        }
    }
    
    // Count white cells
    int count = 0;
    for(int x = 1; x <= N; x++){
        for(int y = 1; y <= N; y++){
            if(grid[x][y]){
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
