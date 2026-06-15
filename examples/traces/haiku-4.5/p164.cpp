#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N, vector<int>(N));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cin >> adj[i][j];
        }
    }
    
    int maxBuy = (M + 1) / 2;
    
    // Try all subsets of airlines
    for(int mask = 1; mask < (1 << M); mask++){
        int count = __builtin_popcount(mask);
        if(count > maxBuy) continue;
        
        // Check if this subset works
        // Build graph with only selected airlines
        vector<vector<int>> dist(N, vector<int>(N, 1e9));
        for(int i = 0; i < N; i++) dist[i][i] = 0;
        
        for(int i = 0; i < N; i++){
            for(int j = i+1; j < N; j++){
                int airline = adj[i][j];
                if(mask & (1 << (airline - 1))){
                    dist[i][j] = dist[j][i] = 1;
                }
            }
        }
        
        // Floyd-Warshall
        for(int k = 0; k < N; k++){
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        
        // Check if all distances <= 3
        bool valid = true;
        for(int i = 0; i < N && valid; i++){
            for(int j = 0; j < N && valid; j++){
                if(dist[i][j] > 3){
                    valid = false;
                }
            }
        }
        
        if(valid){
            cout << count << "\n";
            for(int i = 0; i < M; i++){
                if(mask & (1 << i)){
                    cout << (i + 1) << " ";
                }
            }
            cout << "\n";
            return 0;
        }
    }
    
    cout << "-1\n";
    return 0;
}
