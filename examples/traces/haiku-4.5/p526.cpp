#include<bits/stdc++.h>
using namespace std;

struct Stone {
    double x1, x2, t;
};

struct State {
    double x, time;
    bool operator<(const State& other) const {
        if (abs(time - other.time) > 1e-9) return time > other.time;
        return x < other.x;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double v, G;
    int n;
    cin >> v >> G >> n;
    
    vector<Stone> stones(n);
    set<double> times;
    times.insert(0);
    
    for(int i = 0; i < n; i++){
        cin >> stones[i].x1 >> stones[i].x2 >> stones[i].t;
        times.insert(stones[i].t);
    }
    
    vector<double> timeVec(times.begin(), times.end());
    
    map<pair<double,double>, pair<double,double>> parent;
    priority_queue<State> pq;
    map<pair<double,double>, double> visited;
    
    pq.push({0, 0});
    
    double EPS = 1e-9;
    
    while(!pq.empty()){
        State cur = pq.top();
        pq.pop();
        
        if(abs(cur.x - G) < EPS){
            // Found path - reconstruct
            vector<pair<double,double>> path;
            auto p = make_pair(cur.x, cur.time);
            
            while(parent.count(p)){
                auto prev = parent[p];
                double w = (p.first - prev.first) / (p.second - prev.second + EPS);
                double t = p.second - prev.second;
                path.push_back({w, t});
                p = prev;
            }
            
            reverse(path.begin(), path.end());
            cout << path.size() << "\n";
            cout << fixed << setprecision(18);
            for(auto [w, t] : path){
                cout << w << " " << t << "\n";
            }
            return 0;
        }
        
        auto key = make_pair(cur.x, cur.time);
        if(visited.count(key) && visited[key] < cur.time + EPS) continue;
        visited[key] = cur.time;
        
        // Try moving at different speeds to next time points
        for(double nextTime : timeVec){
            if(nextTime <= cur.time + EPS) continue;
            
            double dt = nextTime - cur.time;
            double maxDist = v * dt;
            
            // Try moving right
            for(double targetX : {cur.x + maxDist, G}){
                double dist = abs(targetX - cur.x);
                if(dist > maxDist + EPS) continue;
                
                double w = (targetX - cur.x) / dt;
                bool valid = true;
                
                // Check all stones at time nextTime
                for(auto& stone : stones){
                    if(abs(stone.t - nextTime) < EPS){
                        double pos = cur.x + w * (stone.t - cur.time);
                        if(pos > stone.x1 + EPS && pos < stone.x2 - EPS){
                            valid = false;
                            break;
                        }
                    }
                }
                
                if(valid){
                    State next = {targetX, nextTime};
                    pq.push(next);
                    if(!parent.count(make_pair(next.x, next.time))){
                        parent[make_pair(next.x, next.time)] = key;
                    }
                }
            }
            
            // Try moving left
            for(double targetX : {cur.x - maxDist, G}){
                double dist = abs(targetX - cur.x);
                if(dist > maxDist + EPS) continue;
                
                double w = (targetX - cur.x) / dt;
                bool valid = true;
                
                for(auto& stone : stones){
                    if(abs(stone.t - nextTime) < EPS){
                        double pos = cur.x + w * (stone.t - cur.time);
                        if(pos > stone.x1 + EPS && pos < stone.x2 - EPS){
                            valid = false;
                            break;
                        }
                    }
                }
                
                if(valid){
                    State next = {targetX, nextTime};
                    pq.push(next);
                    if(!parent.count(make_pair(next.x, next.time))){
                        parent[make_pair(next.x, next.time)] = key;
                    }
                }
            }
        }
    }
    
    cout << -1 << "\n";
    return 0;
}
