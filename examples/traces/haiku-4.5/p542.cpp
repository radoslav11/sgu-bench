#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> a(n);
    long long minVal = LLONG_MAX;
    
    for(int i = 0; i < n; i++){
        cin >> a[i];
        minVal = min(minVal, a[i]);
    }
    
    int count = 0;
    
    // Try all possible values of x from 0 to minVal-1
    for(long long x = 0; x < minVal; x++){
        long long xorSum = 0;
        for(int i = 0; i < n; i++){
            xorSum ^= (a[i] - x);
        }
        if(xorSum == 0){
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
