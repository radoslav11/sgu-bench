#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<long long> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    
    // Build linear basis
    vector<long long> basis;
    for(int i = 0; i < n; i++){
        long long cur = a[i];
        for(long long b : basis){
            cur = min(cur, cur ^ b);
        }
        if(cur > 0){
            basis.push_back(cur);
            // Keep basis in reduced form (optional but helps)
            // Sort descending
            for(int j = basis.size()-1; j > 0 && basis[j] > basis[j-1]; j--)
                swap(basis[j], basis[j-1]);
        }
    }
    
    // Find maximum XOR
    long long ans = 0;
    for(long long b : basis){
        ans = max(ans, ans ^ b);
    }
    
    cout << ans << endl;
    return 0;
}
