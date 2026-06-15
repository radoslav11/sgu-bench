#include <bits/stdc++.h>
using namespace std;

// count numbers in [1, X] that do NOT contain "13" as substring
long long countValid(long long X){
    if(X <= 0) return 0;
    string s = to_string(X);
    int n = s.size();
    // dp over positions, state: tight, prevWas1, started
    // count numbers from 0 to X (including 0) that are valid, then subtract 1 for 0
    // digit dp
    long long total = 0;
    // We'll count all numbers from 0..X (representing with up to n digits, leading allowed)
    // Use iterative: enumerate prefix
    // dp[prevIs1] = count of valid completions for free positions
    // Easier: standard digit dp counting numbers <= X valid (including 0)
    // count numbers with value in [0, X], no "13"
    // process digit by digit
    // function pow with free positions: number of valid strings of length L where
    // we track whether previous digit was 1
    // Let f(len, prev1) = number of ways to fill len digits without forming "13", given prev digit was 1 or not
    // f(0, *) = 1
    // f(len, prev1) = sum over d 0..9, skip if prev1 && d==3, then f(len-1, d==1)
    // precompute
    static long long f[20][2];
    static bool init=false;
    if(!init){
        init=true;
        for(int p=0;p<2;p++) f[0][p]=1;
        for(int len=1; len<20; len++){
            for(int prev1=0; prev1<2; prev1++){
                long long sum=0;
                for(int d=0; d<10; d++){
                    if(prev1 && d==3) continue;
                    sum += f[len-1][d==1?1:0];
                }
                f[len][prev1]=sum;
            }
        }
    }
    // count numbers from 0..X
    int prev1 = 0;
    bool valid = true;
    for(int i=0;i<n;i++){
        int dig = s[i]-'0';
        for(int d=0; d<dig; d++){
            if(prev1 && d==3) continue;
            // remaining positions n-i-1 free
            total += f[n-i-1][d==1?1:0];
        }
        // now fix d=dig
        if(prev1 && dig==3){ valid=false; break; }
        prev1 = (dig==1)?1:0;
    }
    if(valid) total += 1; // X itself
    // total counts 0..X including 0
    total -= 1; // remove 0
    return total;
}

int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        long long N;
        scanf("%lld",&N);
        long long lo=1, hi=2e18;
        // ensure hi valid - 2e18 might overflow; use safe bound
        hi = 2000000000000000000LL;
        while(lo<hi){
            long long mid = lo + (hi-lo)/2;
            if(countValid(mid) >= N) hi=mid;
            else lo=mid+1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
