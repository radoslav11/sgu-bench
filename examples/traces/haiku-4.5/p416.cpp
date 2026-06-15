#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<int> result(n);
    
    int half = n / 2;
    
    // Put small numbers in odd positions (outer), large in even (inner)
    // Arranged to alternate: small, large, small, large, ...
    for(int i = 0; i < half; i++){
        result[2*i] = i + 1;           // odd positions: 1, 2, 3, ..., n/2
        result[2*i + 1] = n - i;       // even positions: n, n-1, n-2, ..., n/2+1
    }
    
    for(int i = 0; i < n; i++){
        cout << result[i];
        if(i < n-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
