#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::precision(15);
    
    int N, d;
    double c;
    cin >> N >> d >> c;
    
    vector<double> a(N+1), b(N+1), delta(N+1);
    for(int i = 1; i <= N; i++){
        cin >> a[i] >> b[i] >> delta[i];
    }
    
    int current_lane = 1;
    double current_time = 0;
    double current_dist = 0;
    vector<pair<int, double>> switches;
    
    double dt = 0.01;
    
    while(current_dist < d - 1e-9){
        double best_lane = current_lane;
        double best_score = -1e9;
        
        for(int lane = 1; lane <= N; lane++){
            double speed = b[lane] + a[lane] * sin(current_time + delta[lane]);
            double switch_cost = c * abs(lane - current_lane);
            double score = speed - switch_cost / dt;
            
            if(score > best_score){
                best_score = score;
                best_lane = lane;
            }
        }
        
        if(best_lane != current_lane){
            double switch_time = c * abs(best_lane - current_lane);
            switches.push_back({(int)best_lane, current_time});
            current_lane = best_lane;
            current_time += switch_time;
        } else {
            double speed = b[current_lane] + a[current_lane] * sin(current_time + delta[current_lane]);
            double dist_step = min(speed * dt, d - current_dist);
            current_dist += dist_step;
            current_time += dist_step / speed;
        }
    }
    
    cout << fixed << current_time << "\n";
    cout << switches.size() << "\n";
    for(auto& sw : switches){
        cout << sw.first << " " << sw.second << "\n";
    }
    
    return 0;
}
