#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K, P;
    cin >> N >> K >> P;
    
    vector<int> B(K);
    for(int i = 0; i < K; i++){
        cin >> B[i];
    }
    
    vector<pair<int,int>> teeth(N); // {cost, gum_id}
    vector<vector<int>> gum_teeth(K); // teeth on each gum
    
    for(int i = 0; i < N; i++){
        int a, c;
        cin >> a >> c;
        c--;
        teeth[i] = {a, c};
        gum_teeth[c].push_back(i);
    }
    
    // Sort teeth by cost
    vector<int> sorted_teeth(N);
    iota(sorted_teeth.begin(), sorted_teeth.end(), 0);
    sort(sorted_teeth.begin(), sorted_teeth.end(), [&](int i, int j){
        return teeth[i].first < teeth[j].first;
    });
    
    int best_count = 0;
    vector<int> best_solution;
    
    // Try all subsets of gums (using bitmask if K is small, otherwise greedy)
    if(K <= 20){
        for(int mask = 0; mask < (1 << K); mask++){
            int anesthesia_cost = 0;
            for(int g = 0; g < K; g++){
                if(mask & (1 << g)){
                    anesthesia_cost += B[g];
                }
            }
            if(anesthesia_cost > P) continue;
            
            int remaining = P - anesthesia_cost;
            vector<int> selected;
            
            for(int tooth_idx : sorted_teeth){
                int gum = teeth[tooth_idx].second;
                if(mask & (1 << gum)){
                    if(teeth[tooth_idx].first <= remaining){
                        selected.push_back(tooth_idx);
                        remaining -= teeth[tooth_idx].first;
                    }
                }
            }
            
            if(selected.size() > best_count){
                best_count = selected.size();
                best_solution = selected;
            }
        }
    } else {
        // Greedy approach for larger K
        set<int> used_gums;
        vector<int> selected;
        int spent = 0;
        
        for(int tooth_idx : sorted_teeth){
            int gum = teeth[tooth_idx].second;
            int cost = teeth[tooth_idx].first;
            int additional = 0;
            if(used_gums.find(gum) == used_gums.end()){
                additional = B[gum];
            }
            if(spent + cost + additional <= P){
                selected.push_back(tooth_idx);
                spent += cost;
                if(used_gums.find(gum) == used_gums.end()){
                    spent += additional;
                    used_gums.insert(gum);
                }
            }
        }
        best_count = selected.size();
        best_solution = selected;
    }
    
    cout << best_count << "\n";
    for(int idx : best_solution){
        cout << idx + 1 << " ";
    }
    if(best_count > 0) cout << "\n";
    
    return 0;
}
