#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int c, k;
    cin >> c >> k;
    
    vector<vector<long long>> available(c, vector<long long>(k));
    for(int i = 0; i < c; i++){
        for(int j = 0; j < k; j++){
            cin >> available[i][j];
        }
    }
    
    vector<tuple<int, int, long long>> result; // color, size_index, count
    
    // We need to fill requirements level by level
    vector<long long> needed(k, 0);
    needed[k-1] = 1; // We need 1 corner of size 2^(k-1)
    
    // Process from size k-1 down to 0
    for(int size_idx = k-1; size_idx >= 0; size_idx--){
        // Try to fulfill the need for this size
        long long need = needed[size_idx];
        
        // Greedily use colors that have available corners of this size
        for(int color = 0; color < c && need > 0; color++){
            long long use = min(need, available[color][size_idx]);
            if(use > 0){
                result.push_back({color + 1, size_idx, use});
                available[color][size_idx] -= use;
                need -= use;
            }
        }
        
        // If we still need more, try to get from larger sizes
        for(int larger_size = size_idx + 1; larger_size < k && need > 0; larger_size++){
            for(int color = 0; color < c && need > 0; color++){
                long long use = min((need + 3) / 4, available[color][larger_size]);
                if(use > 0){
                    // Break down larger corners into 4 smaller ones
                    long long new_corners = use * 4;
                    available[color][size_idx] += new_corners;
                    available[color][larger_size] -= use;
                    
                    long long use_small = min(need, new_corners);
                    result.push_back({color + 1, size_idx, use_small});
                    available[color][size_idx] -= use_small;
                    need -= use_small;
                }
            }
        }
        
        if(need > 0){
            cout << "NO SOLUTION\n";
            return 0;
        }
        
        // Each corner of size 2^size_idx requires 4 corners of size 2^(size_idx-1)
        if(size_idx > 0){
            needed[size_idx - 1] = needed[size_idx] * 4;
        }
    }
    
    cout << result.size() << "\n";
    for(auto [color, size_idx, count] : result){
        cout << color << " " << size_idx << " " << count << "\n";
    }
    
    return 0;
}
