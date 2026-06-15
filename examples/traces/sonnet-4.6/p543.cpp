#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n,r;
    cin>>n>>r;
    vector<int> a(n);
    for(auto &x:a) cin>>x;
    
    int full_tables=0;
    vector<int> parts;
    for(int i=0;i<n;i++){
        int rem=a[i]%r;
        full_tables+=a[i]/r;
        if(rem==0) continue;
        int p=(rem==1)?2:rem;
        parts.push_back(p);
    }
    
    int m=parts.size();
    if(m==0){cout<<full_tables<<endl;return 0;}
    
    const int INF=1e9;
    vector<int> dp(r+1,INF);
    dp[0]=0;
    
    for(int i=0;i<m;i++){
        int p=parts[i];
        vector<int> ndp(r+1,INF);
        for(int j=0;j<=r;j++){
            if(dp[j]==INF) continue;
            if(j>0 && j+p<=r){
                ndp[j+p]=min(ndp[j+p],dp[j]);
            }
            if(ndp[p]>dp[j]+1) ndp[p]=dp[j]+1;
        }
        dp=ndp;
    }
    
    int ans=INF;
    for(int j=0;j<=r;j++){
        if(dp[j]<ans) ans=dp[j];
    }
    cout<<full_tables+ans<<endl;
    return 0;
}
