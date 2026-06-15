#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M; scanf("%d %d",&N,&M);
    vector<long long> C(N+1);
    for(int i=1;i<=N;i++) scanf("%lld",&C[i]);
    const long long INF=1e18;
    vector<vector<long long>> dp(N+1, vector<long long>(M+1, INF));
    for(int i=2;i<=N && i<=M;i++){
        for(int j=1;j<i;j++){
            int k=i-j;
            if(k<=M-1) dp[i][k]=min(dp[i][k], C[i]+C[j]);
        }
    }
    long long ans=INF;
    for(int i=2;i<=N;i++){
        for(int k=1;k<=M-1;k++){
            if(dp[i][k]==INF) continue;
            int j=i-k;
            if(j>=N-M+1) ans=min(ans,dp[i][k]);
            int pmax=min(N, j+M);
            for(int p=i+1;p<=pmax;p++){
                int nk=p-i;
                if(nk<=M-1 && dp[p][nk]>dp[i][k]+C[p])
                    dp[p][nk]=dp[i][k]+C[p];
            }
        }
    }
    printf("%lld\n",ans);
    return 0;
}
