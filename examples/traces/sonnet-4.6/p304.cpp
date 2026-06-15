#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,K,P;
    scanf("%d%d%d",&N,&K,&P);
    vector<int> B(K+1);
    for(int j=1;j<=K;j++) scanf("%d",&B[j]);
    vector<vector<pair<int,int>>> gum(K+1);
    for(int i=1;i<=N;i++){
        int a,c;
        scanf("%d%d",&a,&c);
        gum[c].push_back({a,i});
    }
    for(int j=1;j<=K;j++) sort(gum[j].begin(),gum[j].end());
    
    const int INF=1e9;
    vector<int> dp(N+1, INF);
    dp[0]=0;
    vector<vector<short>> choice(K+1, vector<short>(N+1,-1));
    
    for(int j=1;j<=K;j++){
        auto& teeth=gum[j];
        int m=(int)teeth.size();
        vector<int> S(m+1,0);
        for(int i=1;i<=m;i++) S[i]=S[i-1]+teeth[i-1].first;
        
        vector<int> ndp(N+1,INF);
        for(int k=0;k<=N;k++){
            if(dp[k]==INF) continue;
            for(int t=0;t<=m&&k+t<=N;t++){
                int cost=(t>0?B[j]+S[t]:0);
                int nc=dp[k]+cost;
                if(nc<ndp[k+t]){
                    ndp[k+t]=nc;
                    choice[j][k+t]=(short)t;
                }
            }
        }
        dp=ndp;
    }
    
    int best=0;
    for(int k=N;k>=0;k--){
        if(dp[k]<=P){best=k;break;}
    }
    printf("%d\n",best);
    
    vector<int> res;
    int k=best;
    for(int j=K;j>=1;j--){
        if(k<0||choice[j][k]<0) continue;
        short t=choice[j][k];
        auto& teeth=gum[j];
        for(int i=0;i<t;i++) res.push_back(teeth[i].second);
        k-=t;
    }
    sort(res.begin(),res.end());
    for(int i=0;i<(int)res.size();i++){
        if(i) printf(" ");
        printf("%d",res[i]);
    }
    printf("\n");
    return 0;
}
