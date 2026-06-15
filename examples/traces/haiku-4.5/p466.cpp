#include<bits/stdc++.h>
using std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, Q;
    cin >> N >> Q;
    
    string slots;
    cin >> slots;
    
    map<int, vector<int>> allocations; // query_id -> list of allocated slot indices (0-indexed)
    
    for(int q = 1; q <= Q; q++){
        string cmd;
        cin >> cmd;
        
        if(cmd == "PARK"){
            int S;
            cin >> S;
            
            // Find all clusters
            int best_head = -1, best_size = INT_MAX;
            
            for(int start = 0; start < N; start++){
                if(slots[start] == '.'){
                    int len = 0;
                    for(int j = 0; j < N; j++){
                        if(slots[(start + j) % N] == '.') len++;
                        else break;
                    }
                    if(len >= S && (len < best_size || (len == best_size && start < best_head))){
                        best_size = len;
                        best_head = start;
                    }
                    start += len - 1;
                }
            }
            
            if(best_head == -1){
                cout << "NO ROOM\n";
            } else {
                // Allocate S slots from best_head
                vector<int> allocated_slots;
                for(int i = 0; i < S; i++){
                    int idx = (best_head + i) % N;
                    slots[idx] = 'X';
                    allocated_slots.push_back(idx);
                }
                allocations[q] = allocated_slots;
                
                // Output in range format
                vector<pair<int,int>> ranges;
                int i = 0;
                while(i < allocated_slots.size()){
                    int j = i;
                    while(j + 1 < allocated_slots.size() && allocated_slots[j+1] == allocated_slots[j] + 1){
                        j++;
                    }
                    ranges.push_back({allocated_slots[i] + 1, allocated_slots[j] + 1});
                    i = j + 1;
                }
                
                for(int i = 0; i < ranges.size(); i++){
                    if(i > 0) cout << ",";
                    if(ranges[i].first == ranges[i].second){
                        cout << ranges[i].first;
                    } else {
                        cout << ranges[i].first << "-" << ranges[i].second;
                    }
                }
                cout << "\n";
            }
        } else {
            int qi;
            cin >> qi;
            for(int idx : allocations[qi]){
                slots[idx] = '.';
            }
        }
    }
    
    return 0;
}
