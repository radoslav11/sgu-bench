#include<bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string c1, c2, c3;
    cin >> c1 >> c2 >> c3;
    
    int n = c1.length();
    long long count = 0;
    
    for(long long town = 0; town < (1LL << n); town++){
        long long d1 = 0, d2 = 0, d3 = 0;
        
        for(int i = 0; i < n; i++){
            int bit = (town >> i) & 1;
            int a = c1[n-1-i] - '0';
            int b = c2[n-1-i] - '0';
            int c = c3[n-1-i] - '0';
            
            d1 += abs(bit - a);
            d2 += abs(bit - b);
            d3 += abs(bit - c);
        }
        
        if(d1 == d2 && d2 == d3){
            count++;
        }
    }
    
    cout << count % MOD << endl;
    
    return 0;
}
