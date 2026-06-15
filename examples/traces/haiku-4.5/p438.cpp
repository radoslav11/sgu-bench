#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, D, W;
    cin >> N >> M >> D >> W;
    
    vector<int> X(N), Y(N), C(N);
    for(int i = 0; i < N; i++){
        cin >> X[i] >> Y[i] >> C[i];
    }
    
    map<vector<int>, int> visited;
    queue<pair<vector<int>, int>> q;
    
    vector<int> initial(N+2, 0);
    initial[0] = M;
    q.push({initial, 0});
    visited[initial] = 0;
    
    while(!q.empty()){
        auto [state, time] = q.front();
        q.pop();
        
        if(state[N+1] == M){
            cout << time << endl;
            return 0;
        }
        
        for(int from = 0; from <= N+1; from++){
            if(state[from] == 0) continue;
            
            for(int to = 0; to <= N+1; to++){
                if(from == to) continue;
                
                double fromY = (from == 0) ? 0 : (from == N+1) ? W : Y[from-1];
                double toY = (to == 0) ? 0 : (to == N+1) ? W : Y[to-1];
                
                if(abs(toY - fromY) > D) continue;
                
                int canMove = state[from];
                if(to < N+1) canMove = min(canMove, C[to] - state[to]);
                
                if(canMove <= 0) continue;
                
                vector<int> newState = state;
                newState[from] -= canMove;
                newState[to] += canMove;
                
                if(visited.find(newState) == visited.end()){
                    visited[newState] = time + 1;
                    q.push({newState, time + 1});
                }
            }
        }
    }
    
    cout << "IMPOSSIBLE" << endl;
    return 0;
}
