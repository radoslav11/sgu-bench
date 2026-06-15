#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int h, w;
    cin >> h >> w;
    
    vector<string> grid(h);
    for(int i = 0; i < h; i++){
        cin >> grid[i];
    }
    
    // Try sizes from largest to smallest
    for(int size = min(h, w); size >= 1; size--){
        // Try all positions
        for(int r = 0; r + size <= h; r++){
            for(int c = 0; c + size <= w; c++){
                // Check if square from (r,c) to (r+size-1, c+size-1) is palindrome square
                bool valid = true;
                
                // Check all positions
                for(int i = 0; i < size && valid; i++){
                    for(int j = 0; j < size && valid; j++){
                        // Check if grid[r+i][c+j] matches the palindrome requirements
                        // It should match:
                        // - grid[r+i][c+size-1-j] (same row, mirrored column)
                        // - grid[r+size-1-i][c+j] (mirrored row, same column)
                        // - grid[r+size-1-i][c+size-1-j] (mirrored row, mirrored column)
                        
                        char val = grid[r+i][c+j];
                        if(grid[r+i][c+size-1-j] != val) valid = false;
                        if(grid[r+size-1-i][c+j] != val) valid = false;
                        if(grid[r+size-1-i][c+size-1-j] != val) valid = false;
                    }
                }
                
                if(valid){
                    // Found a valid square, output 1-indexed coordinates
                    cout << r+1 << " " << c+1 << " " << r+size << " " << c+size << "\n";
                    return 0;
                }
            }
        }
    }
    
    return 0;
}
