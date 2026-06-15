#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    long long T;
    cin >> N >> M >> T;
    
    vector<int> C(N);
    for(int i = 0; i < N; i++) cin >> C[i];
    
    vector<array<long long, 4>> groups(M);
    for(int i = 0; i < M; i++){
        cin >> groups[i][0] >> groups[i][1] >> groups[i][2] >> groups[i][3];
    }
    
    vector<vector<long long>> seats(N);
    for(int i = 0; i < N; i++) seats[i].assign(C[i], 0);
    
    queue<int> waiting;
    double total_satisfaction = 0;
    int next_group = 0;
    map<int, long long> finish_times;
    
    auto try_seat = [&](int g_idx, long long current_time) -> bool {
        long long arr_time = groups[g_idx][0];
        int p_count = groups[g_idx][1];
        long long w_time = groups[g_idx][2];
        
        if(current_time > arr_time + w_time || current_time >= T) return false;
        
        int best_counter = -1, best_pos = -1;
        int best_min = -1, best_max = -1;
        
        for(int counter = 0; counter < N; counter++){
            for(int pos = 0; pos <= C[counter] - p_count; pos++){
                bool can_sit = true;
                for(int j = pos; j < pos + p_count; j++){
                    if(seats[counter][j] > current_time){
                        can_sit = false;
                        break;
                    }
                }
                
                if(!can_sit) continue;
                
                int SL = INT_MAX, SR = INT_MAX;
                for(int j = pos - 1; j >= 0; j--){
                    if(seats[counter][j] > current_time){
                        SL = pos - 1 - j;
                        break;
                    }
                }
                if(SL == INT_MAX && pos > 0) SL = pos;
                
                for(int j = pos + p_count; j < C[counter]; j++){
                    if(seats[counter][j] > current_time){
                        SR = j - pos - p_count;
                        break;
                    }
                }
                if(SR == INT_MAX && pos + p_count < C[counter]) SR = C[counter] - pos - p_count;
                
                int curr_min = min(SL, SR);
                int curr_max = max(SL, SR);
                
                if(best_counter == -1 || curr_min > best_min || 
                   (curr_min == best_min && curr_max > best_max) ||
                   (curr_min == best_min && curr_max == best_max && counter < best_counter) ||
                   (curr_min == best_min && curr_max == best_max && counter == best_counter && pos < best_pos)){
                    best_counter = counter;
                    best_pos = pos;
                    best_min = curr_min;
                    best_max = curr_max;
                }
            }
        }
        
        if(best_counter == -1) return false;
        
        long long eat_start = current_time;
        for(int j = best_pos; j < best_pos + p_count; j++){
            eat_start = max(eat_start, seats[best_counter][j]);
        }
        
        long long actual_wait = eat_start - arr_time;
        total_satisfaction += (double)(w_time - actual_wait) / w_time;
        
        long long eat_end = eat_start + groups[g_idx][3];
        for(int j = best_pos; j < best_pos + p_count; j++){
            seats[best_counter][j] = eat_end;
        }
        finish_times[g_idx] = eat_end;
        return true;
    };
    
    while(next_group < M || !waiting.empty()){
        long long current_time = LLONG_MAX;
        
        if(next_group < M) current_time = min(current_time, groups[next_group][0]);
        if(!finish_times.empty()){
            for(auto& p : finish_times) current_time = min(current_time, p.second);
        }
        
        if(current_time == LLONG_MAX) break;
        
        // Remove finished groups
        vector<int> to_remove;
        for(auto& p : finish_times){
            if(p.second == current_time) to_remove.push_back(p.first);
        }
        for(int idx : to_remove) finish_times.erase(idx);
        
        // Add arriving group
        if(next_group < M && groups[next_group][0] == current_time){
            waiting.push(next_group);
            next_group++;
        }
        
        // Try to seat waiting groups
        while(!waiting.empty()){
            int g_idx = waiting.front();
            if(try_seat(g_idx, current_time)){
                waiting.pop();
            } else {
                long long arr_time = groups[g_idx][0];
                long long w_time = groups[g_idx][2];
                if(current_time > arr_time + w_time || current_time >= T){
                    total_satisfaction += -1;
                    waiting.pop();
                } else {
                    break;
                }
            }
        }
    }
    
    // Remaining waiting groups leave
    while(!waiting.empty()){
        total_satisfaction += -1;
        waiting.pop();
    }
    
    cout << fixed << setprecision(15) << total_satisfaction / M << endl;
    
    return 0;
}
