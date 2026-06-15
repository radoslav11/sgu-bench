#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    
    vector<string> grid(n);
    int start_row = -1, start_col = -1;
    
    for(int i = 0; i < n; i++){
        cin >> grid[i];
        for(int j = 0; j < m; j++){
            if(grid[i][j] == 'P'){
                start_row = i;
                start_col = j;
            }
        }
    }
    
    int row = start_row;
    int col = start_col;
    char last_obstacle = '.';
    
    while(true){
        // Try to fall down
        if(row + 1 >= n){
            cout << col + 1 << endl;
            return 0;
        }
        
        char below = grid[row + 1][col];
        
        if(below == '.'){
            row++;
        }
        else if(below == '/' || below == '\\'){
            if(last_obstacle != '.' && last_obstacle != below){
                cout << -1 << endl;
                return 0;
            }
            
            last_obstacle = below;
            row++;
            
            if(below == '/'){
                while(col > 0 && (grid[row][col-1] == '.' || grid[row][col-1] == '/')){
                    col--;
                }
            }
            else{
                while(col < m - 1 && (grid[row][col+1] == '.' || grid[row][col+1] == '\\')){
                    col++;
                }
            }
        }
    }
    
    return 0;
}
