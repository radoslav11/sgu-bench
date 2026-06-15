#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    string s;
    cin >> n >> s;
    
    map<char, int> charToBtn = {{'L', 0}, {'U', 1}, {'R', 2}, {'D', 3}};
    string btnToChar = "LURD";
    
    // State: (cost, beat, left, right, lastPressed)
    // lastPressed: 0=left, 1=right
    map<tuple<int,int,int,int>, int> dist;
    map<tuple<int,int,int,int>, tuple<int,int,int,int>> parent;
    
    priority_queue<tuple<int,int,int,int,int>, vector<tuple<int,int,int,int,int>>, greater<tuple<int,int,int,int,int>>> pq;
    
    // Start: left=L(0), right=R(2)
    auto start = make_tuple(0, 0, 2, -1); // -1 means no leg pressed yet
    dist[start] = 0;
    pq.push({0, 0, 0, 2, -1});
    
    while(!pq.empty()){
        auto [cost, beat, left, right, lastPressed] = pq.top();
        pq.pop();
        
        if(beat == n) continue;
        
        auto state = make_tuple(beat, left, right, lastPressed);
        if(dist.count(state) && dist[state] < cost) continue;
        
        char req = s[beat];
        int reqBtn = (req == 'N') ? -1 : charToBtn[req];
        
        // Try all possible next positions
        for(int nl = 0; nl < 4; nl++){
            for(int nr = 0; nr < 4; nr++){
                if(nl == nr) continue; // Can't be on same button
                if(nl == 2 && nr == 0) continue; // Illegal position RL
                
                // Check if this position satisfies the requirement
                if(reqBtn != -1 && nl != reqBtn && nr != reqBtn) continue;
                
                // Calculate cost
                int moveCost = 0;
                int newLastPressed = -1;
                
                if(nl == left && nr == right){
                    // No movement
                    if(reqBtn != -1){
                        moveCost = 1;
                        newLastPressed = (nl == reqBtn) ? 0 : 1;
                    }
                } else if(nl == left){
                    // Right leg moved
                    moveCost = (lastPressed == 1) ? 9 : 3;
                    newLastPressed = 1;
                } else if(nr == right){
                    // Left leg moved
                    moveCost = (lastPressed == 0) ? 9 : 3;
                    newLastPressed = 0;
                } else {
                    // Both legs moved (jump)
                    moveCost = 10;
                    newLastPressed = (nl == reqBtn) ? 0 : 1;
                }
                
                int newCost = cost + moveCost;
                auto newState = make_tuple(beat + 1, nl, nr, newLastPressed);
                
                if(!dist.count(newState) || dist[newState] > newCost){
                    dist[newState] = newCost;
                    parent[newState] = state;
                    pq.push({newCost, beat + 1, nl, nr, newLastPressed});
                }
            }
        }
    }
    
    // Find best final state
    int minCost = INT_MAX;
    auto bestState = make_tuple(n, 0, 2, -1);
    for(auto& [state, cost] : dist){
        if(get<1>(state) == n && cost < minCost){
            minCost = cost;
            bestState = state;
        }
    }
    
    cout << minCost << "\n";
    
    vector<pair<int,int>> path;
    auto cur = bestState;
    while(get<1>(cur) > 0){
        path.push_back({get<2>(cur), get<3>(cur)});
        cur = parent[cur];
    }
    
    reverse(path.begin(), path.end());
    for(auto [l, r] : path){
        cout << btnToChar[l] << btnToChar[r] << "\n";
    }
    
    return 0;
}
