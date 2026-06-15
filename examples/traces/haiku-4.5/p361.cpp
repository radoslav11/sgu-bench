#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    // Check if solution is possible
    // The pattern repeats every 3x3, so we need both dimensions to allow valid patterns
    vector<vector<char>> grid(N, vector<char>(M, '0'));
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            int ii = i % 3;
            int jj = j % 3;
            
            // Pattern: blue at (0,0), (1,2), (2,1) in each 3x3 block
            if((ii == 0 && jj == 0) || (ii == 1 && jj == 2) || (ii == 2 && jj == 1)){
                grid[i][j] = '#';
            }
        }
    }
    
    // Verify the solution
    bool valid = true;
    
    // Check all 3x2 rectangles
    for(int i = 0; i + 3 <= N && valid; i++){
        for(int j = 0; j + 2 <= M && valid; j++){
            int count = 0;
            for(int di = 0; di < 3; di++){
                for(int dj = 0; dj < 2; dj++){
                    if(grid[i+di][j+dj] == '#') count++;
                }
            }
            if(count != 2) valid = false;
        }
    }
    
    // Check all 2x3 rectangles
    for(int i = 0; i + 2 <= N && valid; i++){
        for(int j = 0; j + 3 <= M && valid; j++){
            int count = 0;
            for(int di = 0; di < 2; di++){
                for(int dj = 0; dj < 3; dj++){
                    if(grid[i+di][j+dj] == '#') count++;
                }
            }
            if(count != 2) valid = false;
        }
    }
    
    if(!valid){
        cout << "No solution" << endl;
    } else {
        for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                cout << grid[i][j];
            }
            cout << endl;
        }
    }
    
    return 0;
}
