#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    // Enumerate all partitions of n into parts >= 2
    // parts[i] = count of cycles of length i+2 (i from 0 to n-2)
    
    double total_derangements = 0, total_winners = 0;
    
    // Use recursive enumeration
    // fact[i]
    vector<double> fact(n+1);
    fact[0]=1; for(int i=1;i<=n;i++) fact[i]=fact[i-1]*i;
    
    // Enumerate partitions of n into parts >= 2
    vector<int> parts(n+1,0);
    function<void(int,int)> solve = [&](int rem, int min_part){
        if(rem == 0){
            double cnt = fact[n];
            int maxp = 0; int maxcnt = 0;
            for(int k=2;k<=n;k++) if(parts[k]>0){
                cnt /= (pow(k, parts[k]) * fact[parts[k]]);
                if(k > maxp){ maxp = k; maxcnt = parts[k]; }
            }
            total_derangements += cnt;
            total_winners += cnt * maxp * maxcnt;
            return;
        }
        for(int p = min_part; p <= rem; p++){
            parts[p]++;
            solve(rem - p, p);
            parts[p]--;
        }
    };
    solve(n, 2);
    
    printf("%.9f\n", total_winners / total_derangements);
}
