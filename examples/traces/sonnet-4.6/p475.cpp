#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m; cin>>n>>m;
    vector<int> w(n),t(n),s(n);
    for(int i=0;i<n;i++) cin>>w[i]>>t[i]>>s[i];
    vector<int> c(m),D(m),d(m);
    for(int i=0;i<m;i++) cin>>c[i]>>D[i]>>d[i];
    int full=1<<n;
    const long long INF=1e18;
    vector<long long> dp(full,INF);
    for(int mask=1;mask<full;mask++){
        long long cost=0;
        for(int j=0;j<n;j++) if(mask>>j&1) cost+=s[j];
        dp[mask]=cost;
    }
    for(int i=0;i<m;i++){
        // precompute for each mask: weight, max walk time
        vector<int> wt(full,0); vector<long long> maxwalk(full,0);
        for(int mask=0;mask<full;mask++){
            for(int j=0;j<n;j++) if(mask>>j&1) wt[mask]+=w[j];
        }
        for(int mask=0;mask<full;mask++){
            // walkers are those NOT in mask
            for(int j=0;j<n;j++) if(!(mask>>j&1)) maxwalk[mask]=max(maxwalk[mask],(long long)t[j]);
        }
        // for this riffle, from prev dp, try all new_mask
        vector<long long> ndp(full,INF);
        for(int prev=1;prev<full;prev++){
            if(dp[prev]==INF) continue;
            for(int nm=1;nm<full;nm++){
                long long sw=0;
                for(int j=0;j<n;j++) if(((prev>>j&1)^(nm>>j&1))) sw+=s[j];
                long long raft_t=(wt[nm]>c[i])?D[i]:d[i];
                long long seg=max(raft_t,maxwalk[nm]);
                long long val=dp[prev]+sw+seg;
                if(val<ndp[nm]) ndp[nm]=val;
            }
        }
        dp=ndp;
    }
    long long ans=INF;
    for(int mask=1;mask<full;mask++){
        if(dp[mask]==INF) continue;
        long long cost=0;
        for(int j=0;j<n;j++) if(mask>>j&1) cost+=s[j];
        ans=min(ans,dp[mask]+cost);
    }
    cout<<ans<<endl;
}
