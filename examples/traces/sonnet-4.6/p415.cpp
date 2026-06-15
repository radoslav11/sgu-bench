#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, x;
    cin >> n >> x;
    vector<int> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    
    vector<int> necessary;
    
    for(int i = 0; i < n; i++){
        bitset<10001> dp;
        dp[0] = 1;
        for(int j = 0; j < n; j++){
            if(j == i) continue;
            dp |= (dp << a[j]);
        }
        if(!dp[x]){
            necessary.push_back(a[i]);
        }
    }
    
    cout << necessary.size() << "\n";
    for(int i = 0; i < (int)necessary.size(); i++){
        if(i) cout << " ";
        cout << necessary[i];
    }
    cout << "\n";
    
    return 0;
}
