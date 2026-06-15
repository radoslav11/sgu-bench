#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> grid(N, vector<int>(M));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> grid[i][j];
        }
    }
    
    int maxArea = -1;
    vector<vector<int>> bestGrid = grid;
    
    // Try all possible configurations
    for(int col = 0; col < M; col++){
        // w1 = width of top and bottom blocks
        // w2 = width of middle block (w2 < w1)
        // h1 = height of top block
        // h2 = height of middle block
        // h3 = height of bottom block
        
        for(int w1 = 1; col + w1 <= M; w1++){
            for(int w2 = 1; w2 < w1 && col + w2 <= M; w2++){
                for(int h1 = 1; h1 < N; h1++){
                    for(int h2 = 1; h1 + h2 < N; h2++){
                        for(int h3 = 1; h1 + h2 + h3 <= N; h3++){
                            // Check if this configuration is valid
                            vector<vector<int>> testGrid = grid;
                            bool valid = true;
                            
                            // Check and place top block
                            for(int i = 0; i < h1 && valid; i++){
                                for(int j = 0; j < w1 && valid; j++){
                                    if(testGrid[i][col + j] == 1) valid = false;
                                    else testGrid[i][col + j] = 8;
                                }
                            }
                            
                            // Check and place middle block
                            for(int i = h1; i < h1 + h2 && valid; i++){
                                for(int j = 0; j < w2 && valid; j++){
                                    if(testGrid[i][col + j] == 1) valid = false;
                                    else testGrid[i][col + j] = 8;
                                }
                            }
                            
                            // Check and place bottom block
                            for(int i = h1 + h2; i < h1 + h2 + h3 && valid; i++){
                                for(int j = 0; j < w1 && valid; j++){
                                    if(testGrid[i][col + j] == 1) valid = false;
                                    else testGrid[i][col + j] = 8;
                                }
                            }
                            
                            if(valid){
                                int area = h1 * w1 + h2 * w2 + h3 * w1;
                                if(area > maxArea){
                                    maxArea = area;
                                    bestGrid = testGrid;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    if(maxArea == -1){
        cout << -1 << "\n";
    } else {
        cout << maxArea << "\n";
        for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                cout << bestGrid[i][j];
                if(j < M - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
