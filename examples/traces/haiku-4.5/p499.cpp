#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    
    long long maxGcd = 0;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            long long g = __gcd(a[i], a[j]);
            maxGcd = max(maxGcd, g);
        }
    }
    
    cout << maxGcd << endl;
    
    return 0;
}
