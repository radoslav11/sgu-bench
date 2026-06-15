#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int F, V;
    cin >> F >> V;
    
    vector<vector<int>> A(F+1, vector<int>(V+1));
    for(int i = 1; i <= F; i++){
        for(int j = 1; j <= V; j++){
            cin >> A[i][j];
        }
    }
    
    // dp[i][j] = max aesthetic value when placing bunches 1..i with bunch i in vase j
    vector<vector<int>> dp(F+1, vector<int>(V+1, INT_MIN));
    vector<vector<int>> parent(F+1, vector<int>(V+1, -1));
    
    // Base case: place bunch 1 in any vase
    for(int j = 1; j <= V; j++){
        dp[1][j] = A[1][j];
    }
    
    // Fill DP table
    for(int i = 2; i <= F; i++){
        for(int j = i; j <= V; j++){  // bunch i must be in vase at least i (after previous bunches)
            // Try all positions for bunch i-1
            for(int k = i-1; k < j; k++){  // k is vase of bunch i-1, must be < j
                if(dp[i-1][k] != INT_MIN){
                    int val = dp[i-1][k] + A[i][j];
                    if(val > dp[i][j]){
                        dp[i][j] = val;
                        parent[i][j] = k;
                    }
                }
            }
        }
    }
    
    // Find maximum value and its vase position
    int maxVal = INT_MIN;
    int maxVase = -1;
    for(int j = F; j <= V; j++){
        if(dp[F][j] > maxVal){
            maxVal = dp[F][j];
            maxVase = j;
        }
    }
    
    // Reconstruct solution
    vector<int> result(F+1);
    int curVase = maxVase;
    for(int i = F; i >= 1; i--){
        result[i] = curVase;
        curVase = parent[i][curVase];
    }
    
    cout << maxVal << "\n";
    for(int i = 1; i <= F; i++){
        if(i > 1) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
