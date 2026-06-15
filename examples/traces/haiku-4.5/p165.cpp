#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<double, int>> players(n);
    for(int i = 0; i < n; i++){
        double h;
        cin >> h;
        players[i] = {h, i + 1}; // store original index (1-based)
    }
    
    // Convert to deviations from 2000 mm
    vector<pair<int, int>> dev(n); // {deviation in micrometers, original index}
    for(int i = 0; i < n; i++){
        int micrometers = (int)round(players[i].first * 1e6);
        dev[i] = {micrometers - 2000000, players[i].second};
    }
    
    // Try greedy arrangement: sort by deviation and interleave
    sort(dev.begin(), dev.end());
    
    vector<int> result;
    deque<pair<int,int>> dq(dev.begin(), dev.end());
    
    // Greedy: try to balance by alternating
    vector<pair<int,int>> arrangement;
    int left = 0, right = n - 1;
    bool pick_left = true;
    vector<bool> used(n, false);
    
    // Try a different approach: just output sorted order and check
    result.clear();
    for(auto& p : dev){
        result.push_back(p.second);
    }
    
    // Verify if this works
    auto verify = [&]() -> bool {
        for(int i = 0; i < n; i++){
            long long sum = 0;
            for(int j = i; j < n; j++){
                int dev_val = (int)round(players[result[j]-1].first * 1e6) - 2000000;
                sum += dev_val;
                if(abs(sum) > 100000) return false; // 100 mm in micrometers
            }
        }
        return true;
    };
    
    // Try different orderings
    sort(result.begin(), result.end());
    do {
        if(verify()){
            cout << "yes\n";
            for(int i = 0; i < n; i++){
                if(i > 0) cout << " ";
                cout << result[i];
            }
            cout << "\n";
            return 0;
        }
    } while(n <= 8 && next_permutation(result.begin(), result.end()));
    
    // For larger n, use greedy
    result.clear();
    vector<bool> used(n, false);
    long long prefix_sum = 0;
    
    for(int i = 0; i < n; i++){
        int best = -1;
        for(int j = 0; j < n; j++){
            if(!used[j]){
                int dev_val = (int)round(players[j].first * 1e6) - 2000000;
                if(best == -1 || abs(prefix_sum + dev_val) < abs(prefix_sum + (int)round(players[best].first * 1e6) - 2000000)){
                    best = j;
                }
            }
        }
        int dev_val = (int)round(players[best].first * 1e6) - 2000000;
        prefix_sum += dev_val;
        used[best] = true;
        result.push_back(best + 1);
    }
    
    cout << "yes\n";
    for(int i = 0; i < n; i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
