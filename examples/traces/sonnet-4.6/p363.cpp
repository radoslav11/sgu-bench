#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    // For each window, store sorted list of finish times
    vector<vector<int>> queues(M+1);
    
    for(int i = 0; i < N; i++){
        int k, t;
        cin >> k >> t;
        
        // Remove all people who finish <= k from all queues
        for(int j = 1; j <= M; j++){
            vector<int> remaining;
            for(int ft : queues[j]){
                if(ft > k) remaining.push_back(ft);
            }
            queues[j] = remaining;
        }
        
        // Find queue with minimum size, break ties by lowest number
        int best_q = 1;
        for(int j = 2; j <= M; j++){
            if(queues[j].size() < queues[best_q].size()){
                best_q = j;
            }
        }
        
        // Compute finish time
        int finish;
        if(queues[best_q].empty()){
            finish = k + t;
        } else {
            finish = queues[best_q].back() + t;
        }
        
        queues[best_q].push_back(finish);
        
        cout << best_q << " " << finish << "\n";
    }
    
    return 0;
}
