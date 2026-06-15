#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<vector<string>> sym(K, vector<string>(N));
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) cin >> sym[k][i];
    }
    
    int numPairs = K*(K-1)/2;
    // pair index
    vector<pair<int,int>> pairs;
    for(int i = 0; i < K; i++)
        for(int j = i+1; j < K; j++)
            pairs.push_back({i,j});
    
    int P = N*M;
    // For each pixel, compute coverage bitmask
    vector<int> cov(P);
    for(int p = 0; p < P; p++){
        int r = p/M, c = p%M;
        int mask = 0;
        for(int q = 0; q < (int)pairs.size(); q++){
            int a = pairs[q].first, b = pairs[q].second;
            if(sym[a][r][c] != sym[b][r][c]) mask |= (1<<q);
        }
        cov[p] = mask;
    }
    
    int full = (1<<numPairs)-1;
    vector<int> dp(1<<numPairs, INT_MAX);
    vector<int> from_mask(1<<numPairs, -1);
    vector<int> from_pixel(1<<numPairs, -1);
    dp[0] = 0;
    
    for(int mask = 0; mask < (1<<numPairs); mask++){
        if(dp[mask] == INT_MAX) continue;
        for(int p = 0; p < P; p++){
            int nmask = mask | cov[p];
            if(dp[nmask] > dp[mask]+1){
                dp[nmask] = dp[mask]+1;
                from_mask[nmask] = mask;
                from_pixel[nmask] = p;
            }
        }
    }
    
    // Reconstruct
    vector<bool> chosen(P, false);
    int cur = full;
    while(cur != 0){
        int p = from_pixel[cur];
        chosen[p] = true;
        cur = from_mask[cur];
    }
    
    cout << dp[full] << "\n";
    for(int r = 0; r < N; r++){
        for(int c = 0; c < M; c++){
            cout << (chosen[r*M+c] ? '1' : '0');
        }
        cout << "\n";
    }
}
