#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> c(n);
    for(int i = 0; i < n; i++){
        cin >> c[i];
    }
    
    vector<int> dp(n, 1);
    vector<int> parent(n, -1);
    
    for(int i = 1; i < n; i++){
        for(int j = 0; j < i; j++){
            if(abs(c[i] - c[j]) != 1){
                if(dp[j] + 1 > dp[i]){
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
        }
    }
    
    int maxLen = 0;
    int endPos = 0;
    for(int i = 0; i < n; i++){
        if(dp[i] > maxLen){
            maxLen = dp[i];
            endPos = i;
        }
    }
    
    vector<int> result;
    int pos = endPos;
    while(pos != -1){
        result.push_back(c[pos]);
        pos = parent[pos];
    }
    
    reverse(result.begin(), result.end());
    
    cout << n - maxLen << "\n";
    for(int i = 0; i < result.size(); i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
