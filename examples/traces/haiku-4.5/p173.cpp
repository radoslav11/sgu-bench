#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K, L;
    cin >> N >> M >> K >> L;
    
    vector<pair<int,int>> ops(M);
    for(int i = 0; i < M; i++){
        cin >> ops[i].first >> ops[i].second;
        ops[i].first--; // 0-indexed
    }
    
    // Reconstruct vector A
    vector<int> A(K-1);
    
    for(int test = 0; test < L; test++){
        string before, after;
        cin >> before >> after;
        
        string shifted = before.substr(1) + before[0];
        
        for(int i = 0; i < K-1; i++){
            if(shifted[i] == '1'){
                bool flipped = (shifted[K-1] != after[K-1]);
                A[i] = flipped ? 1 : 0;
                break;
            }
        }
    }
    
    string result;
    cin >> result;
    
    // Reverse operations
    for(int op = M-1; op >= 0; op--){
        int start = ops[op].first;
        int times = ops[op].second % 2; // Since X is involutive for period 2 in many cases
        
        times = ops[op].second;
        
        for(int t = 0; t < times; t++){
            string sub = result.substr(start, K);
            
            // Count flips needed
            int flips = 0;
            for(int i = 0; i < K-1; i++){
                if(sub[i] == '1' && A[i] == 1) flips++;
            }
            if(flips % 2 == 1) sub[K-1] = (sub[K-1] == '0') ? '1' : '0';
            
            sub = sub[K-1] + sub.substr(0, K-1);
            
            for(int i = 0; i < K; i++){
                result[start + i] = sub[i];
            }
        }
    }
    
    cout << result << endl;
    
    return 0;
}
