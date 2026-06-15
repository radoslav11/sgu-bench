#include<bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    
    // Store which pictures are still available
    set<int> available;
    for(int i = 1; i <= N*N; i++){
        available.insert(i);
    }
    
    // Current position of finger
    int row = 0, col = 0;
    
    vector<pair<int, vector<int>>> moves_and_removals;
    
    int K = N;
    int round_num = 0;
    
    while(available.size() > 1){
        round_num++;
        
        // Make K moves - find all reachable positions
        set<pair<int,int>> reachable;
        
        // BFS to find all positions at distance exactly K with correct parity
        queue<tuple<int,int,int>> q;
        set<pair<pair<int,int>,int>> visited;
        
        q.push({row, col, 0});
        visited.insert({{row, col}, 0});
        
        while(!q.empty()){
            auto [r, c, dist] = q.front();
            q.pop();
            
            if(dist == K){
                reachable.insert({r, c});
                continue;
            }
            
            // Try all 4 directions
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            
            for(int d = 0; d < 4; d++){
                int nr = r + dr[d];
                int nc = c + dc[d];
                
                if(nr >= 0 && nr < N && nc >= 0 && nc < N){
                    if(visited.find({{nr, nc}, dist+1}) == visited.end()){
                        visited.insert({{nr, nc}, dist+1});
                        q.push({nr, nc, dist+1});
                    }
                }
            }
        }
        
        // Convert reachable positions to picture numbers and filter by available
        set<int> safe_pictures;
        for(auto [r, c] : reachable){
            int pic = r * N + c + 1;
            if(available.count(pic)){
                safe_pictures.insert(pic);
            }
        }
        
        // Find pictures to remove (all available except safe ones)
        vector<int> to_remove;
        for(int pic : available){
            if(!safe_pictures.count(pic)){
                to_remove.push_back(pic);
            }
        }
        
        // Remove them
        for(int pic : to_remove){
            available.erase(pic);
        }
        
        if(to_remove.size() > 0){
            moves_and_removals.push_back({K, to_remove});
        }
        
        // Move finger to a safe position
        if(!safe_pictures.empty()){
            int safe_pic = *safe_pictures.begin();
            row = (safe_pic - 1) / N;
            col = (safe_pic - 1) % N;
        }
        
        K++;
    }
    
    // Output
    for(auto [k, removed] : moves_and_removals){
        cout << k;
        for(int pic : removed){
            cout << " " << pic;
        }
        cout << "\n";
    }
    
    return 0;
}
