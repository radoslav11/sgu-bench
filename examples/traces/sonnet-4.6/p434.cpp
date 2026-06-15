#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> s(n),d(n);
    for(auto&x:s)cin>>x;
    for(auto&x:d)cin>>x;
    long long ss=0,sd=0;
    for(int x:s)ss+=x;
    for(int x:d)sd+=x;
    if(ss!=sd){cout<<-1;return 0;}
    vector<int> exc,def;
    for(int i=0;i<n;i++){
        if(s[i]>d[i])exc.push_back(s[i]-d[i]);
        else if(s[i]<d[i])def.push_back(d[i]-s[i]);
    }
    int ne=exc.size(),nd=def.size();
    // min pourings = ne+nd - max_matching(exc[i]==def[j])
    // Use bitmask DP for max matching
    vector<int> dp(1<<nd,-1);
    dp[0]=0;
    for(int i=0;i<ne;i++){
        vector<int> ndp(1<<nd,-1);
        for(int mask=0;mask<(1<<nd);mask++){
            if(dp[mask]<0)continue;
            // try matching exc[i] with some def[j] not in mask
            for(int j=0;j<nd;j++){
                if(mask>>j&1)continue;
                if(exc[i]==def[j]){
                    int nm=mask|(1<<j);
                    ndp[nm]=max(ndp[nm],dp[mask]+1);
                }
            }
            // or don't match
            ndp[mask]=max(ndp[mask],dp[mask]);
        }
        dp=ndp;
    }
    int best=0;
    for(int mask=0;mask<(1<<nd);mask++)best=max(best,dp[mask]);
    cout<<ne+nd-best;
}
