#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long L;
    cin >> n >> L;
    
    vector<long double> pos(n);
    vector<long long> color(n);
    vector<int> dir(n);
    vector<int> orig_idx(n);
    
    for(int i = 0; i < n; i++){
        cin >> pos[i] >> color[i] >> dir[i];
        orig_idx[i] = i;
    }
    
    long long T;
    cin >> T;
    
    // Simulate collisions
    for(int iter = 0; iter < n * n; iter++){
        long double min_collision_time = LLONG_MAX;
        int col_i = -1, col_j = -1;
        
        // Find next collision
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                // Check if i and j will collide
                // i is at pos[i] moving with dir[i]
                // j is at pos[j] moving with dir[j]
                
                if(dir[i] == dir[j]) continue; // Same direction, no collision
                
                // Relative position and velocity
                long double rel_pos = pos[j] - pos[i];
                if(rel_pos < 0) rel_pos += L;
                
                int rel_vel = dir[j] - dir[i]; // Should be -2 or 2
                
                // Time to collision: rel_pos / rel_vel (but need positive time in [0, T])
                long double collision_time = rel_pos / (2.0); // They approach at speed 2
                
                if(dir[i] == 1 && dir[j] == -1){ // i clockwise, j counter-clockwise
                    if(collision_time < min_collision_time && collision_time > 1e-9){
                        min_collision_time = collision_time;
                        col_i = i;
                        col_j = j;
                    }
                }
            }
        }
        
        if(col_i == -1 || min_collision_time > T) break;
        
        // Move to collision time
        for(int i = 0; i < n; i++){
            pos[i] += dir[i] * min_collision_time;
            pos[i] = fmod(pos[i], (long double)L);
            if(pos[i] < 0) pos[i] += L;
        }
        
        T -= min_collision_time;
        
        // Perform collision
        long long temp_color = color[col_i];
        color[col_i] = color[col_j];
        color[col_j] = temp_color;
        dir[col_i] *= -1;
        dir[col_j] *= -1;
    }
    
    // Move remaining time T
    for(int i = 0; i < n; i++){
        pos[i] += dir[i] * T;
        pos[i] = fmod(pos[i], (long double)L);
        if(pos[i] < 0) pos[i] += L;
    }
    
    cout << fixed << setprecision(10);
    for(int i = 0; i < n; i++){
        cout << pos[i] << " " << color[i] << " " << dir[i] << "\n";
    }
    
    return 0;
}
