#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<int> reg(N, 0);
    
    for(int op = 0; op < M; op++){
        int pos;
        cin >> pos;
        
        vector<pair<int,int>> changes;
        reg[pos]++;
        changes.push_back({pos, reg[pos]});
        
        // Handle carries
        for(int i = pos; i < N-1; i++){
            if(reg[i] >= 3){
                changes.push_back({i, reg[i] % 3});
                reg[i] %= 3;
                reg[i+1]++;
                if(i+1 < N) changes.push_back({i+1, reg[i+1]});
            }
        }
        
        // Remove duplicates and keep only last value for each register
        map<int, int> final_changes;
        for(auto [idx, val] : changes){
            final_changes[idx] = val;
        }
        
        cout << final_changes.size();
        for(auto [idx, val] : final_changes){
            cout << " " << idx << " " << val;
        }
        cout << "\n";
    }
    
    return 0;
}
