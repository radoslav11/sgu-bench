#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, k;
    cin >> n >> k;
    
    // For each color, collect diagonal lengths
    vector<int> white_diags, black_diags;
    
    // Identify diagonals for each color
    // A square (i,j) is white if (i+j) is even
    map<int, int> white_map, black_map;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if((i + j) % 2 == 0){
                // white square - use diagonal i-j
                white_map[i - j]++;
            } else {
                // black square - use diagonal i-j
                black_map[i - j]++;
            }
        }
    }
    
    for(auto& p : white_map) white_diags.push_back(p.second);
    for(auto& p : black_map) black_diags.push_back(p.second);
    
    // DP for each color
    auto compute = [](vector<int>& diags, int max_k) -> vector<long long> {
        int m = diags.size();
        vector<vector<long long>> dp(m + 1, vector<long long>(max_k + 1, 0));
        dp[0][0] = 1;
        
        for(int i = 0; i < m; i++){
            for(int j = 0; j <= max_k; j++){
                // Don't place bishop on diagonal i
                dp[i+1][j] += dp[i][j];
                // Place bishop on diagonal i (one of diags[i] positions)
                if(j + 1 <= max_k){
                    dp[i+1][j+1] += dp[i][j] * diags[i];
                }
            }
        }
        
        vector<long long> result(max_k + 1);
        for(int j = 0; j <= max_k; j++){
            result[j] = dp[m][j];
        }
        return result;
    };
    
    auto white_ways = compute(white_diags, k);
    auto black_ways = compute(black_diags, k);
    
    long long answer = 0;
    for(int i = 0; i <= k; i++){
        answer += white_ways[i] * black_ways[k - i];
    }
    
    cout << answer << endl;
    
    return 0;
}
