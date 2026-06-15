#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<long long, long long>> progressions(N);
    vector<set<long long>> candidates(N);
    
    for(int i = 0; i < N; i++){
        cin >> progressions[i].first >> progressions[i].second;
        long long A = progressions[i].first;
        long long B = progressions[i].second;
        
        if(A == 0){
            // Only B is in the progression
            if(B >= 1 && B <= M){
                candidates[i].insert(B);
            }
        } else if(A > 0){
            // X = A*K + B, K >= 0
            // Find all X in [1, M]
            if(B >= 1 && B <= M){
                candidates[i].insert(B);
            }
            // For K >= 1: X = A*K + B
            for(long long X = B + A; X <= M; X += A){
                if(X >= 1){
                    candidates[i].insert(X);
                }
            }
        } else {
            // A < 0: X = A*K + B = -|A|*K + B
            // For K=0: X = B
            // For K=1: X = B - |A|
            // For K=2: X = B - 2|A|, etc.
            long long absA = -A;
            for(long long X = B; X >= 1; X -= absA){
                if(X >= 1 && X <= M){
                    candidates[i].insert(X);
                }
            }
        }
    }
    
    vector<long long> assignment(N, -1);
    set<long long> used;
    
    // Create priority by number of candidates
    vector<pair<int, int>> order;
    for(int i = 0; i < N; i++){
        order.push_back({candidates[i].size(), i});
    }
    sort(order.begin(), order.end());
    
    // Greedy assignment
    for(auto [cnt, i] : order){
        bool assigned = false;
        // Try to assign from own progression first
        for(long long x : candidates[i]){
            if(used.find(x) == used.end()){
                assignment[i] = x;
                used.insert(x);
                assigned = true;
                break;
            }
        }
        // If not possible, assign any unused number
        if(!assigned){
            for(long long x = 1; x <= M; x++){
                if(used.find(x) == used.end()){
                    assignment[i] = x;
                    used.insert(x);
                    break;
                }
            }
        }
    }
    
    for(int i = 0; i < N; i++){
        if(i > 0) cout << " ";
        cout << assignment[i];
    }
    cout << "\n";
    
    return 0;
}
