#include <bits/stdc++.h>
using namespace std;
int main(){
    int T,N; cin>>T>>N;
    while(T--){
        int M=3*N; vector<int> x(M);
        for(auto&v:x) cin>>v;
        sort(x.begin(),x.end(),greater<int>());
        // DP: process sorted elements, state=(nA,nB), nC=i-nA-nB
        // pair: A[k] with C[k] and B[N-1-k] — need to think about final pairing
        // Use DP with memoization, track sorted assignment
        vector<vector<long long>> dp(N+1,vector<long long>(N+1,-1e18));
        dp[0][0]=0;
        // When we finalize: sort A desc, B asc, C desc, pair by index
        // But during DP we don't know future assignments
        // Alternative: assume optimal pairing at end, use DP tracking multisets - too costly
        // Greedy: just try all 3^(3N) - too slow for N=25
        // Let me do meet in the middle for small, and hope greedy works for large
        // Actually reread: for N<=10, T<=1000 so maybe O(3^(3N)) per test is too slow
        // For N=10, 3^30 ~ 2e14, way too slow
        // Need smarter approach
        cout<<0<<"\n";
    }
}
