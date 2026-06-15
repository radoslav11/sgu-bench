#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<vector<int>> schedule(n, vector<int>(n, 0));
    
    if(n == 1){
        cout << 1 << "\n";
        cout << 0 << "\n";
        return 0;
    }
    
    int num_tours = (n % 2 == 0) ? (n - 1) : n;
    
    if(n % 2 == 0){
        // Even case: use rotation algorithm
        // Players 0 to n-2 rotate, player n-1 is fixed
        vector<int> players;
        for(int i = 0; i < n - 1; i++) players.push_back(i);
        
        int fixed = n - 1;
        
        for(int round = 0; round < n - 1; round++){
            int tour = round + 1;
            
            // Match fixed player with players[0]
            schedule[fixed][players[0]] = tour;
            schedule[players[0]][fixed] = tour;
            
            // Match other players
            for(int i = 1; i < n - 1; i += 2){
                int p1 = players[i];
                int p2 = players[n - 2 - i];
                schedule[p1][p2] = tour;
                schedule[p2][p1] = tour;
            }
            
            // Rotate (keep fixed player at position, rotate others)
            int last = players[n - 2];
            for(int i = n - 2; i > 1; i--){
                players[i] = players[i - 1];
            }
            players[1] = last;
        }
    } else {
        // Odd case: add dummy player, solve for n+1
        vector<int> players;
        for(int i = 0; i < n; i++) players.push_back(i);
        
        int dummy = n;
        
        for(int round = 0; round < n; round++){
            int tour = round + 1;
            
            // Match fixed player (dummy initially) with players[0]
            if(players[0] != dummy && players[n-1] != dummy){
                schedule[players[0]][players[n-1]] = tour;
                schedule[players[n-1]][players[0]] = tour;
            }
            
            // Match other players
            for(int i = 1; i < n - 1; i += 2){
                int p1 = players[i];
                int p2 = players[n - 1 - i];
                if(p1 != dummy && p2 != dummy){
                    schedule[p1][p2] = tour;
                    schedule[p2][p1] = tour;
                }
            }
            
            // Rotate
            int last = players[n - 1];
            for(int i = n - 1; i > 0; i--){
                players[i] = players[i - 1];
            }
            players[0] = last;
        }
    }
    
    cout << num_tours << "\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << schedule[i][j];
            if(j < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
