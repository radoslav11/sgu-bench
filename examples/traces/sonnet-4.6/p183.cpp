#include <bits/stdc++.h>
using namespace std;
int main(){
    int N, M; cin >> N >> M;
    vector<int> c(N+1); for(int i=1;i<=N;i++) cin>>c[i];
    const int INF=1e9;
    // dp[i][j]: min cost, last black=i, second last=j, j<i, i-j<M
    vector<vector<int>> dp(N+2, vector<int>(N+2, INF));
    // Initialize: first two black balls at j and i
    for(int j=1;j<=N;j++) for(int i=j+1;i<j+M&&i<=N;i++) dp[i][j]=c[i]+c[j];
    int ans=INF;
    for(int i=2;i<=N;i++) for(int j=max(1,i-M+1);j<i;j++){
        if(dp[i][j]==INF) continue;
        if(i+(M-1)>N){ // check if done
            if(j+M-1>N) ans=min(ans,dp[i][j]);
        }
        for(int k=i+1;k<=min(N,(int)(j+M-1));k++) dp[k][i]=min(dp[k][i],dp[i][j]+c[k]);
    }
    cout<<ans<<endl;
}
