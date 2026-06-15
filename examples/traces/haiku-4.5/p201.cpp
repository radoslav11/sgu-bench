#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string sigma;
    cin >> sigma;
    int K;
    cin >> K;
    
    int S, L;
    cin >> S >> L;
    S--; // 0-indexed
    
    set<int> terminal;
    for(int i = 0; i < L; i++){
        int t;
        cin >> t;
        terminal.insert(t-1); // 0-indexed
    }
    
    vector<vector<int>> phi(K, vector<int>(sigma.size()));
    vector<vector<int>> chi(K, vector<int>(sigma.size()));
    
    for(int i = 0; i < K; i++){
        for(int j = 0; j < sigma.size(); j++){
            cin >> phi[i][j];
            phi[i][j]--; // 0-indexed
        }
    }
    
    for(int i = 0; i < K; i++){
        for(int j = 0; j < sigma.size(); j++){
            cin >> chi[i][j];
        }
    }
    
    int N;
    cin >> N;
    
    // dp[pos][state] = number of ways
    vector<vector<long long>> dp(N+1, vector<long long>(K, 0));
    dp[0][S] = 1;
    
    for(int pos = 0; pos < N; pos++){
        for(int state = 0; state < K; state++){
            if(dp[pos][state] == 0) continue;
            
            // Try each character
            for(int c = 0; c < sigma.size(); c++){
                // Follow transitions with this character
                set<pair<int,int>> visited; // (state, pos_delta) to avoid cycles
                queue<pair<int,int>> q; // (state, pos_delta)
                q.push({state, 0});
                
                while(!q.empty()){
                    auto [cur_state, delta] = q.front();
                    q.pop();
                    
                    if(visited.count({cur_state, delta})) continue;
                    visited.insert({cur_state, delta});
                    
                    int next_state = phi[cur_state][c];
                    int next_pos = pos + delta + (1 - chi[cur_state][c]);
                    
                    if(next_pos <= N){
                        dp[next_pos][next_state] += dp[pos][state];
                    }
                    
                    // If non-absorbing, can continue from next_state with same char
                    if(chi[cur_state][c] == 1){
                        if(!visited.count({next_state, delta + 1})){
                            q.push({next_state, delta + 1});
                        }
                    }
                }
            }
        }
    }
    
    long long ans = 0;
    for(int state : terminal){
        ans += dp[N][state];
    }
    
    cout << ans << endl;
    
    return 0;
}
