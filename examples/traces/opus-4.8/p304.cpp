#include <bits/stdc++.h>
using namespace std;

int main(){
    int N,K; long long P;
    scanf("%d %d %lld",&N,&K,&P);
    vector<long long> B(K+1);
    for(int j=1;j<=K;j++) scanf("%lld",&B[j]);
    vector<vector<pair<int,int>>> gum(K+1);
    for(int i=1;i<=N;i++){
        int a,c; scanf("%d %d",&a,&c);
        gum[c].push_back({a,i});
    }
    for(int j=1;j<=K;j++) sort(gum[j].begin(),gum[j].end());
    
    const long long INF = LLONG_MAX/4;
    vector<long long> dp(N+1, INF);
    dp[0]=0;
    vector<vector<int>> take(K+1, vector<int>(N+1, 0));
    
    for(int j=1;j<=K;j++){
        int sz = gum[j].size();
        vector<long long> pref(sz+1,0);
        for(int t=0;t<sz;t++) pref[t+1]=pref[t]+gum[j][t].first;
        vector<long long> ndp = dp;
        for(int c=0;c<=N;c++){
            if(dp[c]==INF) continue;
            for(int t=1;t<=sz;t++){
                int nc=c+t;
                if(nc>N) break;
                long long cost = dp[c]+B[j]+pref[t];
                if(cost<=P && cost<ndp[nc]){
                    ndp[nc]=cost;
                    take[j][nc]=t;
                }
            }
        }
        dp=ndp;
    }
    
    int best=0;
    for(int c=N;c>=0;c--){
        if(dp[c]<=P && dp[c]<INF){best=c;break;}
    }
    
    vector<int> result;
    int c=best;
    for(int j=K;j>=1;j--){
        int t=take[j][c];
        for(int idx=0;idx<t;idx++) result.push_back(gum[j][idx].second);
        c-=t;
    }
    
    printf("%d\n",best);
    for(int i=0;i<(int)result.size();i++)
        printf("%d%c", result[i], i+1<(int)result.size()?' ':'\n');
    if(result.empty()) printf("\n");
    return 0;
}
