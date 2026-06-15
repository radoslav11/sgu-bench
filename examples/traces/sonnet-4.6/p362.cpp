#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M, is, js;
    cin >> N >> M >> is >> js;
    
    // Convert to 0-indexed
    int r = is - 1, c = js - 1;
    
    bool destroyed[10][10] = {};
    
    // Directions: D, L, U, R
    int dr[] = {1, 0, -1, 0};
    int dc[] = {0, -1, 0, 1};
    char dname[] = {'D', 'L', 'U', 'R'};
    
    string result = "";
    
    while(true){
        // Find next building
        int nr = -1, nc = -1;
        char move = ' ';
        for(int d = 0; d < 4; d++){
            int tr = r + dr[d];
            int tc = c + dc[d];
            if(tr >= 0 && tr < N && tc >= 0 && tc < M && !destroyed[tr][tc]){
                nr = tr; nc = tc;
                move = dname[d];
                break;
            }
        }
        
        if(nr == -1){
            // Robot explodes with current building
            destroyed[r][c] = true;
            break;
        }
        
        // Move
        destroyed[r][c] = true;
        result += move;
        r = nr;
        c = nc;
    }
    
    cout << result << endl;
    
    return 0;
}
