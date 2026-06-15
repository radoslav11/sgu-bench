#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<long long> w(n), c(n);
    for(int i = 0; i < n; i++){
        cin >> w[i] >> c[i];
    }
    
    // Precompute prefix sums
    vector<long long> prefixW(n+1, 0), prefixC(n+1, 0);
    for(int i = 0; i < n; i++){
        prefixW[i+1] = prefixW[i] + w[i];
        prefixC[i+1] = prefixC[i] + c[i];
    }
    
    // BFS to find maximum value
    map<pair<int,int>, pair<long long, string>> best;
    queue<pair<int,int>> q;
    
    q.push({0, 0}); // taken_from_left, taken_from_right
    best[{0, 0}] = {0, ""};
    
    long long maxValue = 0;
    pair<int,int> maxState = {0, 0};
    
    while(!q.empty()){
        auto [tl, tr] = q.front();
        q.pop();
        
        if(tl + tr >= n) continue;
        
        int left = tl;  // number of pearls taken from left
        int right = tr; // number of pearls taken from right
        
        // Current hanging part is from index (n-m-right) to (n-1-right)
        // Current table part is from index left to (n-m-1-right)
        
        int hanging_start = n - m - right;
        int hanging_end = n - right;
        int table_end = n - m - right;
        
        if(hanging_start < 0 || hanging_start >= hanging_end) continue;
        if(table_end < left || table_end <= left) continue;
        
        long long wh = prefixW[hanging_end] - prefixW[hanging_start];
        long long wt = prefixW[table_end] - prefixW[left];
        
        // Try taking from table (left)
        if(left < table_end){
            long long new_wt = wt - w[left];
            if(wh <= k * new_wt){
                long long new_val = best[{tl, tr}].first + c[left];
                if(best.find({tl+1, tr}) == best.end() || best[{tl+1, tr}].first < new_val){
                    best[{tl+1, tr}] = {new_val, best[{tl, tr}].second + "T"};
                    q.push({tl+1, tr});
                    if(new_val > maxValue){
                        maxValue = new_val;
                        maxState = {tl+1, tr};
                    }
                }
            }
        }
        
        // Try taking from hanging (right)
        if(right < m && hanging_start > 0){
            long long new_wh = wh - w[hanging_start];
            if(new_wh <= k * wt){
                long long new_val = best[{tl, tr}].first + c[hanging_start];
                if(best.find({tl, tr+1}) == best.end() || best[{tl, tr+1}].first < new_val){
                    best[{tl, tr+1}] = {new_val, best[{tl, tr}].second + "H"};
                    q.push({tl, tr+1});
                    if(new_val > maxValue){
                        maxValue = new_val;
                        maxState = {tl, tr+1};
                    }
                }
            }
        }
    }
    
    cout << maxState.first + maxState.second << " " << maxValue << "\n";
    if(maxState.first + maxState.second > 0){
        cout << best[maxState].second << "\n";
    }
    
    return 0;
}
