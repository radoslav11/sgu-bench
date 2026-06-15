#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long B;
    int N;
    cin >> B >> N;
    
    vector<pair<long long, vector<pair<long long, long long>>>> stables; // position, horses (speed, distance)
    
    for(int i = 0; i < N; i++){
        long long X;
        int M;
        cin >> X >> M;
        vector<pair<long long, long long>> horses;
        for(int j = 0; j < M; j++){
            long long v, d;
            cin >> v >> d;
            horses.push_back({v, d});
        }
        stables.push_back({X, horses});
    }
    
    // Dijkstra: state is position, value is minimum time
    map<long long, double> dist;
    priority_queue<pair<double, long long>, vector<pair<double, long long>>, greater<pair<double, long long>>> pq;
    
    dist[0] = 0;
    pq.push({0, 0});
    
    while(!pq.empty()){
        auto [time, pos] = pq.top();
        pq.pop();
        
        if(pos == B){
            cout << fixed << setprecision(8) << time << endl;
            return 0;
        }
        
        if(dist.count(pos) && dist[pos] < time) continue;
        
        // Find stable at current position
        vector<pair<long long, long long>> available_horses;
        for(auto& stable : stables){
            if(stable.first == pos){
                for(auto& horse : stable.second){
                    available_horses.push_back(horse);
                }
            }
        }
        
        // If no horses at current position and pos != 0, skip
        if(available_horses.empty() && pos != 0) continue;
        
        // If at position 0, must find horses from first stable
        if(pos == 0){
            for(auto& stable : stables){
                for(auto& horse : stable.second){
                    available_horses.push_back(horse);
                }
            }
        }
        
        // Try each horse
        for(auto [speed, max_dist] : available_horses){
            long long reach = min(pos + max_dist, B);
            double new_time = time + (double)(reach - pos) / speed;
            
            if(reach == B){
                cout << fixed << setprecision(8) << new_time << endl;
                return 0;
            }
            
            // Check if we can reach any stable from here
            for(auto& stable : stables){
                long long stable_pos = stable.first;
                if(stable_pos > pos && stable_pos <= reach){
                    double time_to_stable = time + (double)(stable_pos - pos) / speed;
                    if(!dist.count(stable_pos) || dist[stable_pos] > time_to_stable){
                        dist[stable_pos] = time_to_stable;
                        pq.push({time_to_stable, stable_pos});
                    }
                }
            }
        }
    }
    
    cout << "-1" << endl;
    return 0;
}
