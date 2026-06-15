#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

ll C[70][70];

void precompute(){
    for(int i=0;i<70;i++){
        C[i][0]=1;
        for(int j=1;j<=i;j++) C[i][j]=C[i-1][j-1]+C[i-1][j];
    }
}

// count numbers in [1..n] with popcount==3
ll count3(ll n){
    if(n<=0) return 0;
    ll res=0;
    int ones=0;
    for(int i=62;i>=0;i--){
        if((n>>i)&1){
            // put 0 here, fill rest
            int need=3-ones;
            if(need>=0 && i>=need) res+=C[i][need];
            ones++;
            if(ones>3) break;
        }
    }
    if(ones==3) res++;
    return res;
}

int main(){
    precompute();
    int N;
    scanf("%d",&N);
    while(N--){
        ll M;
        scanf("%lld",&M);
        if(M==0){
            // K=1: count=0, K=2: count=0... many K give 0
            // Actually for K=1,2,3 count might be 0
            // Need to check uniqueness
            // count for K=1: [2,2] -> 0
            // K=2: [3,4] -> 0
            // K=3: [4,6] -> 0
            // Multiple K give 0, so NO
            printf("NO\n");
            continue;
        }
        // find all K with count3(2K)-count3(K)==M
        // K can be large... need smarter approach
        // For K with b bits (highest bit b-1), 
        // the count grows roughly as C(b-1,2)
        // Find range of b to search
        // For fixed b, K ranges [2^(b-1), 2^b-1]
        // count3(2K)-count3(K): 
        // 2K in [2^b, 2^(b+1)-2], K in [2^(b-1),2^b-1]
        // max count for b-bit K: roughly C(b,2)
        // We need b*(b-1)/2 >= M roughly
        // For M up to 2^31, b up to ~65536... too large
        
        // Hmm, wait. Let me reconsider.
        // C(b,2) = b*(b-1)/2. For M=2^31~2e9, b~63246.
        // K~2^63246... way too large to enumerate.
        
        // Need a smarter approach.
        // Let's think analytically.
        printf("NO\n"); // placeholder
    }
}
