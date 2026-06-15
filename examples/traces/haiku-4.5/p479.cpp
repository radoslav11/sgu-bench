#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> target(n, vector<int>(m));
    vector<vector<int>> current(n, vector<int>(m, 0));
    vector<vector<bool>> planted(n, vector<bool>(m, false));
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> target[i][j];
        }
    }
    
    vector<pair<int,int>> order;
    
    for(int op = 0; op < n * m; op++){
        // Find a cell that can be planted
        int pi = -1, pj = -1;
        
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(!planted[i][j] && current[i][j] == target[i][j] - 1){
                    pi = i;
                    pj = j;
                    break;
                }
            }
            if(pi != -1) break;
        }
        
        if(pi == -1){
            cout << "No solution\n";
            return 0;
        }
        
        // Plant at (pi, pj)
        planted[pi][pj] = true;
        current[pi][pj]++;
        order.push_back({pi+1, pj+1});
        
        // Update adjacent cells
        int di[] = {-1, 1, 0, 0};
        int dj[] = {0, 0, -1, 1};
        
        for(int d = 0; d < 4; d++){
            int ni = pi + di[d];
            int nj = pj + dj[d];
            if(ni >= 0 && ni < n && nj >= 0 && nj < m && current[ni][nj] > 0){
                current[ni][nj]++;
            }
        }
    }
    
    // Verify solution
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(current[i][j] != target[i][j]){
                cout << "No solution\n";
                return 0;
            }
        }
    }
    
    for(auto p : order){
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
