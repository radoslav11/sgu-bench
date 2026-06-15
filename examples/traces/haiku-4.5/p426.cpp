#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<int> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    
    for(int j = 0; j < m; j++){
        // Create a^(j)
        vector<int> seq(n);
        for(int i = 0; i < n; i++){
            seq[i] = (a[i] + j) % m;
        }
        
        // Find smallest cyclic shift
        int best_start = 0;
        for(int start = 1; start < n; start++){
            bool is_better = false;
            for(int pos = 0; pos < n; pos++){
                int seq_best = seq[(best_start + pos) % n];
                int seq_curr = seq[(start + pos) % n];
                if(seq_curr < seq_best){
                    is_better = true;
                    break;
                }
                if(seq_curr > seq_best){
                    break;
                }
            }
            if(is_better){
                best_start = start;
            }
        }
        
        // Output k-th element (1-indexed) of smallest cyclic shift
        int result = seq[(best_start + k - 1) % n];
        cout << result << "\n";
    }
    
    return 0;
}
